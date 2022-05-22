#include "layers/datalink.h"
#include "layers/transport.h"

namespace yap3 {
namespace layers {

Datalink::Datalink(std::unique_ptr<yap3::backend::Backend> backend) noexcept
    : m_backend{std::move(backend)} {}

void Datalink::wrapping_layers(
    std::shared_ptr<yap3::layers::Transport> transport_layer) noexcept {
    m_transport_layer = transport_layer;
}

ssize_t Datalink::read(std::uint8_t* buffer,
                       std::size_t const size) const noexcept {
    return m_backend->read(buffer, size);
}

bool Datalink::write(std::uint8_t const* const buffer,
                     std::size_t const size) const noexcept {
    return m_backend->write(buffer, size);
}

}  // namespace layers
}  // namespace yap3
