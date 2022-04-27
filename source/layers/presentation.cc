#include "layers/application.h"
#include "layers/presentation.h"
#include "layers/transport.h"

#include <spdlog/spdlog.h>

namespace yap3 {
namespace layers {

Presentation::Presentation() noexcept {
    std::memset(m_buffer.data(), 0x00, presentation::LAYER_MAX_PAYLOAD_SIZE);
    m_buffer_size = 0;
}

void Presentation::wrapping_layers(
    std::shared_ptr<Application> application_layer,
    std::shared_ptr<Transport> transport_layer) noexcept {
    m_application_layer = application_layer;
    m_transport_layer = transport_layer;
}

bool Presentation::encrypt(std::uint8_t const* buffer,
                           std::size_t const size) noexcept {
    std::memset(m_buffer.data(), 0x00, presentation::LAYER_MAX_PAYLOAD_SIZE);
    m_buffer_size = 0;

    if (m_transport_layer) {
        if (buffer != nullptr &&
            (size <= presentation::LAYER_MAX_PAYLOAD_SIZE &&
             size >= presentation::LAYER_IPC_PAYLOAD_OFFSET)) {
            switch (static_cast<presentation::EncryptType>(
                buffer[presentation::LAYER_APP_CONTROL_OFFSET])) {
                case presentation::EncryptType::kTypeA:
                    break;
                case presentation::EncryptType::kTypeB:
                    break;
                case presentation::EncryptType::kNone:
                default:
                    std::memcpy(m_buffer.data(), buffer, size);
                    m_buffer_size = size;

                    return m_transport_layer->process_message(m_buffer.data(),
                                                              m_buffer_size);
            }
        } else {
            spdlog::error("{} error:Incorrect buffer provided", __func__);
        }
    }
    return false;
}

bool Presentation::decrypt(std::uint8_t const* buffer,
                           std::size_t const size) noexcept {
    std::memset(m_buffer.data(), 0x00, presentation::LAYER_MAX_PAYLOAD_SIZE);
    m_buffer_size = 0;

    if (m_application_layer) {
        if (buffer != nullptr &&
            (size <= presentation::LAYER_MAX_PAYLOAD_SIZE &&
             size >= presentation::LAYER_IPC_PAYLOAD_OFFSET)) {
            switch (static_cast<presentation::EncryptType>(
                buffer[presentation::LAYER_APP_CONTROL_OFFSET])) {
                case presentation::EncryptType::kTypeA:
                    break;
                case presentation::EncryptType::kTypeB:
                    break;
                case presentation::EncryptType::kNone:
                default:
                    std::memcpy(m_buffer.data(), buffer, size);
                    m_buffer_size = size;

                    return m_application_layer->on_frame_receive(
                        m_buffer.data(), m_buffer_size);
            }
        } else {
            spdlog::error("{} error:Incorrect buffer provided", __func__);
        }
    }

    return false;
}

}  // namespace layers
}  // namespace yap3
