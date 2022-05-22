#include "backend/uart/backend_uart.h"
#include "protocol.h"

#include <thread>

namespace yap3 {

Protocol::Protocol() noexcept
    : application_layer(std::make_shared<layers::Application>()),
      presentation_layer(std::make_shared<layers::Presentation>()),
      transport_layer(std::make_shared<layers::Transport>()),
      datalink_layer(std::make_shared<layers::Datalink>(
          std::make_unique<backend::BackendUart>("/dev/pts/4", 115200))),
      cycle_time{50} {
    application_layer->wrapping_layers(presentation_layer);
    presentation_layer->wrapping_layers(application_layer, transport_layer);
    transport_layer->wrapping_layers(presentation_layer, datalink_layer);
    datalink_layer->wrapping_layers(transport_layer);
}

void Protocol::transmit() const noexcept {
    std::array<std::uint8_t, 5> data{0x01, 0x02, 0x03, 0x04, 0x05};

    application_layer->on_ipc_message_receive(data.data(), data.size());
    transport_layer->transmit();
}

void Protocol::receive() const noexcept {
    transport_layer->receive();
    std::array<std::uint8_t, 10> data{};
    std::size_t size = 0;
    application_layer->on_ipc_message_send(0x1, data.data(), &size);
}

std::int32_t Protocol::run() const noexcept {
    transport_layer->protocol_reset();

    while (1) {
        transmit();
        receive();
        std::this_thread::sleep_for(cycle_time);
    }

    return 0;
}

}  // namespace yap3
