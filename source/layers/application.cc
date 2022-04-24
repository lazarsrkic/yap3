#include "layers/application.h"

namespace yap3 {
namespace layers {
namespace application {}  // namespace application

void Application::wrapping_layers(std::shared_ptr<Layer> presentation_layer,
                                  std::shared_ptr<Layer>,
                                  std::shared_ptr<Layer>) noexcept {
    m_presentation_layer = presentation_layer;
}

}  // namespace layers
}  // namespace yap3
