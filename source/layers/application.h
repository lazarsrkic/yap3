#ifndef SOURCE_LAYERS_APPLICATION_H_
#define SOURCE_LAYERS_APPLICATION_H_

#include <memory>

namespace yap3 {
namespace layers {

class Presentation;

class Application {
public:
    void wrapping_layers(
        std::shared_ptr<Presentation> presentation_layer) noexcept;

private:
    std::shared_ptr<Presentation> m_presentation_layer;
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_APPLICATION_H_
