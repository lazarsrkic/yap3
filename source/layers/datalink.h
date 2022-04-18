#ifndef SOURCE_LAYERS_DATALINK_H_
#define SOURCE_LAYERS_DATALINK_H_

#include "backend/backend.h"

#include <sys/types.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace yap3 {
namespace layers {
namespace datalink {

class Datalink {
public:
    Datalink(std::unique_ptr<yap3::backend::Backend> backend) noexcept;
    std::pair<bool, std::vector<std::uint8_t>> read(void) const noexcept;
    ssize_t write(std::vector<std::uint8_t> const& data) const noexcept;
    inline operator bool() const noexcept { return *m_backend; };

private:
    std::unique_ptr<yap3::backend::Backend> m_backend;
};

}  // namespace datalink
}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_DATALINK_H_
