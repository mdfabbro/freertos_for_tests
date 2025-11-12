// main.cpp

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include "FreeRTOS/FreeRTOSHooks.h"

#include "SemaphoreCpp.h"

#include "tasks/ISRCaller.h"
#include "tasks/Deferred.h"
#include <cstdio>
#include <thread>

BinarySemaphore syncISR;

void interruptSoftware( int )
{
    BaseType_t wasWoken {pdFALSE};
    if(!syncISR.give_ISR(wasWoken)) {
        // Something went wrong!
        printf("[ERROR] Unable to do 'syncISR.give_ISR'\n");
    }
    portYIELD_FROM_ISR( wasWoken );
}

void vPortGenerateSimulatedInterrupt(int interruptID)
{
    std::thread([interruptID]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // TODO: Random latency...
        interruptSoftware(interruptID);
    }).detach();
}

int main() {
    ISRCaller caller{};
    Deferred deferred{&syncISR};
    vTaskStartScheduler();
    return 0;
}
