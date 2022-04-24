#ifndef SOURCE_PROTOCOL_H_
#define SOURCE_PROTOCOL_H_

#include "layers/layer.h"

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

    std::shared_ptr<layers::Layer> application_layer;
    std::shared_ptr<layers::Layer> presentation_layer;
    std::shared_ptr<layers::Layer> transport_layer;
    std::shared_ptr<layers::Layer> datalink_layer;
    std::chrono::milliseconds cycle_time;
};

}  // namespace yap3

#endif  // SOURCE_PROTOCOL_H_
