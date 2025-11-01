extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"

#include "Serial/ChannelHandle.h"
#include "Serial/Server.h"
#include "Serial/Client.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "tasks/Sample.h"

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
