#include "backend/uart/backend_uart.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iterator>
#include <memory>
#include <random>

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Welcome to yap3peer");

    yap3::backend::BackendUart uart{"/dev/pts/4", 115200};
    if (!uart) {
        spdlog::error("Failed to create Uart backend for read");
        return -1;
    }

    std::vector<std::uint8_t> msg{0x01, 0x02, 0x03, 0x04, 0x05};
    while (1) {
        std::random_shuffle(msg.begin(), msg.end());
        if (uart.write(msg.data(), msg.size()) == -1) {
            spdlog::error("Failed to send msg over uart!");
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
