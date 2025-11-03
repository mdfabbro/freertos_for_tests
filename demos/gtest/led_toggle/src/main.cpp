// main.cpp

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include "FreeRTOS/FreeRTOSHooks.h"

#include "tasks/Blink.h"
#include "Led/ConsoleLed.h"
#include <cstdio>

int main() {
    Led::ConsoleLed led{};
    BlinkTask blink{&led};
    vTaskStartScheduler();
    return 0;
}
