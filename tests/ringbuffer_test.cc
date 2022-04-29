#include "utils/ringbuffer.h"

#include <gtest/gtest.h>

TEST(RINGBUFFER, OnCreateBufferIsEmptyAndNotFull) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};

    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.capacity(), 3);
    ASSERT_EQ(ringbuffer.size(), 0);
}

TEST(RINGBUFFER, AfterAddingElementBufferIsNotEmptyNorFUll) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};
        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);
    ringbuffer.print();
}

TEST(RINGBUFFER, AfterResetQueueIsEmpty) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};
        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);
    ringbuffer.print();

    ringbuffer.reset();
    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 0);
    ringbuffer.print();
}

TEST(RINGBUFFER, SizeIncreasesWithAddingElementsUntilBufferIsFull) {
    yap3::utils::RingBuffer<2, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);
    ringbuffer.print();
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    }
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);
    ringbuffer.print();
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};

        ASSERT_FALSE(ringbuffer.put(msg1.data(), msg1.size()));
    }
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);
    ringbuffer.print();
}

TEST(RINGBUFFER, RingBufferOfSize3IsFullAfterAdding3Elements) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};
        std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};
        std::array<std::uint8_t, 4> msg3{0x02, 0x02, 0x02, 0x02};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
        ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
        ASSERT_TRUE(ringbuffer.put(msg3.data(), msg3.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);
    ringbuffer.print();
}

TEST(RINGBUFFER, OnRingBufferFullDontAddNewElement) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x00, 0x01, 0x02, 0x03};
        std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};
        std::array<std::uint8_t, 4> msg3{0x02, 0x02, 0x02, 0x02};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
        ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
        ASSERT_TRUE(ringbuffer.put(msg3.data(), msg3.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);
    ringbuffer.print();
    {
        std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};

        ASSERT_FALSE(ringbuffer.put(msg1.data(), msg1.size()));
    }
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);
    ringbuffer.print();
}

TEST(RINGBUFFER, ReleaseOnEmptyQueueReturnsFalse) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};

    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 0);

    ASSERT_FALSE(ringbuffer.release());
}

TEST(RINGBUFFER, ElementReleaseDecrementsSize) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};
        std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
        ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);
    ringbuffer.print();

    ASSERT_TRUE(ringbuffer.release());
    ASSERT_EQ(ringbuffer.size(), 1);
    ringbuffer.print();
}

TEST(RINGBUFFER, AfterReleaseOnFullBufferItIsNotFullAnymore) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};
        std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};
        std::array<std::uint8_t, 4> msg3{0x02, 0x02, 0x02, 0x02};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
        ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
        ASSERT_TRUE(ringbuffer.put(msg3.data(), msg3.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);
    ringbuffer.print();

    ASSERT_TRUE(ringbuffer.release());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);
    ringbuffer.print();
}

TEST(RINGBUFFER, ReleaseOneByOneUntilEmpty) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};
        std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};
        std::array<std::uint8_t, 4> msg3{0x02, 0x02, 0x02, 0x02};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
        ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
        ASSERT_TRUE(ringbuffer.put(msg3.data(), msg3.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);
    ringbuffer.print();

    ASSERT_TRUE(ringbuffer.release());
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);
    ringbuffer.print();

    ASSERT_TRUE(ringbuffer.release());
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);
    ringbuffer.print();

    ASSERT_TRUE(ringbuffer.release());
    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 0);
    ringbuffer.print();
}

TEST(RINGBUFFER, RetriveFromEmptyRingbufferShallFail) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};

    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 0);

    ASSERT_FALSE(ringbuffer.retrieve(1, nullptr, nullptr));
}

TEST(RINGBUFFER, RetriveShallFailIfBufferOrSizeIsNull) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);

    ASSERT_FALSE(ringbuffer.retrieve(1, nullptr, nullptr));

    std::uint8_t test_buffer[8] = {0};
    ASSERT_FALSE(ringbuffer.retrieve(1, test_buffer, nullptr));
}

TEST(RINGBUFFER, RetriveDoesntRemoveElementFromRingbuffer) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};
    std::array<std::uint8_t, 4> msg2{0x02, 0x02, 0x02, 0x02};

    ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);

    std::uint8_t test_buffer[8] = {0};
    std::size_t size{0};
    std::size_t offset{1};

    ASSERT_TRUE(ringbuffer.retrieve(offset, test_buffer, &size));
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 2);
    ASSERT_EQ(size, msg2.size());

    std::size_t index{0};

    auto msg2iterator = msg2.begin();

    while (index < size) {
        ASSERT_EQ(test_buffer[index], *(msg2iterator + index));
        index++;
    }
}

TEST(RINGBUFFER, IfOffsetIsZeroRetriveShallReturnTail) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};
    {
        std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};
        std::array<std::uint8_t, 4> msg3{0x02, 0x02, 0x02, 0x02};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
        ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
        ASSERT_TRUE(ringbuffer.put(msg3.data(), msg3.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);

    std::uint8_t test_buffer[8] = {0};
    std::size_t size{0};
    std::size_t offset{0};

    ASSERT_TRUE(ringbuffer.retrieve(offset, test_buffer, &size));
    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);
    ASSERT_EQ(size, msg1.size());

    std::size_t index{0};

    auto msg1iterator = msg1.begin();

    while (index < size) {
        ASSERT_EQ(test_buffer[index], *(msg1iterator + index));
        index++;
    }
}

TEST(RINGBUFFER, AfterReleaseAndOffsetZeroRetriveShallShiftAsWell) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};

    std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};
    std::array<std::uint8_t, 4> msg2{0x03, 0x02, 0x01, 0x00};
    std::array<std::uint8_t, 4> msg3{0x02, 0x02, 0x02, 0x02};

    ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    ASSERT_TRUE(ringbuffer.put(msg2.data(), msg2.size()));
    ASSERT_TRUE(ringbuffer.put(msg3.data(), msg3.size()));

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_TRUE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 3);

    std::uint8_t test_buffer[8] = {0};
    std::size_t size{0};
    std::size_t offset{0};

    {
        ASSERT_TRUE(ringbuffer.release());
        ASSERT_TRUE(ringbuffer.retrieve(offset, test_buffer, &size));
        ASSERT_FALSE(ringbuffer.empty());
        ASSERT_FALSE(ringbuffer.full());
        ASSERT_EQ(ringbuffer.size(), 2);
        ASSERT_EQ(size, msg2.size());

        std::size_t index{0};

        auto msg2iterator = msg2.begin();

        while (index < size) {
            ASSERT_EQ(test_buffer[index], *(msg2iterator + index));
            index++;
        }
    }

    {
        ASSERT_TRUE(ringbuffer.release());
        ASSERT_TRUE(ringbuffer.retrieve(offset, test_buffer, &size));
        ASSERT_FALSE(ringbuffer.empty());
        ASSERT_FALSE(ringbuffer.full());
        ASSERT_EQ(ringbuffer.size(), 1);
        ASSERT_EQ(size, msg3.size());

        std::size_t index{0};

        auto msg3iterator = msg3.begin();

        while (index < size) {
            ASSERT_EQ(test_buffer[index], *(msg3iterator + index));
            index++;
        }
    }
}

TEST(RINGBUFFER, RemoveFromEmptyRingbufferShallFail) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};

    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 0);

    std::uint8_t test_buffer[8] = {0};
    std::size_t size{0};
    ASSERT_FALSE(ringbuffer.remove(test_buffer, &size));
}

TEST(RINGBUFFER, RemoveShallFailIfBufferOrSizeIsNull) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    {
        std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};

        ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));
    }

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);

    ASSERT_FALSE(ringbuffer.remove(nullptr, nullptr));

    std::uint8_t test_buffer[8] = {0};
    ASSERT_FALSE(ringbuffer.remove(test_buffer, nullptr));
}

TEST(RINGBUFFER, RemoveReturnsTailAndRemoveThatItemFromRingBuffer) {
    yap3::utils::RingBuffer<3, 4> ringbuffer{};
    std::array<std::uint8_t, 4> msg1{0x01, 0x01, 0x01, 0x01};

    ASSERT_TRUE(ringbuffer.put(msg1.data(), msg1.size()));

    ASSERT_FALSE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 1);

    std::uint8_t test_buffer[8] = {0};
    std::size_t size{0};

    ASSERT_TRUE(ringbuffer.remove(test_buffer, &size));
    ASSERT_TRUE(ringbuffer.empty());
    ASSERT_FALSE(ringbuffer.full());
    ASSERT_EQ(ringbuffer.size(), 0);

    std::size_t index{0};

    auto msg1iterator = msg1.begin();

    while (index < size) {
        ASSERT_EQ(test_buffer[index], *(msg1iterator + index));
        index++;
    }
}
