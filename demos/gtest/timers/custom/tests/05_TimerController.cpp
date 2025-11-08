#include <gtest/gtest.h>

#include "Timers/Lists.h"
#include "Timers/Timer.h"
#include "Timers/TimerController.h"
#include "Timers/SendCommand.h"

class TimerControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset ticks and clear queue
        Timers::resetCurrentTicks();
        Timers::Running::GetList().clear();
        Timers::EnqueuedCommands::GetList().clear();
    }
};

namespace Timers {

// Derived class to test callback calls
class TestTimer : public Timer {
public:
    int callbackCount {0};

    TestTimer(const char* name, Ticks period, bool autoReload)
        : Timer(name, period, &TestTimer::staticCallback, autoReload) 
    {}

private:
    // Static function because Timer expects a Callback pointer
    static void staticCallback(Timer* t) {
        auto* self = dynamic_cast<TestTimer*>(t);
        if (self) {
            self->callbackCount++;
        }
    }
};

}

TEST_F(TimerControllerTest, CallbackOnce) {
    Timers::TestTimer t1("T1", 100, false );
    Timers::SendCommand::Start(&t1);

    EXPECT_TRUE(Timers::Running::GetList().empty());
    EXPECT_EQ(t1.callbackCount, 0); // And not called

    Timers::TimerController();

    EXPECT_EQ(Timers::Running::GetList().size(),1);
    EXPECT_EQ(t1.callbackCount, 0); // And not called

    Timers::incrementCurrentTicks(50);
    Timers::TimerController();

    EXPECT_EQ(Timers::Running::GetList().size(),1);
    EXPECT_EQ(t1.callbackCount, 0); // And not called

    Timers::incrementCurrentTicks(50);
    Timers::TimerController();

    EXPECT_EQ(Timers::Running::GetList().size(),0); // Removed as it has not autoreload
    EXPECT_EQ(t1.callbackCount, 1); // And called once

}

TEST_F(TimerControllerTest, CallbackEvery100) {
    const int CalledTimes = 10;
    Timers::TestTimer t1("T1", 100, true );
    Timers::SendCommand::Start(&t1);

    EXPECT_TRUE(Timers::Running::GetList().empty());
    EXPECT_EQ(t1.callbackCount, 0); // And not called

    Timers::TimerController();
    EXPECT_EQ(Timers::Running::GetList().size(),1); // As the timer is enabled
    for(int i = 0; i < CalledTimes; ++i) {
        // Once time it should not be called
        Timers::incrementCurrentTicks(50);
        Timers::TimerController();
        Timers::incrementCurrentTicks(50);
        Timers::TimerController();
    }

    EXPECT_EQ(Timers::Running::GetList().size(),1); // The timer should still be there
    EXPECT_EQ(t1.callbackCount, CalledTimes);       // It was called N times

}


TEST_F(TimerControllerTest, CallbackTwoTimers) {
    const int CalledTimes = 10;
    Timers::TestTimer t1("T1", 100, true );
    Timers::TestTimer t2("T1", 200, true );
    Timers::SendCommand::Start(&t1);
    Timers::SendCommand::Start(&t2);

    EXPECT_TRUE(Timers::Running::GetList().empty());
    EXPECT_EQ(t1.callbackCount, 0); // And not called

    Timers::TimerController();
    EXPECT_EQ(Timers::Running::GetList().size(),2); 
    for(int i = 0; i < CalledTimes; ++i) {
        // Once time it should not be called
        Timers::incrementCurrentTicks(50);
        Timers::TimerController();
        Timers::incrementCurrentTicks(50);
        Timers::TimerController();
    }

    EXPECT_EQ(Timers::Running::GetList().size(),2); // The timers should still be there
    EXPECT_EQ(t1.callbackCount, CalledTimes);       // It was called N times
    EXPECT_EQ(t2.callbackCount, CalledTimes/2);     // It was called N/2 times

}
