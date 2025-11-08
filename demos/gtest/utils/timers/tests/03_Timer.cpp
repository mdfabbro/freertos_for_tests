#include <gtest/gtest.h>

#include "Timers/Timer.h"

namespace Timers {

// Derived class to test callback calls
class TestTimer : public Timer {
public:
    int callbackCount {0};

    TestTimer(const char* name, Ticks period, bool autoReload)
        : Timer(name, period, &TestTimer::staticCallback, autoReload) 
    {}

    // Public method to simulate expiry and trigger callback
    void triggerCallback() {
        // Call the protected callback via base pointer
        callback(this);
    }

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

TEST(TimerTest, CallbackCalledOnce) {
    Timers::TestTimer t("test", 10, false);

    // simulate expiry
    t.triggerCallback();  // indirectly calls the protected callback
    EXPECT_EQ(t.callbackCount, 1);
}

