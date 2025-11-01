#ifndef __READER_TASK_H__
#define __READER_TASK_H__

#include <FreeRTOS/Task.hpp>
#include "Http/Server.h"

extern "C" {
    #include "queue.h"
}

class ReaderTask : public FreeRTOS::Task {
public:
    ReaderTask(Http::IClientSessionController* srv, QueueHandle_t q)
        : FreeRTOS::Task(tskIDLE_PRIORITY + 1, 1024, "ReaderTask") , server(srv), queue(q) {}

    void taskFunction() final {
        while (true) {
            int clientId = server->getNextClientSessionId();
            if (clientId > 0) {
                xQueueSend(queue, &clientId, portMAX_DELAY);
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }

private:
    Http::IClientSessionController* server;
    QueueHandle_t queue;
};

#endif // __READER_TASK_H__
