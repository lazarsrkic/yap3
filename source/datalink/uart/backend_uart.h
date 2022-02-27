#ifndef SOURCE_DATALINK_UART_BACKEND_UART_H_
#define SOURCE_DATALINK_UART_BACKEND_UART_H_

#include "datalink/backend.h"
#include "datalink/uart/serial.h"

#include <cstdint>
#include <memory>
#include <string>

namespace yap3 {
namespace datalink {

class BackendUart : public Backend {
public:
    BackendUart(std::string const& device, std::uint32_t const baudrate);
    ssize_t read(std::uint8_t* buf, std::size_t const len) override;
    ssize_t write(std::uint8_t const* buf, std::size_t const len) override;
    operator bool() override;

private:
    uart::Serial m_serial;
    bool m_valid{false};
};

}  // namespace datalink
}  // namespace yap3

#endif  // SOURCE_DATALINK_UART_BACKEND_UART_H_
