#ifndef __READER_TASK_H__
#define __READER_TASK_H__

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Queue.hpp>
#include "Http/Server.h"

class ReaderTask : public FreeRTOS::Task {
public:
    ReaderTask(Http::IClientSessionController* srv, FreeRTOS::Queue<int>& q)
        : FreeRTOS::Task(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE, "ReaderTask"),
          server(srv), queue(q) {}

    void taskFunction() final {
        while (true) {
            int clientId = server->getNextClientSessionId();
            if (clientId > 0) {
                queue.sendToBack(clientId, portMAX_DELAY);
            }
            delay(xDelay);
        }
    }

private:
    Http::IClientSessionController* server;
    FreeRTOS::Queue<int>& queue;
    static constexpr TickType_t xDelay = 50 / portTICK_PERIOD_MS;
};

#endif // __READER_TASK_H__
