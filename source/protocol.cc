#include "backend/uart/backend_uart.h"
#include "protocol.h"

#include <thread>

namespace yap3 {

Protocol::Protocol() noexcept
    : application_layer(std::make_shared<layers::Application>()),
      presentation_layer(std::make_shared<layers::Presentation>()),
      transport_layer(std::make_shared<layers::Transport>()),
      datalink_layer(std::make_shared<layers::Datalink>(
          std::make_unique<backend::BackendUart>("/dev/pts/5", 115200))),
      cycle_time{50} {
    application_layer->wrapping_layers(presentation_layer);
    presentation_layer->wrapping_layers(application_layer, transport_layer);
    transport_layer->wrapping_layers(application_layer, presentation_layer,
                                     datalink_layer);
    datalink_layer->wrapping_layers(transport_layer);
}

void Protocol::transmit() const noexcept {}

void Protocol::receive() const noexcept {}

std::int32_t Protocol::run() const noexcept {
    while (1) {
        transmit();
        receive();
        std::this_thread::sleep_for(cycle_time);
    }

    return 0;
}

}  // namespace yap3
