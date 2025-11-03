#include "Blink.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

using namespace FreeRTOScpp;

BlinkTask::BlinkTask()
    : TaskClass("BlinkTask", TaskPrio_Low, 128)
{
}

void BlinkTask::task() {
    for (;;) {
        const auto ticks = static_cast<unsigned long>(xTaskGetTickCount());
        printf("Blink! Total ticks: %lu\n", ticks);
        TaskBase::delay(pdMS_TO_TICKS(1000));
    }
}
