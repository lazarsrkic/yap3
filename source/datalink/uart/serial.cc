#include "datalink/uart/baudrate.h"
#include "datalink/uart/serial.h"

#include <spdlog/spdlog.h>

#include <memory>

namespace yap3 {
namespace datalink {
namespace uart {

Serial::Serial(std::string const& device,
               std::uint32_t const baudrate) noexcept {
    Baudrate bdrate{baudrate};
    if (!bdrate) {
        spdlog::error("Failed to create Baudrate object!");
        return;
    }

    m_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (m_fd == -1) {
        spdlog::error("Failed to open device {}! Error: {}", device,
                      std::strerror(errno));
        return;
    }

    if (tcgetattr(m_fd, &m_settings)) {
        spdlog::error("Failed to get serial settings! Error: {}", device,
                      std::strerror(errno));
    }

    cfmakeraw(&m_settings);

    m_settings.c_cc[VMIN] = 0;
    m_settings.c_cc[VTIME] = 0;

    if (cfsetspeed(&m_settings, bdrate)) {
        spdlog::error("Failed to set serial i/o speed! Error: {}", device,
                      std::strerror(errno));
    }

    if (tcsetattr(m_fd, TCSANOW, &m_settings)) {
        spdlog::error("Failed to set serial settings! Error: {}", device,
                      std::strerror(errno));
    }

    if (tcflush(m_fd, TCIOFLUSH)) {
        spdlog::error("Failed to flush serial i/o buffers! Error: {}", device,
                      std::strerror(errno));
    }
}

Serial::~Serial() noexcept {
    if (m_fd != -1) {
        if (::close(m_fd) != -1) {
            spdlog::debug("Closed uart stream!");
        } else {
            spdlog::error("Failed to close uart stream! Error: {}",
                          std::strerror(errno));
        }
    }
}

Serial::operator bool() const noexcept { return m_fd != -1; }

Serial::operator std::int32_t() const noexcept { return m_fd; }

}  // namespace uart
}  // namespace datalink
}  // namespace yap3
