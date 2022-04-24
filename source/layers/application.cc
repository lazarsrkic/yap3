#include "layers/application.h"
#include "layers/presentation.h"

namespace yap3 {
namespace layers {
namespace application {}  // namespace application

void Application::wrapping_layers(
    std::shared_ptr<Presentation> presentation_layer) noexcept {
    m_presentation_layer = presentation_layer;
}

}  // namespace layers
}  // namespace yap3
