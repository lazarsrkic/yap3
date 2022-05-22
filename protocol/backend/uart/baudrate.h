#ifndef YAP3_PROTOCOL_BACKEND_UART_BAUDRATE_H_
#define YAP3_PROTOCOL_BACKEND_UART_BAUDRATE_H_

#include <termios.h>

#include <cstdint>

namespace yap3 {
namespace backend {
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
}  // namespace backend
}  // namespace yap3

#endif  // YAP3_PROTOCOL_BACKEND_UART_BAUDRATE_H_
