#ifndef SOURCE_LAYERS_TRANSPORT_H_
#define SOURCE_LAYERS_TRANSPORT_H_

#include "layers/layer.h"

#include <memory>

namespace yap3 {
namespace layers {
namespace transport {}  // namespace transport

class Transport : public Layer {
public:
    void wrapping_layers(
        std::shared_ptr<Layer> application_layer,
        std::shared_ptr<Layer> presentation_layer,
        std::shared_ptr<Layer> datalink_layer) noexcept override;

private:
    std::shared_ptr<Layer> m_application_layer{nullptr};
    std::shared_ptr<Layer> m_presentation_layer{nullptr};
    std::shared_ptr<Layer> m_datalink_layer{nullptr};
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_TRANSPORT_H_
