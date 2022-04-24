#ifndef SOURCE_LAYERS_PRESENTATION_H_
#define SOURCE_LAYERS_PRESENTATION_H_

#include <cstdint>
#include <memory>
namespace yap3 {
namespace layers {

class Application;
class Transport;
namespace presentation {
enum class EncryptType : std::uint8_t { kNone, kTypeA, kTypeB };
}  // namespace presentation

class Presentation {
public:
    Presentation(presentation::EncryptType const& type =
                     presentation::EncryptType::kNone){};

    void wrapping_layers(std::shared_ptr<Application> application_layer,
                         std::shared_ptr<Transport> transport_layer) noexcept;

    bool encrypt(std::int8_t const* buffer, std::uint8_t const size) noexcept;
    bool decrypt(std::int8_t const* buffer, std::uint8_t const size) noexcept;

private:
    presentation::EncryptType encypt_type_;
    std::shared_ptr<Application> m_application_layer;
    std::shared_ptr<Transport> m_transport_layer;
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_PRESENTATION_H_
