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

    SerialReaderTask readerTask(&server);

    // Start the FreeRTOS scheduler (blocks until endScheduler is called)
    FreeRTOS::Kernel::startScheduler();

    // Code here executes only after scheduler has stopped
    std::cout << "Scheduler finished, exiting program\n";
    return 0;
}
