#include "layers/application.h"
#include "layers/datalink.h"
#include "layers/presentation.h"
#include "layers/transport.h"

namespace yap3 {
namespace layers {

void Transport::wrapping_layers(
    std::shared_ptr<Application> application_layer,
    std::shared_ptr<Presentation> presentation_layer,
    std::shared_ptr<Datalink> datalink_layer) noexcept {
    m_application_layer = application_layer;
    m_presentation_layer = presentation_layer;
    m_datalink_layer = datalink_layer;
}

}  // namespace layers
}  // namespace yap3
