#include "backend/uart/backend_uart.h"

#include <spdlog/spdlog.h>

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

namespace {
std::string log_buffer(uint8_t const* const buf, std::size_t const len) {
    std::stringstream stream{};

    for (auto i = 0U; i < len; ++i) {
        stream << "0x" << std::setfill('0') << std::setw(2) << std::hex
               << static_cast<int>(*(buf + i)) << ' ' << std::dec;
    }
    return stream.str();
}
}  // namespace

namespace yap3 {
namespace backend {

BackendUart::BackendUart(std::string const& device,
                         std::uint32_t const baudrate) noexcept
    : m_serial{device, baudrate} {
    if (!m_serial) {
        spdlog::error("Failed to create Serial object!");
        return;
    }
    spdlog::debug("Uart backend created!");
}

BackendUart::operator bool() const noexcept { return m_serial; }

ssize_t BackendUart::read(std::uint8_t* buf,
                          std::size_t const len) const noexcept {
    auto const read_len = ::read(m_serial, buf, len);
    if (read_len == -1) {
        spdlog::error("Failed to read data from serial stream! Error: {}",
                      std::strerror(errno));
    } else {
        if (read_len != 0) {
            spdlog::debug("Backend read: {}", log_buffer(buf, read_len));
        }
    }

    return read_len;
}

ssize_t BackendUart::write(std::uint8_t const* buf,
                           std::size_t const len) const noexcept {
    auto const write_len = ::write(m_serial, buf, len);
    if (write_len == -1) {
        spdlog::error("Failed to write data to serial stream! Error: {}",
                      std::strerror(errno));
    } else {
        spdlog::debug("Backend write: {}", log_buffer(buf, write_len));
    }
    return write_len;
}

}  // namespace backend
}  // namespace yap3
