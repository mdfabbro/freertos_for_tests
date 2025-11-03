#ifndef __BLINK_H__
#define __BLINK_H__

#include "TaskCPP.h"

class BlinkTask : public FreeRTOScpp::TaskClass {
public:
    BlinkTask();

    void task() override;
};

#endif // __BLINK_H__
