#include <gtest/gtest.h>

#include "Timers/Timer.h"

#include <cstdlib>
#include <ctime>

// Simple test fixture for Timers namespace
class TimersTest : public ::testing::Test {
protected:
    void SetUp() override {
        Timers::resetCurrentTicks(); // start from zero before each test
    }
};

TEST_F(TimersTest, ResetSetsTicksToZero) {
    Timers::incrementCurrentTicks(5);
    EXPECT_GT(Timers::getCurrentTicks(), 0u);

    Timers::resetCurrentTicks();
    EXPECT_EQ(Timers::getCurrentTicks(), 0u);
}

TEST_F(TimersTest, IncrementAddsCorrectly) {
    EXPECT_EQ(Timers::getCurrentTicks(), 0u);

    Timers::incrementCurrentTicks(3);
    EXPECT_EQ(Timers::getCurrentTicks(), 3u);

    Timers::incrementCurrentTicks(7);
    EXPECT_EQ(Timers::getCurrentTicks(), 10u);
}

TEST_F(TimersTest, MultipleIncrementsAndReset) {
    Timers::incrementCurrentTicks(1);
    Timers::incrementCurrentTicks(2);
    Timers::incrementCurrentTicks(3);
    EXPECT_EQ(Timers::getCurrentTicks(), 6u);

    Timers::resetCurrentTicks();
    EXPECT_EQ(Timers::getCurrentTicks(), 0u);
}

TEST_F(TimersTest, RandomIncrementsAccumulateCorrectly) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    unsigned int expected = 0;
    const int N = 100;

    for (int i = 0; i < N; ++i) {
        unsigned int add = std::rand() % 10;
        Timers::incrementCurrentTicks(add);
        expected += add;
    }

    EXPECT_EQ(Timers::getCurrentTicks(), expected);
}