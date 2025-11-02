#ifndef __SERIAL_READER_TASK_H__
#define __SERIAL_READER_TASK_H__

#include <FreeRTOS/Task.hpp>
#include "Serial/BaseChannel.h"
#include <cstdio>

class SerialReaderTask : public FreeRTOS::Task {
private:
    Serial::BaseChannel* serial;
    static constexpr TickType_t xDelayRead = 100 / portTICK_PERIOD_MS;

public:
    explicit SerialReaderTask(Serial::BaseChannel* serial_)
        : FreeRTOS::Task(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE, "SerialReaderTask"),
          serial(serial_) {}

    void taskFunction() final {
        // Just a simple example, we can improve it by adding a circular buffer, 
        // and CRC check.
        uint8_t buf[16];
        for (;;) {
            size_t n = serial->receive(buf, sizeof(buf));
            if (n > 0) {
                for (size_t i = 0; i < n; i++) {
                    if (buf[i] & 0x80) { // Check if it’s a valid MessageType
                        if (buf[i] == 0x81) { // ALIVE message
                            if ((i + 3) <= n) {
                                printf("[Reader] Alive from device: 0x%c%c\n",
                                       (char)buf[i + 1], (char)buf[i + 2]);
                            }
                        }
                    }
                }
            }
            delay(xDelayRead);
        }
    }
};

#endif // __SERIAL_READER_TASK_H__
