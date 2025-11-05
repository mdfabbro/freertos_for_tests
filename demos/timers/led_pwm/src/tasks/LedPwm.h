#ifndef __BLINK_H__
#define __BLINK_H__

#include "TaskCpp.h"
#include "TimerCpp.h"   

#include "Led/Interfaces.h"

#include <iostream>


class LedPwmTask : public FreeRTOScpp::TaskClass {
private:
    Led::ILed* led;
    FreeRTOScpp::TimerMember<LedPwmTask> timer;
    TickType_t period;
    uint8_t pwm;

public:
    LedPwmTask(Led::ILed* led, TickType_t period = pdMS_TO_TICKS(1000), uint8_t pwm = 50 )
        : TaskClass("BlinkTask", TaskPrio_Low, 128),
          led(led),
          timer("LedPwmTask", this, &LedPwmTask::spin, period, true), // auto-reload=true
          period(period),
          pwm(pwm)
    {}

    ~LedPwmTask() = default;

    void task() override {
        updateLedAndPeriod();
        timer.start();
        for (;;) {
            TaskBase::delay(portMAX_DELAY);
        }
    }

    void spin() {
        // This will be executed in the TIMER_TASK of the FreeRTOS, so 
        // its priority will be defined by configTIMER_TASK_PRIORITY.
        updateLedAndPeriod();
    }

    inline void updateLedAndPeriod( ) { 
        const TickType_t ticks = xTaskGetTickCount();
        auto state = led->Toggle();
        const TickType_t onTicks = (period * (TickType_t)pwm) / 100;
        const TickType_t newTicks = (state == Led::ILed::State::On) ? onTicks : (period - onTicks) ;
        timer.period( newTicks );
        printf("  |  Current ticks: %8lu", static_cast<unsigned long>(ticks));  
    }

};

#endif // __BLINK_H__
