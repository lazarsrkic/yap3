#include "datalink/backend.h"
#include "datalink/uart/backend_uart.h"

#include <spdlog/spdlog.h>

#include <array>
#include <chrono>
#include <memory>

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Welcome to YAP3");

    yap3::datalink::BackendUart uart1{"/dev/pts/3", 115200};
    if (!uart1) {
        spdlog::error("Failed to create Uart backend for write");
        return -1;
    }

    yap3::datalink::BackendUart uart2{"/dev/pts/2", 115200};
    if (!uart2) {
        spdlog::error("Failed to create Uart backend for read");
        return -1;
    }

    std::array<std::uint8_t, 6> msg_send2{0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5};
    auto write_len = uart2.write(msg_send2.data(), msg_send2.size());
    spdlog::info("Written {} bytes!", write_len);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::array<std::uint8_t, 6> msg_receive2{};
    auto read_len = uart1.read(msg_receive2.data(), msg_receive2.size());
    spdlog::info("Read {} bytes!", read_len);

    std::array<std::uint8_t, 6> msg_send{0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    auto write2_len = uart1.write(msg_send.data(), msg_send.size());
    spdlog::info("Written {} bytes!", write2_len);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::array<std::uint8_t, 6> msg_receive{};
    auto read2_len = uart2.read(msg_receive.data(), msg_receive.size());
    spdlog::info("Read {} bytes!", read2_len);

    return 0;
}
