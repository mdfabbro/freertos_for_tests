#ifndef __TASKS_DEFERRED_H__
#define __TASKS_DEFERRED_H__

#include "TaskCpp.h"
#include "SemaphoreCpp.h"
#include <stdio.h>

class Deferred : public FreeRTOScpp::TaskClass {
private:
    BinarySemaphore *semaphore {nullptr};
public:
    Deferred(BinarySemaphore *semaphore)
    : TaskClass("Deferred", TaskPrio_Low, 128), semaphore(semaphore) {
    }

    ~Deferred() = default;

    void task() override {
        for (;;) {
            spin();
        }
    }

    void spin() {
        if( semaphore->take( ) ) {
            printf("[Deferred] Called from ISR!\n");
        }   
    }
};

#endif // __TASKS_DEFERRED_H__
