#ifndef YAP3_PROTOCOL_UTILS_SLIDINGWINDOW_H_
#define YAP3_PROTOCOL_UTILS_SLIDINGWINDOW_H_

#include <cstdint>

namespace yap3 {
namespace utils {

struct SlidingWindow {
    SlidingWindow(std::uint8_t const window_size) noexcept;
    void reset() noexcept;

    std::uint8_t last_acked;
    std::uint8_t to_be_acked;
    std::uint8_t offset_to_send;
    std::uint8_t retry_count;
    std::uint8_t const m_window_size;
};

}  // namespace utils
}  // namespace yap3

#endif  // YAP3_PROTOCOL_UTILS_SLIDINGWINDOW_H_
