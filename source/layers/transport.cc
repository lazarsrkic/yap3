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

bool Transport::process_message(std::uint8_t const* buffer,
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

    std::uint16_t crc = yap3::utils::Crc16{}(buffer, size);
    if (crc == 0) {
        spdlog::error("{} error: crc calculation failed", __func__);
        return false;
    }

    m_buffer[transport::LAYER_CRC_OFFSET] = crc >> 8;
    m_buffer[transport::LAYER_CRC_OFFSET + 1] = crc & 0xFF;

    m_buffer[transport::LAYER_FRAME_CONTROL_OFFSET] =
        0;  // TODO: Populate this one
    std::memcpy(m_buffer.data() + transport::LAYER_IPC_PAYLOAD_OFFSET, buffer,
                size);

    std::lock_guard<std::mutex> lock{m_tx_ringbuffer_mutex};

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

}  // namespace layers
}  // namespace yap3
