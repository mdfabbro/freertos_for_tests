#ifndef __TASKS_ADC_DEFERRED_H__
#define __TASKS_ADC_DEFERRED_H__

#include "TaskCpp.h"
#include <stdio.h>
#include "Queue/Vector.h"

template<size_t Size>
class ADCDeferred : public FreeRTOScpp::TaskClass {
private:
    static constexpr TickType_t TimeoutMs = pdMS_TO_TICKS(1 * 3);
    size_t pushed {0};
    FreeRTOScpp::TaskClass & processingTask;
    Vector<int16_t,Size> &buffer;
    

public:
    ADCDeferred(FreeRTOScpp::TaskClass & processingTask, Vector<int16_t,Size> &buffer)
    : TaskClass("ADCDeferred", TaskPrio_Mid, 128) , processingTask(processingTask), buffer(buffer) {
    }

    ~ADCDeferred() = default;

    void task() override {
        for (;;) {
            spin();
        }
    }

    void spin() {
        uint32_t adcValue ;
        if( wait( 0 , 0, &adcValue, TimeoutMs ) ) {
            constexpr int32_t ADCBits = 12; // Assuming it is a 12 bit ADC
            constexpr int32_t Offset = (1 << (ADCBits - 1)) ; 
            int16_t adjValue = static_cast<int16_t>(static_cast<int32_t>(adcValue) - Offset);
            *buffer[pushed] = adjValue;
            ++pushed;
            if((pushed % (Size/4)) == 0) {
                const uint32_t block = (pushed-1)/(Size/4);
                if(processingTask.notify(block, eSetValueWithoutOverwrite) != pdTRUE) { // We split the buffer in 4 blocks and we pass the index that is full (0, 1, 2 or 3).
                    printf("[ADCDeferred] [ERROR] Unable to notify to the processing task!\n");
                }
                if(pushed == Size) { pushed = 0; } // clear
                
            } 
        } else {
            printf("[ADCDeferred] [ERROR] Timeout!\n");
        }
    }
};

#endif // __TASKS_DEFERRED_H__
