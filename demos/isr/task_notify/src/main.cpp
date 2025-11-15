// main.cpp

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include "FreeRTOS/FreeRTOSHooks.h"

#include "tasks/ISRCaller.h"
#include "tasks/ADCDeferred.h"
#include "tasks/Filter.h"
#include <cstdio>
#include <thread>
#include "Queue/Vector.h"
#include "extras/NoisySine.h"


constexpr size_t BufferSize = 64;

NoisySine sine;
ADCDeferred<BufferSize> *adcHandler {nullptr};

void vPortGenerateSimulatedInterrupt(int)
{
    if(adcHandler) {
        BaseType_t wasWoken;
        const auto ADCValue = sine.GetNextSample();    
        if(adcHandler->notify_ISR(ADCValue, eSetValueWithoutOverwrite, wasWoken) != pdPASS) {
            // Something went wrong! 
            printf("[ERROR] Unable to do 'syncISR.give_ISR'\n");
        }
        portYIELD_FROM_ISR( wasWoken );
    }
}

int main() {
    constexpr size_t FilterLength = 24;

    Vector<int16_t, BufferSize> buffer;
    Filter filter {buffer, FilterLength};
    ADCDeferred deferred{filter, buffer};
    adcHandler = &deferred;

    for( size_t i = 0; i < BufferSize; ++i) {
        buffer.push_back(0);
    }    
    ISRCaller caller{};
    vTaskStartScheduler();
    return 0;
}
