#ifndef YAP3_PROTOCOL_LAYERS_DATALINK_H_
#define YAP3_PROTOCOL_LAYERS_DATALINK_H_

#include "backend/backend.h"

#include <cstdint>
#include <memory>

namespace yap3 {
namespace layers {

class Transport;

class Datalink {
public:
    Datalink(std::unique_ptr<yap3::backend::Backend> backend) noexcept;

    void wrapping_layers(std::shared_ptr<Transport> transport_layer) noexcept;

    ssize_t read(std::uint8_t* buffer, std::size_t const size) const noexcept;
    bool write(std::uint8_t const* const buffer,
               std::size_t const size) const noexcept;
    inline operator bool() const noexcept { return *m_backend; };

private:
    std::unique_ptr<yap3::backend::Backend> m_backend;
    std::shared_ptr<Transport> m_transport_layer;
};

}  // namespace layers
}  // namespace yap3

#endif  // YAP3_PROTOCOL_LAYERS_DATALINK_H_
