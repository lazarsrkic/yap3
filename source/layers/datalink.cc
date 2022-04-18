#include "layers/datalink.h"

namespace yap3 {
namespace layers {
namespace datalink {

Datalink::Datalink(std::unique_ptr<yap3::backend::Backend> backend) noexcept
    : m_backend{std::move(backend)} {}

std::pair<bool, std::vector<std::uint8_t>> Datalink::read(void) const noexcept {
    return {false, {}};
}

ssize_t Datalink::write(std::vector<std::uint8_t> const& data) const noexcept {
    return m_backend->write(data.data(), data.size());
}

}  // namespace datalink
}  // namespace layers
}  // namespace yap3
