#pragma once
#include <cstdint>

namespace FreeRTOScpp {

class TaskClass {
public:
    TaskClass(const char*, int, int) {}
    virtual ~TaskClass() = default;
    virtual void task() = 0;
};

} // namespace FreeRTOScpp

class TaskBase {
public:
    static void delay(uint32_t) {
        // No Delay
    }
};

enum {
    TaskPrio_Low = 1
};

#ifndef pdMS_TO_TICKS
    #define pdMS_TO_TICKS( xTimeInMs )    ( xTimeInMs )
#endif

