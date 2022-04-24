#ifndef SOURCE_LAYERS_PRESENTATION_H_
#define SOURCE_LAYERS_PRESENTATION_H_

#include "layers/layer.h"

#include <cstdint>
#include <utility>
namespace yap3 {
namespace layers {
namespace presentation {
enum class EncryptType : std::uint8_t { kNone, kTypeA, kTypeB };
}  // namespace presentation

class Presentation : public Layer {
public:
    Presentation(presentation::EncryptType const& type =
                     presentation::EncryptType::kNone){};

    void wrapping_layers(std::shared_ptr<Layer> application_layer,
                         std::shared_ptr<Layer> transport_layer,
                         std::shared_ptr<Layer>) noexcept override;
    bool encrypt(std::int8_t const* buffer, std::uint8_t const size) noexcept;
    bool decrypt(std::int8_t const* buffer, std::uint8_t const size) noexcept;

private:
    presentation::EncryptType encypt_type_;
    std::shared_ptr<Layer> m_application_layer{nullptr};
    std::shared_ptr<Layer> m_transport_layer{nullptr};
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_PRESENTATION_H_
