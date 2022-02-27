#ifndef SOURCE_DATALINK_UART_BAUDRATE_H_
#define SOURCE_DATALINK_UART_BAUDRATE_H_

#include <termios.h>

#include <cstdint>

namespace yap3 {
namespace datalink {
namespace uart {

class Baudrate {
public:
    Baudrate(std::uint32_t const baudrate);
    operator bool() const;
    operator speed_t() const;

private:
    speed_t m_baudrate{B0};
};

}  // namespace uart
}  // namespace datalink
}  // namespace yap3

#endif  // SOURCE_DATALINK_UART_BAUDRATE_H_
