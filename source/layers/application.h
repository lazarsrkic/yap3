#ifndef SOURCE_LAYERS_APPLICATION_H_
#define SOURCE_LAYERS_APPLICATION_H_

#include "layers/layer.h"

#include <memory>

namespace yap3 {
namespace layers {
namespace application {}  // namespace application

class Application : public Layer {
public:
    void wrapping_layers(std::shared_ptr<Layer> presentation_layer,
                         std::shared_ptr<Layer>,
                         std::shared_ptr<Layer>) noexcept override;

private:
    std::shared_ptr<Layer> m_presentation_layer{nullptr};
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_APPLICATION_H_
