#include "layers/application.h"
#include "layers/presentation.h"

#include <spdlog/spdlog.h>

#include <cstring>

namespace yap3 {
namespace layers {

Application::Application() noexcept {
    std::memset(m_buffer.data(), 0x00, application::LAYER_MAX_PAYLOAD_SIZE);
    m_buffer_size = 0;
}

void Application::wrapping_layers(
    std::shared_ptr<Presentation> presentation_layer) noexcept {
    m_presentation_layer = presentation_layer;
}

bool Application::on_ipc_message_receive(std::uint8_t const* buffer,
                                         std::size_t const size) noexcept {
    std::memset(m_buffer.data(), 0x00, application::LAYER_MAX_PAYLOAD_SIZE);
    m_buffer_size = 0;

    if (m_presentation_layer) {
        if (buffer != nullptr &&
            (size <= configuration::MAX_IPC_MSG_PAYLOAD_SIZE &&
             size >= application::LAYER_IPC_MESSAGE_PAYLOAD_OFFSET)) {
            m_buffer[application::LAYER_MESSAGE_APP_CONTROL_OFFSET] = 0x00;
            m_buffer[application::LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET] =
                buffer
                    [application::LAYER_IPC_MESSAGE_SENDER_RECEIVER_ID_OFFSET];

            std::memcpy(
                m_buffer.data() + application::LAYER_MESSAGE_PAYLOAD_OFFSET,
                buffer + application::LAYER_IPC_MESSAGE_PAYLOAD_OFFSET,
                (size - application::LAYER_IPC_MESSAGE_PAYLOAD_OFFSET));
            m_buffer_size = size -
                            application::LAYER_IPC_MESSAGE_PAYLOAD_OFFSET +
                            application::LAYER_MESSAGE_PAYLOAD_OFFSET;

            if (m_presentation_layer->encrypt(m_buffer.data(), m_buffer_size)) {
                spdlog::info("IPC message successfully processed");
                return true;
            }
        } else {
            spdlog::error("{} error:Incorrect buffer provided", __func__);
        }
    }
    spdlog::error("{} failed!", __func__);
    return false;
}

bool Application::on_ipc_message_send(std::uint8_t const receiver_id,
                                      std::uint8_t* buffer,
                                      std::size_t* size) noexcept {
    if (receiver_id > 0x0F) {
        return false;
    }

    if (buffer == nullptr || size == nullptr) {
        return false;
    }

    spdlog::info("New message for receiver with id: {}!", receiver_id);

    std::lock_guard<std::mutex> lock{m_rx_ringbuffers_mutex};

    if (!m_rx_ringbuffers[receiver_id].empty()) {
        if (m_rx_ringbuffers[receiver_id].remove(buffer, size)) {
            spdlog::info("Message for client {} removed/taken from rx queue!",
                         receiver_id);
            return true;
        } else {
            spdlog::error(
                "Removal/Getting message for receiver with id {} failed!",
                receiver_id);
        }
    } else {
        spdlog::error(
            "Message for receiver with id {} can't be removed from rx queue! "
            "Queue is empty!",
            receiver_id);
    }
    return false;
}

bool Application::on_frame_receive(std::uint8_t const* buffer,
                                   std::size_t const size) noexcept {
    if (buffer == nullptr ||
        size < application::LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET) {
        return false;
    }

    std::uint8_t const receiver_id =
        *(buffer + application::LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET) & 0x0F;

    spdlog::info("New message for receiver with id: {}!", receiver_id);

    std::lock_guard<std::mutex> lock{m_rx_ringbuffers_mutex};

    if (!m_rx_ringbuffers[receiver_id].full()) {
        if (m_rx_ringbuffers[receiver_id].put(
                buffer + application::LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET,
                size - application::LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET)) {
            spdlog::info("Message for receiver with id {} added to rx queue!",
                         receiver_id);
            return true;
        } else {
            spdlog::error("Adding message for receiver with id {} failed!",
                          receiver_id);
        }

    } else {
        spdlog::error(
            "Message for receiver with id {} can't be added to rx queue! Queue "
            "is full!",
            receiver_id);
    }

    return false;
}

}  // namespace layers
}  // namespace yap3
