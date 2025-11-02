#ifndef __RESPONDER_TASK_H__
#define __RESPONDER_TASK_H__

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Queue.hpp>
#include "Http/Server.h"
#include "Responder.h"

class ResponderTask : public FreeRTOS::Task {
public:
    ResponderTask(Http::IClientSessionController* srv, FreeRTOS::Queue<int>& q)
        : FreeRTOS::Task(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE, "ResponderTask"),
          server(srv), queue(q) {}

    void taskFunction() final {
        const bool LogsEnable {false};
        Responder responder{LogsEnable};

        while (true) {
            const auto clientId = queue.receive(portMAX_DELAY);
            if (clientId.has_value()) {
                Http::IClientSession* client = server->getClientSession(clientId.value());
                if (client && !client->isClosed()) {
                    responder.setClientSession(client);
                    responder.respond();
                }
            }
        }
    }

private:
    Http::IClientSessionController* server;
    FreeRTOS::Queue<int>& queue;
};

#endif // __RESPONDER_TASK_H__
