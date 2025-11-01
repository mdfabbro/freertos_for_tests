#ifndef __RESPONDER_TASK_H__
#define __RESPONDER_TASK_H__

#include <FreeRTOS/Task.hpp>
#include "Http/Server.h"
#include "Responder.h"

extern "C" {
    #include "queue.h"
}


class ResponderTask : public FreeRTOS::Task {
public:
    ResponderTask(Http::IClientSessionController* srv, QueueHandle_t q)
        : FreeRTOS::Task(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE, "ReaderTask") ,
          server(srv), queue(q) {}

    void taskFunction() final {
        const bool LogsEnable {false};
        Responder responder{LogsEnable};

        int clientId;
        while (true) {
            // Wait indefinitely for a client ID
            if (xQueueReceive(queue, &clientId, portMAX_DELAY) == pdTRUE) {
                Http::IClientSession* client = server->getClientSession(clientId);
                if (client && !client->isClosed()) {
                    responder.setClientSession(client);
                    responder.respond();
                }
            }
        }
    }

private:
    Http::IClientSessionController* server;
    QueueHandle_t queue;
};

#endif // __RESPONDER_TASK_H__
