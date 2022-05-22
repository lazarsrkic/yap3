#ifndef YAP3_PROTOCOL_BACKEND_UART_BACKEND_UART_H_
#define YAP3_PROTOCOL_BACKEND_UART_BACKEND_UART_H_

#include "backend/backend.h"
#include "backend/uart/serial.h"

#include <cstdint>
#include <memory>
#include <string>

namespace yap3 {
namespace backend {

class BackendUart : public Backend {
public:
    BackendUart(std::string const& device,
                std::uint32_t const baudrate) noexcept;
    ssize_t read(std::uint8_t* buf,
                 std::size_t const len) const noexcept override;
    ssize_t write(std::uint8_t const* buf,
                  std::size_t const len) const noexcept override;
    operator bool() const noexcept override;

private:
    uart::Serial m_serial;
    bool m_valid{false};
};

}  // namespace backend
}  // namespace yap3

#endif  // YAP3_PROTOCOL_BACKEND_UART_BACKEND_UART_H_
