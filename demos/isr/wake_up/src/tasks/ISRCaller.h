#ifndef __TASKS_ISR_CALLER_H__
#define __TASKS_ISR_CALLER_H__

#include "TaskCpp.h"
#include "Led/Interfaces.h"
#include "Port.h"

class ISRCaller : public FreeRTOScpp::TaskClass {
private:
    static constexpr int InterruptID {3};
public:
    ISRCaller()
    : TaskClass("ISRCaller", TaskPrio_Low, 128) {
    }

    ~ISRCaller() = default;

    void task() override {
        constexpr auto DelayMs = pdMS_TO_TICKS(1000);
        for (;;) {
            spin();
            TaskBase::delay(DelayMs);
        }
    }
    void spin() {
        vPortGenerateSimulatedInterrupt( InterruptID );
    }
};

#endif // __TASKS_ISR_CALLER_H__
