#include "utils/mqueue.h"

#include <spdlog/spdlog.h>

#include <iomanip>
#include <sstream>

namespace {
const ::timespec ms_to_timespec(std::chrono::milliseconds const& timeout) {
    auto const now = std::chrono::system_clock::now();
    std::chrono::nanoseconds const time_to_wait =
        now.time_since_epoch() + timeout;
    auto const secs =
        std::chrono::duration_cast<std::chrono::seconds>(time_to_wait).count();
    auto const nsecs = std::chrono::duration_cast<std::chrono::nanoseconds>(
                           time_to_wait % std::chrono::seconds(1))
                           .count();

    return timespec{secs, nsecs};
}

std::string log_message(std::vector<std::uint8_t> const& msg) {
    std::stringstream stream;
    for (auto& item : msg) {
        stream << "0x" << std::setfill('0') << std::setw(2) << std::hex
               << static_cast<int>(item) << ' ' << std::dec;
    }

    return stream.str();
}
}  // namespace

namespace yap3 {
namespace utils {

MQueue::MQueue(std::string const& name, std::int32_t const mode,
               std::size_t const msg_size) noexcept
    : m_buffer(msg_size) {
    m_attr.mq_msgsize = msg_size;
    m_attr.mq_maxmsg = 10L;
    m_fd = ::mq_open(std::string("/" + name).c_str(), mode,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, &m_attr);
    if (m_fd == -1) {
        spdlog::error("Failed to open mqueue handle! Error: {}",
                      std::strerror(errno));
        return;
    }
}

MQueue::~MQueue() noexcept {
    if (m_fd != -1) {
        if (::mq_close(m_fd)) {
            spdlog::error("Failed to close mqueue handle! Error: {}",
                          std::strerror(errno));
        }
    }
}

bool MQueue::send(std::vector<std::uint8_t> const& data) const noexcept {
    if (m_fd != -1) {
        if (::mq_send(m_fd, reinterpret_cast<const char*>(data.data()),
                      m_attr.mq_msgsize, 0) == 0) {
            spdlog::debug("{}: sent message {}!", __func__, log_message(data));
            return true;
        } else {
            spdlog::error("Failed to send msg to mqueue! Error: {}",
                          std::strerror(errno));
            return false;
        }
    }
    spdlog::error("{}: invalid mqueue file descriptor!", __func__);
    return false;
}

bool MQueue::timed_send(
    std::vector<std::uint8_t> const& data,
    std::chrono::milliseconds const& timeout) const noexcept {
    if (m_fd != -1) {
        auto const& time_point = ms_to_timespec(timeout);
        if (::mq_timedsend(m_fd, reinterpret_cast<const char*>(data.data()),
                           m_attr.mq_msgsize, 0, &time_point) == 0) {
            spdlog::debug("{}: sent message {}!", __func__, log_message(data));
            return true;
        } else {
            spdlog::error("Failed to send msg to mqueue! Error: {}",
                          std::strerror(errno));
            return false;
        }
    }
    spdlog::error("{}: invalid mqueue file descriptor!", __func__);
    return false;
}

bool MQueue::receive(std::vector<std::uint8_t>& data) const noexcept {
    if (m_fd != -1) {
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
        auto const& received_msg_size =
            ::mq_receive(m_fd, m_buffer.data(), m_attr.mq_msgsize, nullptr);

        if (received_msg_size == -1) {
            spdlog::error("{} failed! Error: {}", __func__,
                          std::strerror(errno));
            return false;
        }

        data.erase(data.begin(), data.end());
        std::copy(m_buffer.begin(), m_buffer.begin() + received_msg_size,
                  std::back_inserter(data));
        spdlog::debug("{}: received message {}!", __func__, log_message(data));
        return true;
    }
    spdlog::error("{}: invalid mqueue file descriptor!", __func__);
    return false;
}

bool MQueue::timed_receive(
    std::vector<std::uint8_t>& data,
    std::chrono::milliseconds const& timeout) const noexcept {
    if (m_fd != -1) {
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
        auto const& time_point = ms_to_timespec(timeout);
        auto const& received_msg_size = ::mq_timedreceive(
            m_fd, m_buffer.data(), m_attr.mq_msgsize, nullptr, &time_point);

        if (received_msg_size == -1) {
            spdlog::error("{} failed! Error: {}", __func__,
                          std::strerror(errno));
            return false;
        }
        data.erase(data.begin(), data.end());
        std::copy(m_buffer.begin(), m_buffer.begin() + received_msg_size,
                  std::back_inserter(data));
        spdlog::debug("{}: received message {}!", __func__, log_message(data));
        return true;
    }
    spdlog::error("{}: invalid mqueue file descriptor!", __func__);
    return false;
}

}  // namespace utils
}  // namespace yap3
