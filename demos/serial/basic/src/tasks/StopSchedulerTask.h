#ifndef __STOP_SCHEDULER_TASK_H__
#define __STOP_SCHEDULER_TASK_H__

#include <FreeRTOS/Task.hpp>

// Task that waits for a notification to stop the scheduler
void StopSchedulerTask(void*);

#endif // __STOP_SCHEDULER_TASK_H__