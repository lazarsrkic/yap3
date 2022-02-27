#ifndef SOURCE_DATALINK_BACKEND_H_
#define SOURCE_DATALINK_BACKEND_H_

#include <sys/types.h>

#include <cstddef>
#include <cstdint>

namespace yap3 {
namespace datalink {

class Backend {
public:
    virtual ssize_t read(std::uint8_t* buf, std::size_t const len) = 0;
    virtual ssize_t write(std::uint8_t const* buf, std::size_t const len) = 0;
    virtual operator bool() = 0;
};

}  // namespace datalink
}  // namespace yap3

#endif  // SOURCE_DATALINK_BACKEND_H_
