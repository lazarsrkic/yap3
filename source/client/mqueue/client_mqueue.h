#ifndef SOURCE_CLIENT_MQUEUE_CLIENT_MQUEUE_H_
#define SOURCE_CLIENT_MQUEUE_CLIENT_MQUEUE_H_
#include "client/client.h"
#include "utils/mqueue.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace yap3 {
namespace client {

class ClientMQueue : public Client {
public:
    ClientMQueue(std::string const& mqname);
    bool timed_send(
        std::vector<std::uint8_t> const& data,
        std::chrono::milliseconds const& timeout) const noexcept override;
    bool timed_receive(
        std::vector<std::uint8_t>& data,
        std::chrono::milliseconds const& timeout) const noexcept override;

    inline operator bool() const noexcept { return m_send && m_receive; }

private:
    yap3::utils::MQueue m_send;
    yap3::utils::MQueue m_receive;
};

}  // namespace client
}  // namespace yap3

#endif  // SOURCE_CLIENT_MQUEUE_CLIENT_MQUEUE_H_
