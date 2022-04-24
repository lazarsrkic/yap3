#include "utils/timer.h"

namespace yap3 {
namespace utils {

Timer::Timer(std::chrono::milliseconds const& timeout) noexcept
    : m_time_point(std::chrono::steady_clock::now()), m_timeout(timeout) {}

bool Timer::expired() const noexcept {
    std::chrono::time_point<std::chrono::steady_clock> current =
        std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        current - m_time_point);
    if (elapsed > m_timeout) {
        return true;
    }

    return false;
}

void Timer::reset() noexcept {
    m_time_point = std::chrono::steady_clock::now();
}

}  // namespace utils
}  // namespace yap3
