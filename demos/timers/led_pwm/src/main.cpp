// main.cpp

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include "FreeRTOS/FreeRTOSHooks.h"

#include "tasks/LedPwm.h"
#include "Led/ConsoleLed.h"
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
    // Default values
    TickType_t periodMs = 1000;
    uint8_t pwm = 50;

    // Parse command line arguments (if provided)
    if (argc >= 2) {
        int p = std::atoi(argv[1]);
        if (p > 0)
            periodMs = static_cast<TickType_t>(p);
    }

    if (argc >= 3) {
        int duty = std::atoi(argv[2]);
        if (duty >= 0 && duty <= 100)
            pwm = static_cast<uint8_t>(duty);
    }

    printf("Starting LED PWM with period = %lu ms, duty = %u%%\n",
                static_cast<unsigned long>(periodMs), pwm);

    Led::ConsoleLed led{};
    LedPwmTask blink{&led, pdMS_TO_TICKS(periodMs), pwm};

    vTaskStartScheduler();
    return 0;
}
