#ifndef SOURCE_UTILS_TIMER_H_
#define SOURCE_UTILS_TIMER_H_

#include <spdlog/spdlog.h>

#include <chrono>

namespace yap3 {
namespace utils {

class Timer {
public:
    Timer(std::chrono::milliseconds const& timeout) noexcept;

    bool expired() const noexcept;
    void reset() noexcept;

private:
    std::chrono::time_point<std::chrono::steady_clock> m_time_point;
    std::chrono::milliseconds const m_timeout;
};

}  // namespace utils
}  // namespace yap3

#endif  // SOURCE_UTILS_TIMER_H_
