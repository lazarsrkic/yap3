#ifndef SOURCE_UTILS_MQUEUE_H_
#define SOURCE_UTILS_MQUEUE_H_
#include <mqueue.h>

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace yap3 {
namespace utils {

class MQueue {
public:
    MQueue(std::string const& name,
           std::int32_t const mode = O_RDWR | O_NONBLOCK,
           std::size_t const msg_size = 5U) noexcept;
    ~MQueue() noexcept;
    MQueue(MQueue const& other) = delete;
    MQueue& operator=(MQueue const& other) = delete;

    bool send(std::vector<std::uint8_t> const& data) const noexcept;
    bool timed_send(std::vector<std::uint8_t> const& data,
                    std::chrono::milliseconds const& timeout) const noexcept;

    bool receive(std::vector<std::uint8_t>& data) const noexcept;
    bool timed_receive(std::vector<std::uint8_t>& data,
                       std::chrono::milliseconds const& timeout) const noexcept;

    inline operator bool() const noexcept { return m_fd != -1; };

private:
    mqd_t m_fd{-1};
    struct mq_attr m_attr;
    mutable std::vector<char> m_buffer;
};

}  // namespace utils
}  // namespace yap3

#endif  // SOURCE_UTILS_MQUEUE_H_
