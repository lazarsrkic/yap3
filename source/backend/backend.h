#ifndef SOURCE_BACKEND_BACKEND_H_
#define SOURCE_BACKEND_BACKEND_H_

#include <sys/types.h>

#include <cstddef>
#include <cstdint>

namespace yap3 {
namespace backend {

class Backend {
public:
    virtual ssize_t read(std::uint8_t* buf,
                         std::size_t const len) const noexcept = 0;
    virtual ssize_t write(std::uint8_t const* buf,
                          std::size_t const len) const noexcept = 0;
    virtual operator bool() const noexcept = 0;
};

}  // namespace backend
}  // namespace yap3

#endif  // SOURCE_BACKEND_BACKEND_H_
