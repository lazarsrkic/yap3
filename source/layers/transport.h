#ifndef SOURCE_LAYERS_TRANSPORT_H_
#define SOURCE_LAYERS_TRANSPORT_H_

#include "configuration.h"
#include "utils/ringbuffer.h"
#include "utils/slidingwindow.h"
#include "utils/timer.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>

namespace yap3 {
namespace layers {

class Presentation;
class Datalink;

namespace transport {
constexpr std::uint8_t MAX_COMPLETE_MESSAGE_SIZE{
    2 * (yap3::configuration::FLAG_SIZE * 2 + yap3::configuration::CRC_SIZE +
         yap3::configuration::FRAME_CONTROL_SIZE +
         yap3::configuration::APP_CONTROL_SIZE +
         yap3::configuration::MAX_IPC_MESSAGE_PAYLOAD_SIZE)};
constexpr std::uint16_t READ_BUFFER_SIZE{MAX_COMPLETE_MESSAGE_SIZE * 5};

// it has to start with something greather than 7, in order to avoid collision
constexpr std::uint8_t START{0xA2U};
constexpr std::uint8_t STOP{0xA3U};
constexpr std::uint8_t ESCAPE{0x7DU};

constexpr std::uint8_t LAYER_START_FLAG_OFFSET{0U};
constexpr std::uint8_t LAYER_CRC_OFFSET{yap3::configuration::FLAG_SIZE};
constexpr std::uint8_t LAYER_FRAME_CONTROL_OFFSET{
    yap3::configuration::FLAG_SIZE + yap3::configuration::CRC_SIZE};
constexpr std::uint8_t LAYER_IPC_PAYLOAD_OFFSET{
    yap3::configuration::FLAG_SIZE + yap3::configuration::CRC_SIZE +
    yap3::configuration::FRAME_CONTROL_SIZE};

constexpr std::chrono::milliseconds BUS_ACTIVITY_TIMEOUT{300U};
constexpr std::chrono::milliseconds ACK_TIMEOUT{150U};
constexpr std::chrono::milliseconds KEEP_ALIVE_TIMEOUT{75U};

enum class Message : std::uint8_t { kReset, kData, kAck, kNack, kKeepAlive };
enum class ResetType : std::uint8_t { kResetRequested, kRequestReset };

constexpr std::uint8_t WINDOW_SIZE{4U};
constexpr std::uint8_t SLIDING_WINDOW_MAX_RETRIES{3U};
}  // namespace transport

class Transport {
public:
    void wrapping_layers(std::shared_ptr<Presentation> presentation_layer,
                         std::shared_ptr<Datalink> datalink_layer) noexcept;

    bool process_ipc_message(std::uint8_t const* buffer,
                             std::size_t const size) noexcept;

    void transmit() noexcept;
    void receive() noexcept;
    void protocol_reset(transport::ResetType const& reset_type =
                            transport::ResetType::kRequestReset) noexcept;

private:
    bool send_message(std::uint8_t const offset) noexcept;
    bool send_internal_message(transport::Message const& msg_type,
                               std::uint8_t const message_id = 0) noexcept;
    bool extract_messages(std::uint8_t const* buffer,
                          std::size_t const buffer_size,
                          std::uint8_t* remain_buffer,
                          std::size_t* remain_size) noexcept;
    bool process_message(std::uint8_t const* buffer,
                         std::size_t const size) noexcept;
    void process_ack_message(std::uint8_t const message_id) noexcept;

    typedef yap3::utils::RingBuffer<yap3::configuration::MAX_RINGBUFFER_SIZE,
                                    transport::MAX_COMPLETE_MESSAGE_SIZE>
        tx_ringbuffer_type;

    std::shared_ptr<Presentation> m_presentation_layer;
    std::shared_ptr<Datalink> m_datalink_layer;

    std::array<std::uint8_t, transport::MAX_COMPLETE_MESSAGE_SIZE> m_buffer;
    std::size_t m_buffer_size;

    std::mutex m_tx_ringbuffer_mutex;
    tx_ringbuffer_type m_tx_ringbuffer{};

    yap3::utils::SlidingWindow m_sliding_window{transport::WINDOW_SIZE};

    yap3::utils::Timer bus_activity_timer{transport::BUS_ACTIVITY_TIMEOUT};
    yap3::utils::Timer ack_timer{transport::ACK_TIMEOUT};
    yap3::utils::Timer keep_alive_timer{transport::KEEP_ALIVE_TIMEOUT};
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_TRANSPORT_H_
