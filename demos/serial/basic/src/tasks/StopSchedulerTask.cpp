#include "StopSchedulerTask.h"

// Task that waits for a notification to stop the scheduler
void StopSchedulerTask(void*) {
    for (;;) {
        // Wait indefinitely until notified
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Stopping scheduler!\n");

        // Stop the FreeRTOS scheduler
        FreeRTOS::Kernel::endScheduler();
    }
}