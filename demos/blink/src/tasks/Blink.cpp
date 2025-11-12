#include "Blink.h"

BlinkTask::BlinkTask(UBaseType_t priority, const char* name)
    : FreeRTOS::Task(priority, configMINIMAL_STACK_SIZE, name) {
}

void BlinkTask::taskFunction() {
    const TickType_t Periodms = pdMS_TO_TICKS( 100 );
    TickType_t lastWakeTicks = xTaskGetTickCount();
    for (;;) {
        const auto ticks = (unsigned long)xTaskGetTickCount();
        printf("Blink! Total ticks: %lu\n", ticks);
        vTaskDelayUntil(&lastWakeTicks, pdMS_TO_TICKS(Periodms) );
    }
}