#include "utils/crc16.h"

#include <gtest/gtest.h>

#include <array>

// online tool for calculation
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html Custom CRC
// Paramterization Polynomial: 0x8005 Init value: 0x0000 Final XOR Value: 0x0000
TEST(CRC16, TestVariousInputs) {
    yap3::utils::Crc16 crc16_calculator{};

    std::array<std::uint8_t, 9> test_data{0x31, 0x32, 0x33, 0x34, 0x35,
                                          0x36, 0x37, 0x38, 0x39};
    ASSERT_EQ(crc16_calculator(test_data.data(), test_data.size()), 0xFEE8);

    std::array<std::uint8_t, 9> test_data2{0x01, 0x02, 0x03, 0x04, 0x05,
                                           0x06, 0x07, 0x08, 0x09};
    ASSERT_EQ(crc16_calculator(test_data2.data(), test_data2.size()), 0xC9B);

    std::array<std::uint8_t, 5> test_data3{0x01, 0x02, 0x03, 0x04, 0x05};
    ASSERT_EQ(crc16_calculator(test_data3.data(), test_data3.size()), 0xB059);

    ASSERT_EQ(crc16_calculator(test_data.data(), 0), 0x0);
    ASSERT_EQ(crc16_calculator(nullptr, 123), 0x0);
}
