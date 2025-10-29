// main.cpp

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"

#include "tasks/Blink.h"

int main() {

    BlinkTask blink((tskIDLE_PRIORITY + 1), "Blink");
    fflush(stdout);

    if (blink.isValid()) {
        FreeRTOS::Kernel::startScheduler();
    } else {
        fprintf(stderr, "No se pudo crear la tarea Blink\n");
        return 1;
    }
    return 0; // Only when calling Kernel::endScheduler()
}
