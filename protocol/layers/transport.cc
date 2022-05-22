#include "layers/datalink.h"
#include "layers/presentation.h"
#include "layers/transport.h"
#include "utils/crc16.h"

namespace yap3 {
namespace layers {

void Transport::wrapping_layers(
    std::shared_ptr<Presentation> presentation_layer,
    std::shared_ptr<Datalink> datalink_layer) noexcept {
    m_presentation_layer = presentation_layer;
    m_datalink_layer = datalink_layer;
}

bool Transport::process_ipc_message(std::uint8_t const* buffer,
                                    std::size_t const size) noexcept {
    if (buffer == nullptr || size == 0) {
        return false;
    }

    std::memset(m_buffer.data(), 0x00, transport::MAX_COMPLETE_MESSAGE_SIZE);
    m_buffer_size = yap3::configuration::FLAG_SIZE * 2 +
                    yap3::configuration::CRC_SIZE +
                    yap3::configuration::FRAME_CONTROL_SIZE + size;

    m_buffer[transport::LAYER_START_FLAG_OFFSET] = transport::START;
    m_buffer[m_buffer_size - 1] = transport::STOP;

    {
        std::lock_guard<std::mutex> lock{m_tx_ringbuffer_mutex};
        m_buffer[transport::LAYER_FRAME_CONTROL_OFFSET] =
            ((static_cast<std::uint8_t>(transport::Message::kData) & 0x7)
             << 4) |
            (static_cast<std::uint8_t>(m_tx_ringbuffer.head()) & 0xF);
    }

    std::memcpy(m_buffer.data() + transport::LAYER_IPC_PAYLOAD_OFFSET, buffer,
                size);

    std::uint16_t const crc =
        yap3::utils::Crc16{}(
            m_buffer.data() + transport::LAYER_FRAME_CONTROL_OFFSET,
            size + configuration::FRAME_CONTROL_SIZE) &
        0xDFDF;
    if (crc == 0) {
        spdlog::error("{} error: crc calculation failed", __func__);
        return false;
    }

    m_buffer[transport::LAYER_CRC_OFFSET] = crc >> 8;
    m_buffer[transport::LAYER_CRC_OFFSET + 1] = crc & 0xFF;

    if (!m_tx_ringbuffer.full()) {
        if (m_tx_ringbuffer.put(m_buffer.data(), m_buffer_size)) {
            spdlog::info("New message added to tx queue");
            return true;
        } else {
            spdlog::error("Failed to add new message to tx queue!");
        }

    } else {
        spdlog::error("Tx queue is full! Failed to add new message!");
    }

    return false;
}

void Transport::transmit() noexcept {
    if (bus_activity_timer.expired()) {
        spdlog::error("{} error: Bus activity timer expired!", __func__);
        protocol_reset(transport::ResetType::kRequestReset);
    } else {
        auto ack_timer_expired = ack_timer.expired();

        if (ack_timer_expired) {
            ack_timer.reset();
        }

        if (ack_timer_expired && m_sliding_window.offset_to_send) {
            if (m_sliding_window.retry_count >=
                transport::SLIDING_WINDOW_MAX_RETRIES) {
                spdlog::error("{} error: Retry count excided!", __func__);
                protocol_reset(transport::ResetType::kRequestReset);
            } else {
                m_sliding_window.retry_count++;
                for (auto msg_id = 0; msg_id < m_sliding_window.offset_to_send;
                     msg_id++) {
                    send_message(msg_id);
                }
            }
        } else {
            {
                std::lock_guard<std::mutex> lock(m_tx_ringbuffer_mutex);

                while (
                    m_sliding_window.offset_to_send < transport::WINDOW_SIZE &&
                    m_sliding_window.offset_to_send < m_tx_ringbuffer.size()) {
                    if (send_message(m_sliding_window.offset_to_send)) {
                        m_sliding_window.offset_to_send++;
                    } else {
                        spdlog::error(
                            "Sliding window send stopped due to failure on "
                            "message sending");
                        break;
                    }
                }
            }

            if (keep_alive_timer.expired()) {
                send_internal_message(transport::Message::kKeepAlive,
                                      m_sliding_window.last_acked);
            }
        }
    }
}

void Transport::receive() noexcept {
    static std::array<std::uint8_t, transport::READ_BUFFER_SIZE> buffer{};
    static std::size_t buffer_size{0};

    std::array<std::uint8_t, transport::READ_BUFFER_SIZE> in_buffer{};
    ssize_t in_buffer_size{0};
    std::array<std::uint8_t, transport::MAX_COMPLETE_MESSAGE_SIZE>
        remain_buffer{};
    std::size_t remain_buffer_size{0};

    in_buffer_size = m_datalink_layer->read(
        in_buffer.data(), transport::READ_BUFFER_SIZE - buffer_size);

    if (in_buffer_size >= 0 && transport::READ_BUFFER_SIZE > (in_buffer_size)) {
        std::memcpy(buffer.data() + buffer_size, in_buffer.data(),
                    in_buffer_size);
        buffer_size = buffer_size + in_buffer_size;

        if (!extract_messages(in_buffer.data(), in_buffer_size,
                              remain_buffer.data(), &remain_buffer_size)) {
            spdlog::error(
                "Failed to extract messages, reseting receive buffer");
            std::memset(buffer.data(), 0x00, transport::READ_BUFFER_SIZE);
            buffer_size = 0;
            return;
        }

        std::memcpy(buffer.data(), remain_buffer.data(), remain_buffer_size);
        buffer_size = remain_buffer_size;
    } else {
        spdlog::error("There is no data on the line to receive");
    }
}

bool Transport::extract_messages(std::uint8_t const* buffer,
                                 std::size_t const buffer_size,
                                 std::uint8_t* remain_buffer,
                                 std::size_t* remain_size) noexcept {
    bool retval = true;
    std::size_t start = 0;
    std::size_t end = buffer_size;

    std::size_t msg_start = 0;
    std::size_t msg_size = 0;

    std::size_t index = 0;

    bool start_flag_found = false;
    bool stop_flag_found = false;
    bool previous_is_escape_flag = false;

    while (start < end) {
        msg_start = 0;
        msg_size = 0;

        start_flag_found = false;
        stop_flag_found = false;
        previous_is_escape_flag = false;

        for (index = start; index < end; index++) {
            start = index + 1;
            if (buffer[index] == transport::ESCAPE) {
                previous_is_escape_flag = true;
                continue;
            } else {
                previous_is_escape_flag = false;
            }

            if (buffer[index] == transport::START && !previous_is_escape_flag) {
                start_flag_found = true;
                msg_start = index;
                break;
            }
        }

        if (start_flag_found) {
            for (index = msg_start + 1;
                 index < end && index < transport::MAX_COMPLETE_MESSAGE_SIZE;
                 index++) {
                start = index + 1;
                if (buffer[index] == transport::ESCAPE) {
                    previous_is_escape_flag = true;
                    continue;
                } else {
                    previous_is_escape_flag = false;
                }

                if (buffer[index] == transport::STOP &&
                    !previous_is_escape_flag) {
                    stop_flag_found = true;
                    msg_size = (index - msg_start) + 1;
                    break;
                } else if (buffer[index] == transport::START &&
                           !previous_is_escape_flag) {
                    msg_start = index;
                    break;
                }
            }

            if (stop_flag_found) {
                if (!process_message(buffer + msg_start, msg_size)) {
                    spdlog::error("Failed to process message!");
                    retval = false;
                    break;
                }
            }
        }
    }

    if (start_flag_found && !stop_flag_found) {
        *remain_size = end - msg_start;
        std::memcpy(remain_buffer, buffer + msg_start, *remain_size);
    } else {
        *remain_size = 0;
    }

    return retval;
}

bool Transport::process_message(std::uint8_t const* buffer,
                                std::size_t const size) noexcept {
    if (buffer == nullptr ||
        size < (yap3::configuration::FLAG_SIZE * 2 +
                yap3::configuration::CRC_SIZE +
                yap3::configuration::FRAME_CONTROL_SIZE) ||
        size > transport::MAX_COMPLETE_MESSAGE_SIZE) {
        return false;
    }

    std::size_t message_size = size - (yap3::configuration::FLAG_SIZE * 2 +
                                       yap3::configuration::CRC_SIZE);

    std::uint16_t received_crc = buffer[transport::LAYER_CRC_OFFSET] << 8 |
                                 buffer[transport::LAYER_CRC_OFFSET + 1];
    std::uint16_t calculated_crc =
        yap3::utils::Crc16{}(buffer + transport::LAYER_FRAME_CONTROL_OFFSET,
                             message_size) &
        0xDFDF;

    std::uint8_t const message_id =
        buffer[transport::LAYER_FRAME_CONTROL_OFFSET] & 0xF;
    transport::Message const message_type = static_cast<transport::Message>(
        (buffer[transport::LAYER_FRAME_CONTROL_OFFSET] >> 4) & 0xF);

    if (calculated_crc != received_crc) {
        spdlog::error("Invalid CRC! Received: {}, Expected {}!", received_crc,
                      calculated_crc);
        send_internal_message(transport::Message::kNack, message_id);
        return false;
    }
    bus_activity_timer.reset();

    switch (message_type) {
        case transport::Message::kData:
            spdlog::debug("Received kData message");
            if (message_id == ((m_sliding_window.last_acked + 1) %
                               yap3::configuration::MAX_RINGBUFFER_SIZE)) {
                if (m_presentation_layer->decrypt(
                        buffer + transport::LAYER_IPC_PAYLOAD_OFFSET,
                        message_size)) {
                    spdlog::info("Sending kAck for message id {}", message_id);
                    send_internal_message(transport::Message::kAck, message_id);

                    m_sliding_window.last_acked =
                        (m_sliding_window.last_acked + 1) %
                        yap3::configuration::MAX_RINGBUFFER_SIZE;
                } else {
                    spdlog::error("Failed to decrypt data message");
                    send_internal_message(transport::Message::kNack,
                                          message_id);
                }
            }
            break;
        case transport::Message::kAck:
            spdlog::debug("Received kAck message");
            process_ack_message(message_id);
            break;
        case transport::Message::kReset:
            spdlog::debug("Received kReset message");
            protocol_reset(transport::ResetType::kResetRequested);
            break;
        case transport::Message::kKeepAlive:
            spdlog::debug("Received kKeepAlive message");
            process_ack_message(message_id);
            break;
        case transport::Message::kNack:
            spdlog::debug("Received kNack message");
            if (message_id == m_sliding_window.to_be_acked &&
                m_sliding_window.offset_to_send) {
                if (m_sliding_window.retry_count >=
                    transport::SLIDING_WINDOW_MAX_RETRIES) {
                    spdlog::error("{} error: Retry count excided!", __func__);
                    protocol_reset(transport::ResetType::kRequestReset);
                } else {
                    m_sliding_window.retry_count++;
                    for (auto msg_id = 0;
                         msg_id < m_sliding_window.offset_to_send; msg_id++) {
                        send_message(msg_id);
                    }
                }
            }
            break;
        default:
            spdlog::error("Unknown message type");
            send_internal_message(transport::Message::kNack, message_id);
            return false;
    }

    return true;
}

void Transport::process_ack_message(std::uint8_t const message_id) noexcept {
    std::uint8_t const max_msg_id_to_process =
        (m_sliding_window.to_be_acked + m_sliding_window.offset_to_send) %
        yap3::configuration::MAX_RINGBUFFER_SIZE;
    std::uint8_t const old_offset_to_send = m_sliding_window.offset_to_send;

    std::size_t acked_count{0};

    if (max_msg_id_to_process < m_sliding_window.to_be_acked) {
        if (message_id >= m_sliding_window.to_be_acked ||
            message_id < max_msg_id_to_process) {
            m_sliding_window.to_be_acked =
                (message_id + 1) % yap3::configuration::MAX_RINGBUFFER_SIZE;

            if (max_msg_id_to_process < m_sliding_window.to_be_acked) {
                m_sliding_window.offset_to_send =
                    max_msg_id_to_process +
                    yap3::configuration::MAX_RINGBUFFER_SIZE -
                    m_sliding_window.to_be_acked;
                acked_count =
                    (old_offset_to_send - m_sliding_window.offset_to_send) %
                    yap3::configuration::MAX_RINGBUFFER_SIZE;
            } else {
                m_sliding_window.offset_to_send =
                    max_msg_id_to_process - m_sliding_window.to_be_acked;
                acked_count =
                    (old_offset_to_send - m_sliding_window.offset_to_send) %
                    yap3::configuration::MAX_RINGBUFFER_SIZE;
            }
        }
    } else {
        if (message_id >= m_sliding_window.to_be_acked &&
            message_id < max_msg_id_to_process) {
            m_sliding_window.to_be_acked =
                (message_id + 1) % yap3::configuration::MAX_RINGBUFFER_SIZE;
            m_sliding_window.offset_to_send =
                max_msg_id_to_process - m_sliding_window.to_be_acked;

            acked_count =
                (old_offset_to_send - m_sliding_window.offset_to_send) %
                yap3::configuration::MAX_RINGBUFFER_SIZE;
        }
    }

    if (acked_count != 0) {
        std::lock_guard<std::mutex> lock(m_tx_ringbuffer_mutex);
        while (acked_count > 0) {
            if (!m_tx_ringbuffer.release()) {
                spdlog::error("Failed to release it from tx ringbuffer");
            }

            acked_count--;
        }

        ack_timer.reset();
        m_sliding_window.retry_count = 0;
    }
}

void Transport::protocol_reset(
    transport::ResetType const& reset_type) noexcept {
    spdlog::error("Reseting protocol!");

    bus_activity_timer.reset();
    ack_timer.reset();
    keep_alive_timer.reset();

    m_tx_ringbuffer.reset();

    m_sliding_window.reset();

    if (reset_type == transport::ResetType::kRequestReset) {
        if (!send_internal_message(transport::Message::kReset)) {
            spdlog::error("{} error: Failed to send reset message", __func__);
        }
    }
}

bool Transport::send_message(std::uint8_t const offset) noexcept {
    static std::array<std::uint8_t, transport::MAX_COMPLETE_MESSAGE_SIZE>
        buffer{};
    std::size_t buffer_size = 0;
    if (m_tx_ringbuffer.retrieve(offset, buffer.data(), &buffer_size)) {
        if (m_datalink_layer->write(buffer.data(), buffer_size)) {
            ack_timer.reset();
            keep_alive_timer.reset();
            return true;
        }
    }

    spdlog::error("Failed to send frame with offset {}", offset);
    return false;
}

bool Transport::send_internal_message(transport::Message const& msg_type,
                                      std::uint8_t const message_id) noexcept {
    static std::array<std::uint8_t, yap3::configuration::FLAG_SIZE * 2 +
                                        yap3::configuration::CRC_SIZE +
                                        yap3::configuration::FRAME_CONTROL_SIZE>
        internal_message{yap3::layers::transport::START, 0x00, 0x00, 0x00,
                         yap3::layers::transport::STOP};

    std::uint8_t const frame_ctrl =
        ((static_cast<std::uint8_t>(msg_type) & 0x7) << 4) |
        (static_cast<std::uint8_t>(message_id) & 0xF);

    internal_message[transport::LAYER_FRAME_CONTROL_OFFSET] = frame_ctrl;

    std::uint16_t const crc =
        yap3::utils::Crc16{}(
            internal_message.data() + transport::LAYER_FRAME_CONTROL_OFFSET,
            configuration::FRAME_CONTROL_SIZE) &
        0xDFDF;
    if (crc == 0 && msg_type != transport::Message::kReset) {
        spdlog::error("{} error: crc calculation failed", __func__);
        return false;
    }

    internal_message[transport::LAYER_CRC_OFFSET] = crc >> 8;
    internal_message[transport::LAYER_CRC_OFFSET + 1] = crc & 0xFF;

    if (m_datalink_layer->write(internal_message.data(),
                                internal_message.size())) {
        keep_alive_timer.reset();
        return true;
    }
    return false;
}

}  // namespace layers
}  // namespace yap3
