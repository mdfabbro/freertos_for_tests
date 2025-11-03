#include <gtest/gtest.h>
#include "tasks/Blink.h"
#include "MockLed.h"

using namespace testing;

+TEST(BlinkTaskTests, StepTogglesLed) {
    MockLed led;
    
    led.Set(Led::ILed::State::Off);
    EXPECT_EQ( led.GetState(), Led::ILed::State::Off );

    BlinkTask task(&led);
    
    // We do two steps and the Led should go On and Off.
    task.spin();
    EXPECT_EQ( led.GetState(), Led::ILed::State::On );
    task.spin();
    EXPECT_EQ( led.GetState(), Led::ILed::State::Off );
}

TEST(BlinkTaskTests, StepTogglesLedMultiple) {
    MockLed led;
    
    led.Set(Led::ILed::State::Off);
    EXPECT_EQ( led.GetState(), Led::ILed::State::Off );

    BlinkTask task(&led);
    
    for(int i = 0; i < 100; ++i) {
        task.spin();
        EXPECT_EQ( led.GetState(), Led::ILed::State::On );
        task.spin();
        EXPECT_EQ( led.GetState(), Led::ILed::State::Off );
    }
}