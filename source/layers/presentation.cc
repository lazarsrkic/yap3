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
                    m_buffer[presentation::LAYER_APP_CONTROL_OFFSET] =
                        buffer[presentation::LAYER_APP_CONTROL_OFFSET];
                    std::memcpy(
                        m_buffer.data() +
                            static_cast<size_t>(
                                presentation::LAYER_IPC_PAYLOAD_OFFSET),
                        buffer + static_cast<size_t>(
                                     presentation::LAYER_IPC_PAYLOAD_OFFSET),
                        (size - static_cast<size_t>(
                                    presentation::LAYER_IPC_PAYLOAD_OFFSET)));
                    m_buffer_size = size;
                    // call function to build frame for transport layer
                    return true;
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
                    m_buffer[presentation::LAYER_APP_CONTROL_OFFSET] =
                        buffer[presentation::LAYER_APP_CONTROL_OFFSET];
                    std::memcpy(
                        m_buffer.data() +
                            static_cast<size_t>(
                                presentation::LAYER_IPC_PAYLOAD_OFFSET),
                        buffer + static_cast<size_t>(
                                     presentation::LAYER_IPC_PAYLOAD_OFFSET),
                        (size - static_cast<size_t>(
                                    presentation::LAYER_IPC_PAYLOAD_OFFSET)));
                    m_buffer_size = size;
                    // call function to send message to application layer
                    return true;
            }
        } else {
            spdlog::error("{} error:Incorrect buffer provided", __func__);
        }
    }

    return false;
}

}  // namespace layers
}  // namespace yap3
