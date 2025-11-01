#include "Sample.h"

// Task that continuously reads from the serial port
void SerialReaderTask(void* arg) {
    TaskData* data = static_cast<TaskData*>(arg);
    char buf[16];

    for (;;) {
        // Read from the serial port (non-blocking)
        size_t n = data->serialPort->receive(buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0'; // null-terminate for printing
            printf("Received: %s\n", buf);

            // Check each character, notify StopSchedulerTask if '1' is received
            for (size_t i = 0; i < n; i++) {
                if (buf[i] == '1') {
                    xTaskNotifyGive(data->notifyHandle);
                }
            } 
        }

        // Delay to yield CPU (100 ms)
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

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
