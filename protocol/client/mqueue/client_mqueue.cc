#include "client/mqueue/client_mqueue.h"

namespace yap3 {
namespace client {

ClientMQueue::ClientMQueue(std::string const& mqname)
    : m_send{"yap", O_WRONLY | O_NONBLOCK},
      m_receive{mqname, O_RDONLY | O_NONBLOCK} {}

bool ClientMQueue::timed_send(
    std::vector<std::uint8_t> const& data,
    std::chrono::milliseconds const& timeout) const noexcept {
    if (m_send) {
        return m_send.timed_send(data, timeout);
    }
    return false;
}

bool ClientMQueue::timed_receive(
    std::vector<std::uint8_t>& data,
    std::chrono::milliseconds const& timeout) const noexcept {
    if (m_receive) {
        return m_receive.timed_receive(data, timeout);
    }
    return false;
}

}  // namespace client
}  // namespace yap3
