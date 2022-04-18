#ifndef SOURCE_BACKEND_UART_SERIAL_H_
#define SOURCE_BACKEND_UART_SERIAL_H_

#include <termios.h>
#include <unistd.h>

#include <cstdint>
#include <string>

namespace yap3 {
namespace backend {
namespace uart {

class Serial {
public:
    Serial(std::string const& device, std::uint32_t const baudrate) noexcept;
    ~Serial() noexcept;
    Serial(Serial const& other) = delete;
    Serial& operator=(Serial const& other) = delete;

    inline operator bool() const noexcept { return m_fd != -1; }
    inline operator std::int32_t() const noexcept { return m_fd; }

private:
    std::int32_t m_fd{-1};
    struct termios m_settings {};
};

}  // namespace uart
}  // namespace backend
}  // namespace yap3

#endif  // SOURCE_BACKEND_UART_SERIAL_H_
