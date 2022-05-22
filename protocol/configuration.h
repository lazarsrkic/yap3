#ifndef YAP3_PROTOCOL_CONFIGURATION_H_
#define YAP3_PROTOCOL_CONFIGURATION_H_

#include "utils/ringbuffer.h"

#include <cstdint>

namespace yap3 {
namespace configuration {

constexpr std::uint8_t FLAG_SIZE{1U};
constexpr std::uint8_t CRC_SIZE{2U};
constexpr std::uint8_t FRAME_CONTROL_SIZE{1U};
constexpr std::uint8_t APP_CONTROL_SIZE{1U};

constexpr std::uint8_t MAX_IPC_MESSAGE_PAYLOAD_SIZE{100U};

constexpr std::uint8_t MAX_RINGBUFFER_SIZE{16U};

}  // namespace configuration
}  // namespace yap3

#endif  // YAP3_PROTOCOL_CONFIGURATION_H_
