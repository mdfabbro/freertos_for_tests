#ifndef __TASKS_ISR_CALLER_H__
#define __TASKS_ISR_CALLER_H__

#include "TaskCpp.h"
#include "Led/Interfaces.h"
#include "Port.h"

class ISRCaller : public FreeRTOScpp::TaskClass {
private:
    static constexpr int InterruptID {3};
    static constexpr auto DelayMs = pdMS_TO_TICKS( 1 );
public:
    ISRCaller()
    : TaskClass("ISRCaller", TaskPrio_High, 128) {
    }

    ~ISRCaller() = default;

    void task() override {
        delay(pdMS_TO_TICKS( 2 )); // initial delay to make sure everything else is up!
        TickType_t prev = xTaskGetTickCount() ;
        for (;;) {
            spin();
            delayUntil(prev, DelayMs);
        }
    }
    void spin() {
        // Generating a SW interruption
        vPortGenerateSimulatedInterrupt( InterruptID );
    }
};

#endif // __TASKS_ISR_CALLER_H__
