#ifndef __BLINK_H__
#define __BLINK_H__

#include <FreeRTOS/Task.hpp>

class BlinkTask : public FreeRTOS::Task {
public:
    BlinkTask(UBaseType_t priority, const char* name);
    void taskFunction() final;
};

#endif // __BLINK_H__