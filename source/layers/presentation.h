#ifndef SOURCE_LAYERS_PRESENTATION_H_
#define SOURCE_LAYERS_PRESENTATION_H_

#include "configuration.h"

#include <array>
#include <cstdint>
#include <memory>

namespace yap3 {
namespace layers {

class Application;
class Transport;
namespace presentation {
constexpr std::uint8_t LAYER_MAX_PAYLOAD_SIZE{
    yap3::configuration::APP_CONTROL_SIZE +
    yap3::configuration::MAX_IPC_MSG_PAYLOAD_SIZE};
constexpr std::uint8_t LAYER_IPC_PAYLOAD_OFFSET{
    yap3::configuration::APP_CONTROL_SIZE};
constexpr std::uint8_t LAYER_APP_CONTROL_OFFSET{0U};

enum class EncryptType : std::uint8_t { kNone, kTypeA, kTypeB };
}  // namespace presentation

class Presentation {
public:
    Presentation() noexcept;

    void wrapping_layers(std::shared_ptr<Application> application_layer,
                         std::shared_ptr<Transport> transport_layer) noexcept;

    bool encrypt(std::uint8_t const* buffer, std::size_t const size) noexcept;
    bool decrypt(std::uint8_t const* buffer, std::size_t const size) noexcept;

private:
    std::shared_ptr<Application> m_application_layer;
    std::shared_ptr<Transport> m_transport_layer;
    std::array<std::uint8_t, presentation::LAYER_MAX_PAYLOAD_SIZE> m_buffer;
    std::size_t m_buffer_size;
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_PRESENTATION_H_
