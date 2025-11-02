#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdint>

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

#include "tasks/AliveTask.h"

int main(int argc, char* argv[]) {
    const std::string serialDevice{"/tmp/serial_test"};

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <deviceId (0-255)>\n";
        return 1;
    }

    // Convert and validate
    int id = std::atoi(argv[1]);
    if (id < 0 || id > 255) {
        std::cerr << "Error: deviceId must be between 0 and 255\n";
        return 1;
    }

    uint8_t deviceId = static_cast<uint8_t>(id);

    // Create a Serial::Client using a PTY channel
    Serial::Client client{serialDevice};

    // Create AliveTask with the provided device ID
    AliveTask aliveTask(&client, deviceId);

    // Start the FreeRTOS scheduler (blocks until endScheduler is called)
    FreeRTOS::Kernel::startScheduler();

    std::cout << "Scheduler finished, exiting program\n";
    return 0;
}
