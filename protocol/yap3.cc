#include "protocol.h"

#include <spdlog/spdlog.h>

int main(int, char const *[]) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Welcome to YAP3");

    yap3::Protocol yap3{};
    return yap3.run();
}
