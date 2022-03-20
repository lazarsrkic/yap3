#include "client/mqueue/client_mqueue.h"

#include <spdlog/spdlog.h>

#include <chrono>

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Welcome to yap3client");

    yap3::client::ClientMQueue client{"client"};
    if (!client) {
        spdlog::error("Failed to create client mqueue");
        return -1;
    }

    std::vector<std::uint8_t> msg_receive2(5);

    while (1) {
        if (!client.timed_receive(msg_receive2,
                                  std::chrono::milliseconds(100))) {
            spdlog::error("Failed to receive message from yap3");
            continue;
        }

        if (!client.timed_send(msg_receive2, std::chrono::milliseconds(100))) {
            spdlog::error("Failed to send message to yap3");
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
