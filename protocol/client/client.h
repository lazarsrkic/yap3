#ifndef YAP3_PROTOCOL_CLIENT_CLIENT_H_
#define YAP3_PROTOCOL_CLIENT_CLIENT_H_

#include <chrono>
#include <cstdint>
#include <vector>

namespace yap3 {
namespace client {

class Client {
protected:
    virtual bool timed_send(std::vector<std::uint8_t> const&,
                            std::chrono::milliseconds const&) const noexcept {
        return false;
    };
    virtual bool send(std::vector<std::uint8_t> const&) const noexcept {
        return false;
    };
    virtual bool timed_receive(
        std::vector<std::uint8_t>&,
        std::chrono::milliseconds const&) const noexcept {
        return false;
    };
    virtual bool receive(std::vector<std::uint8_t>&,
                         std::chrono::milliseconds const&) const noexcept {
        return false;
    };
};

}  // namespace client
}  // namespace yap3

#endif  // YAP3_PROTOCOL_CLIENT_CLIENT_H_
