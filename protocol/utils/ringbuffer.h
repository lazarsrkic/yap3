#ifndef YAP3_PROTOCOL_UTILS_RINGBUFFER_H_
#define YAP3_PROTOCOL_UTILS_RINGBUFFER_H_

#include <spdlog/spdlog.h>

#include <atomic>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>

namespace yap3 {
namespace utils {

template <std::size_t RINGBUFFER_SIZE, std::size_t MAX_MESSAGE_SIZE>
class RingBuffer {
public:
    RingBuffer() noexcept {
        for (ringbuffer_item_type& item : buffer_) {
            std::memset(item.first.data(), 0x00, MAX_MESSAGE_SIZE);
            item.second = 0;
        }
    }

    void reset() noexcept {
        for (ringbuffer_item_type& item : buffer_) {
            std::memset(item.first.data(), 0x00, MAX_MESSAGE_SIZE);
            item.second = 0;
        }

        head_ = 0;
        tail_ = 0;
        full_ = false;
    }

    std::size_t head() const noexcept { return head_; }

    bool put(std::uint8_t const* item, std::size_t size) noexcept {
        if (full_) {
            return false;
        }

        buffer_[head_].second = size;
        std::memcpy(buffer_[head_].first.data(), item, size);
        advance_pointers();

        return true;
    }

    bool retrieve(std::size_t offset, std::uint8_t* buf,
                  std::size_t* size) const noexcept {
        if (empty()) {
            return false;
        }

        if (buf == nullptr || size == nullptr) {
            return false;
        }

        std::size_t const index = (tail_ + offset) % RINGBUFFER_SIZE;

        if (tail_ > head_) {
            if (index >= tail_ || index < head_) {
                auto const& item = buffer_[index];
                std::memcpy(buf, item.first.data(), item.second);
                *size = item.second;
                return true;
            }
        } else {
            if (index >= tail_ && (index < head_ || full_)) {
                auto const& item = buffer_[index];
                std::memcpy(buf, item.first.data(), item.second);
                *size = item.second;
                return true;
            }
        }
        return false;
    }

    bool remove(std::uint8_t* buf, std::size_t* size) {
        if (empty()) {
            return false;
        }

        if (buf == nullptr || size == nullptr) {
            return false;
        }

        auto& item = buffer_[tail_];
        std::memcpy(buf, item.first.data(), item.second);
        *size = item.second;

        std::memset(item.first.data(), 0x00, MAX_MESSAGE_SIZE);
        item.second = 0;

        tail_++;

        if (tail_ == RINGBUFFER_SIZE) {
            tail_ = 0;
        }

        full_ = false;

        return true;
    }

    bool release() noexcept {
        if (empty()) {
            return false;
        }

        std::memset(buffer_[tail_].first.data(), 0x00, MAX_MESSAGE_SIZE);
        buffer_[tail_].second = 0;

        tail_++;

        if (tail_ == RINGBUFFER_SIZE) {
            tail_ = 0;
        }

        full_ = false;
        return true;
    }

    inline bool empty() const noexcept { return !full_ && head_ == tail_; }

    inline bool full() const noexcept { return full_; }

    inline size_t capacity() const noexcept { return RINGBUFFER_SIZE; }

    size_t size() const noexcept {
        if (!full_) {
            if (head_ < tail_) {
                return head_ + RINGBUFFER_SIZE - tail_;
            } else {
                return head_ - tail_;
            }
        } else {
            return RINGBUFFER_SIZE;
        }
    }

    void print() const noexcept {
        std::stringstream stream;
        std::size_t index{0};
        for (ringbuffer_item_type const& item : buffer_) {
            auto const& local_index = index++;
            if (item.second == 0) {
                continue;
            }
            stream << "[" << local_index << "]: " << log_item(item) << "\n";
        }

        spdlog::debug("\nBuffer content:\n{}", stream.str());
    }

private:
    typedef std::pair<std::array<std::uint8_t, MAX_MESSAGE_SIZE>, std::uint8_t>
        ringbuffer_item_type;

    std::string log_item(ringbuffer_item_type const& item) const noexcept {
        std::stringstream stream;
        std::size_t index{0};
        while (index < item.second) {
            stream << "0x" << std::setfill('0') << std::setw(2) << std::hex
                   << static_cast<int>(item.first[index++]) << ' ' << std::dec;
        }

        return stream.str();
    }

    void advance_pointers() {
        if (full_) {
            if (++tail_ == RINGBUFFER_SIZE) {
                tail_ = 0;
            }
        }

        if (++head_ == RINGBUFFER_SIZE) {
            head_ = 0;
        }

        full_ = head_ == tail_;
    }

    mutable std::mutex mutex_;
    std::array<ringbuffer_item_type, RINGBUFFER_SIZE> buffer_;
    std::size_t head_ = 0;
    std::size_t tail_ = 0;
    std::atomic_bool full_ = false;
};

}  // namespace utils
}  // namespace yap3

#endif  // YAP3_PROTOCOL_UTILS_RINGBUFFER_H_
