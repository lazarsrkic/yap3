#ifndef YAP3_PROTOCOL_PROTOCOL_H_
#define YAP3_PROTOCOL_PROTOCOL_H_

#include "layers/application.h"
#include "layers/datalink.h"
#include "layers/presentation.h"
#include "layers/transport.h"

#include <chrono>
#include <cstdint>
#include <memory>

namespace yap3 {

class Protocol {
public:
    Protocol() noexcept;
    std::int32_t run() const noexcept;

private:
    void transmit() const noexcept;
    void receive() const noexcept;

    std::shared_ptr<layers::Application> application_layer;
    std::shared_ptr<layers::Presentation> presentation_layer;
    std::shared_ptr<layers::Transport> transport_layer;
    std::shared_ptr<layers::Datalink> datalink_layer;
    std::chrono::milliseconds cycle_time;
};

}  // namespace yap3

#endif  // YAP3_PROTOCOL_PROTOCOL_H_
