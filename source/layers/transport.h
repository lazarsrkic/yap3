#ifndef SOURCE_LAYERS_TRANSPORT_H_
#define SOURCE_LAYERS_TRANSPORT_H_

#include <memory>

namespace yap3 {
namespace layers {

class Application;
class Presentation;
class Datalink;

class Transport {
public:
    void wrapping_layers(std::shared_ptr<Application> application_layer,
                         std::shared_ptr<Presentation> presentation_layer,
                         std::shared_ptr<Datalink> datalink_layer) noexcept;

private:
    std::shared_ptr<Application> m_application_layer;
    std::shared_ptr<Presentation> m_presentation_layer;
    std::shared_ptr<Datalink> m_datalink_layer;
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_TRANSPORT_H_
