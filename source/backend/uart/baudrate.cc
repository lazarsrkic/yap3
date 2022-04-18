#include "backend/uart/baudrate.h"

#include <spdlog/spdlog.h>

#include <memory>

namespace yap3 {
namespace backend {
namespace uart {

Baudrate::Baudrate(std::uint32_t const baudrate) {
    switch (baudrate) {
        case 4800U:
            m_baudrate = B4800;
            break;
        case 9600U:
            m_baudrate = B9600;
            break;
        case 19200U:
            m_baudrate = B19200;
            break;
        case 38400U:
            m_baudrate = B38400;
            break;
        case 57600U:
            m_baudrate = B57600;
            break;
        case 115200U:
            m_baudrate = B115200;
            break;
        case 230400U:
            m_baudrate = B230400;
            break;
        default:
            spdlog::error("Unsupported baudrate {}!", baudrate);
    }
}

Baudrate::operator bool() const { return m_baudrate != B0; }

Baudrate::operator speed_t() const { return m_baudrate; }

}  // namespace uart
}  // namespace backend
}  // namespace yap3
