#ifndef __BLINK_H__
#define __BLINK_H__

#include "TaskCpp.h"
#include "Led/Interfaces.h"

class BlinkTask : public FreeRTOScpp::TaskClass {
private:
    Led::ILed* led;
public:
    BlinkTask(Led::ILed* led)
    : TaskClass("BlinkTask", TaskPrio_Low, 128), led(led) {
    }

    ~BlinkTask() = default;

    void task() override {
        constexpr auto ToggleTimeMs = pdMS_TO_TICKS(1000);
        for (;;) {
            spin();
            TaskBase::delay(ToggleTimeMs);
        }
    }
    void spin() {
        led->Toggle();
    }
};

#endif // __BLINK_H__
