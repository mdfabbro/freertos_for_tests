#include "Http/Server.h"
#include "tasks/ReaderTask.h"
#include "tasks/ResponderTask.h"

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
    #include "queue.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"


int main() {
    constexpr int PORT = 8080;
    constexpr int MAX_CLIENTS = 5;
    // Initialize the HTTP server
    Http::Server server(PORT, MAX_CLIENTS);
    server.start();

    // Create a queue to pass client IDs between tasks
    QueueHandle_t clientQueue = xQueueCreate(10, sizeof(int));

    ReaderTask reader(&server, clientQueue);
    ResponderTask responder(&server, clientQueue);

    // Start the FreeRTOS scheduler
    FreeRTOS::Kernel::startScheduler();

    // Only reached if the scheduler ends
    server.stop();
    return 0;
}
