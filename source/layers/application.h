#ifndef SOURCE_LAYERS_APPLICATION_H_
#define SOURCE_LAYERS_APPLICATION_H_
#include "configuration.h"

#include <array>
#include <cstdint>
#include <memory>
#include <mutex>

namespace yap3 {
namespace layers {

class Presentation;

namespace application {

constexpr std::uint8_t LAYER_IPC_MESSAGE_SENDER_RECEIVER_ID_OFFSET{0U};
constexpr std::uint8_t LAYER_IPC_MESSAGE_PAYLOAD_OFFSET{1U};

constexpr std::uint8_t LAYER_MESSAGE_APP_CONTROL_OFFSET{0U};
constexpr std::uint8_t LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET{
    yap3::configuration::APP_CONTROL_SIZE};
constexpr std::uint8_t LAYER_MESSAGE_PAYLOAD_OFFSET{
    LAYER_MESSAGE_SENDER_RECEIVER_ID_OFFSET + 1U};

constexpr std::uint8_t LAYER_MAX_PAYLOAD_SIZE{
    yap3::configuration::MAX_IPC_MSG_PAYLOAD_SIZE +
    yap3::configuration::APP_CONTROL_SIZE};
}  // namespace application

class Application {
public:
    Application() noexcept;
    void wrapping_layers(
        std::shared_ptr<Presentation> presentation_layer) noexcept;

    bool on_ipc_message_receive(std::uint8_t const* buffer,
                                std::size_t const size) noexcept;

    bool on_ipc_message_send(std::uint8_t const receiver_id,
                             std::uint8_t* buffer, std::size_t* size) noexcept;

    bool on_frame_receive(std::uint8_t const* buffer,
                          std::size_t const size) noexcept;

private:
    std::shared_ptr<Presentation> m_presentation_layer;
    std::array<std::uint8_t, application::LAYER_MAX_PAYLOAD_SIZE> m_buffer;
    std::size_t m_buffer_size;

    std::mutex m_rx_ringbuffers_mutex;
    std::array<yap3::configuration::rx_ringbuffer_type, 16> m_rx_ringbuffers{};
};

}  // namespace layers
}  // namespace yap3

#endif  // SOURCE_LAYERS_APPLICATION_H_
