#include "utils/slidingwindow.h"

namespace yap3 {
namespace utils {

SlidingWindow::SlidingWindow(std::uint8_t const window_size) noexcept
    : m_window_size(window_size) {
    last_acked = m_window_size - 1;
    to_be_acked = 0;
    offset_to_send = 0;
    retry_count = 0;
}

void SlidingWindow::reset() noexcept {
    last_acked = m_window_size - 1;
    to_be_acked = 0;
    offset_to_send = 0;
    retry_count = 0;
}

}  // namespace utils
}  // namespace yap3
