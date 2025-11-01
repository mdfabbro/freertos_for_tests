#include "Responder.h"
#include <stdio.h>


void Responder::respond() {
    if (!session || session->isClosed()) return;

    const char* response = "Hello from FreeRTOS server!\n";
    const auto responseLen = strlen(response);
    
    char buf[512];
    size_t bytes = session->read(buf, sizeof(buf));
    if (bytes <= 0) {
        session->close();
        return;
    }
    buf[bytes] = '\0';

    if(enablePrint) {
        printf("[Responder] Received: %s", buf);
    }

    snprintf(buf, sizeof(buf),
                "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %zu\r\n\r\n%s",
                responseLen, response);

    session->write(buf, strlen(buf));
    session->close();
    if(enablePrint) {
        printf("[Responder] Responding: %s", buf);
    }
    session = nullptr; // Done
}
