#ifndef YAP3_PROTOCOL_UTILS_CRC16_H_
#define YAP3_PROTOCOL_UTILS_CRC16_H_

#include <cstdint>

namespace yap3 {
namespace utils {

class Crc16 {
public:
    std::uint16_t operator()(std::uint8_t const* const buffer,
                             std::uint8_t const size) const noexcept;
};

}  // namespace utils
}  // namespace yap3

#endif  // YAP3_PROTOCOL_UTILS_CRC16_H_
