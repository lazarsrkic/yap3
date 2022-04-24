#include "layers/transport.h"

namespace yap3 {
namespace layers {

void Transport::wrapping_layers(
    std::shared_ptr<Layer> application_layer,
    std::shared_ptr<Layer> presentation_layer,
    std::shared_ptr<Layer> datalink_layer) noexcept {
    m_application_layer = application_layer;
    m_presentation_layer = presentation_layer;
    m_datalink_layer = datalink_layer;
}

}  // namespace layers
}  // namespace yap3
