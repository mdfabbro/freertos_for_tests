// main.cpp

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"

#include "tasks/Blink.h"

#include "Serial/ChannelHandle.h"
#include "Serial/Server.h"
#include "Serial/Client.h"

#include <iostream>
#include <chrono>
#include <thread>

// Structure to pass data to the SerialReaderTask
struct TaskData {
    Serial::ITransceiver* serialPort; // pointer to the serial interface
    TaskHandle_t notifyHandle;        // handle to the task to notify (StopSchedulerTask)
};

// Task that continuously reads from the serial port
void SerialReaderTask(void* arg) {
    TaskData* data = static_cast<TaskData*>(arg);
    char buf[16];

    for (;;) {
        // Read from the serial port (non-blocking)
        ssize_t n = data->serialPort->receive(buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0'; // null-terminate for printing
            printf("Received: %s\n", buf);

            // Check each character, notify StopSchedulerTask if '1' is received
            for (int i = 0; i < n; i++) {
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

int main() {
    const std::string serialDevice{"/tmp/serial_test"};

    // Create a Serial::Server using a PTY channel
    Serial::Server server{Serial::ChannelHandle{serialDevice}};

    std::cout << "Listening on port: "<< serialDevice << std::endl;
    std::cout << "To stop the program, try this in a terminal:"<< std::endl;
    std::cout << "    $ echo -n '1' > "<< serialDevice << std::endl;

    TaskHandle_t stopTaskHandle = nullptr;

    // Create the task responsible for stopping the scheduler
    xTaskCreate(
        StopSchedulerTask,
        "StopScheduler",
        configMINIMAL_STACK_SIZE,
        nullptr,
        tskIDLE_PRIORITY + 1,
        &stopTaskHandle
    );

    // Prepare data to pass to the SerialReaderTask
    TaskData data{&server, stopTaskHandle};

    // Create the task that reads the serial port
    xTaskCreate(
        SerialReaderTask,
        "SerialReader",
        configMINIMAL_STACK_SIZE,
        &data,
        tskIDLE_PRIORITY + 1,
        nullptr
    );

    // Start the FreeRTOS scheduler (blocks until endScheduler is called)
    FreeRTOS::Kernel::startScheduler();

    // Code here executes only after scheduler has stopped
    std::cout << "Scheduler finished, exiting program\n";
    return 0;
}
