#ifndef SOURCE_CONFIGURATION_H_
#define SOURCE_CONFIGURATION_H_

#include "utils/ringbuffer.h"

#include <cstdint>

namespace yap3 {
namespace configuration {

constexpr std::uint8_t FLAG_SIZE{1U};
constexpr std::uint8_t CRC_SIZE{2U};
constexpr std::uint8_t FRAME_CONTROL_SIZE{1U};
constexpr std::uint8_t APP_CONTROL_SIZE{1U};

constexpr std::uint8_t MAX_IPC_MSG_PAYLOAD_SIZE{100U};

constexpr std::uint8_t MAX_RINGBUFFER_SIZE{10U};

typedef yap3::utils::RingBuffer<MAX_RINGBUFFER_SIZE, MAX_IPC_MSG_PAYLOAD_SIZE>
    rx_ringbuffer_type;

}  // namespace configuration
}  // namespace yap3

#endif  // SOURCE_CONFIGURATION_H_
