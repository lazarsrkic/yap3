#ifndef SOURCE_DATALINK_UART_SERIAL_H_
#define SOURCE_DATALINK_UART_SERIAL_H_

#include <termios.h>
#include <unistd.h>

#include <cstdint>
#include <string>

namespace yap3 {
namespace datalink {
namespace uart {

class Serial {
public:
    Serial(std::string const& device, std::uint32_t const baudrate);
    ~Serial();
    Serial(Serial const& other) = delete;
    Serial& operator=(Serial const& other) = delete;

    operator bool() const;
    operator std::int32_t() const;

private:
    std::int32_t m_fd{-1};
    struct termios m_settings {};
};

}  // namespace uart
}  // namespace datalink
}  // namespace yap3

#endif  // SOURCE_DATALINK_UART_SERIAL_H_
