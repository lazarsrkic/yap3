#include "layers/application.h"
#include "layers/presentation.h"
#include "layers/transport.h"

namespace yap3 {
namespace layers {

void Presentation::wrapping_layers(
    std::shared_ptr<Application> application_layer,
    std::shared_ptr<Transport> transport_layer) noexcept {
    m_application_layer = application_layer;
    m_transport_layer = transport_layer;
}

bool Presentation::encrypt(std::int8_t const* buffer,
                           std::uint8_t const size) noexcept {
    return true;
}

bool Presentation::decrypt(std::int8_t const* buffer,
                           std::uint8_t const size) noexcept {
    return true;
}

}  // namespace layers
}  // namespace yap3
