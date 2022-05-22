#include "utils/timer.h"

#include <gtest/gtest.h>

TEST(TIMER, AfterCreationTimerShallNotBeExpired) {
    yap3::utils::Timer test_timer{std::chrono::milliseconds{30}};

    ASSERT_FALSE(test_timer.expired());
}

TEST(TIMER, AfterResetTimerShallNotBeExpired) {
    yap3::utils::Timer test_timer{std::chrono::milliseconds{30}};

    std::this_thread::sleep_for(std::chrono::milliseconds{35});
    ASSERT_TRUE(test_timer.expired());

    test_timer.reset();
    ASSERT_FALSE(test_timer.expired());
}

TEST(TIMER, TwoTimersWithDifferentTimeoutShallNotExpireAtTheSameTime) {
    yap3::utils::Timer test_timer{std::chrono::milliseconds{30}};
    yap3::utils::Timer test_timer2{std::chrono::milliseconds{50}};

    std::this_thread::sleep_for(std::chrono::milliseconds{35});
    ASSERT_TRUE(test_timer.expired());
    ASSERT_FALSE(test_timer2.expired());
}
