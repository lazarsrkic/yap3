#include "datalink/uart/backend_uart.h"
#include "utils/mqueue.h"

#include <spdlog/spdlog.h>

#include <array>
#include <chrono>
#include <memory>

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Welcome to YAP3");

    yap3::datalink::BackendUart uart{"/dev/pts/5", 115200};
    if (!uart) {
        spdlog::error("Failed to create Uart backend for write");
        return -1;
    }

    yap3::utils::MQueue yap3{"yap", O_RDWR | O_NONBLOCK | O_CREAT};
    if (!yap3) {
        spdlog::error("Failed to create yap mqueue");
        return -1;
    }

    yap3::utils::MQueue client{"client", O_RDWR | O_NONBLOCK | O_CREAT};
    if (!client) {
        spdlog::error("Failed to create client mqueue");
        return -1;
    }

    std::vector<std::uint8_t> receive(5);

    while (1) {
        if (uart.read(receive.data(), receive.size()) == -1) {
            continue;
        };

        if (!client.timed_send(receive, std::chrono::milliseconds(100))) {
            spdlog::error("Failed to send message to client");
        }

        if (!yap3.timed_receive(receive, std::chrono::milliseconds(100))) {
            spdlog::error("Failed to receive message from yap");
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
