#include "Blink.h"

BlinkTask::BlinkTask(UBaseType_t priority, const char* name)
    : FreeRTOS::Task(priority, configMINIMAL_STACK_SIZE, name) {
}

void BlinkTask::taskFunction() {
    for (;;) {
        const auto ticks = (unsigned long)xTaskGetTickCount();
        printf("Blink! Total ticks: %lu\n", ticks);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}