#ifndef SOURCE_LAYERS_LAYER_H_
#define SOURCE_LAYERS_LAYER_H_

#include <memory>

namespace yap3 {
namespace layers {

class Layer {
public:
    Layer() = default;
    virtual ~Layer() = default;
    Layer(Layer const&) = delete;
    Layer(Layer&&) = delete;
    Layer& operator=(Layer const&) = delete;
    Layer& operator=(Layer&&) = delete;

    virtual void wrapping_layers(std::shared_ptr<Layer>,
                                 std::shared_ptr<Layer> = nullptr,
                                 std::shared_ptr<Layer> = nullptr) noexcept = 0;
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_LAYER_H_
