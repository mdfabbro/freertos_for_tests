#ifndef __ALIVE_TASK_H__
#define __ALIVE_TASK_H__

#include <FreeRTOS/Task.hpp>
#include "Serial/BaseChannel.h"

class AliveTask : public FreeRTOS::Task {
private:
    Serial::BaseChannel* serial;
    uint8_t deviceId;
    static constexpr TickType_t xDelayAlive = 1000 / portTICK_PERIOD_MS;
public:
    AliveTask(Serial::BaseChannel* serial_, uint8_t deviceId_)
        : FreeRTOS::Task(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE, "AliveTask") ,
          serial(serial_), deviceId(deviceId_) {}
    
    struct  __attribute__((packed)) AliveMessage {
        uint8_t MessageID = 0x81;
        char deviceId[2] {};
        explicit AliveMessage(uint8_t deviceId_) {
            auto ToHexaChar = [](uint8_t d) -> char {
                if(d > 15) return '?';
                if(d > 9)  return (d - 10) + 'A';
                return d + '0';
            };
            deviceId[0] = ToHexaChar(deviceId_ >> 4);
            deviceId[1] = ToHexaChar(deviceId_ % 16);
        };
    };
    static_assert(sizeof(AliveMessage) == 3, "AliveMessage must be exactly 3 bytes");

    void taskFunction() final {
        AliveMessage message {deviceId};
        while (true) {
            serial->transmit(static_cast<void*>(&message), sizeof(AliveMessage));
            delay(xDelayAlive);
        }
    }
};

#endif // __ALIVE_TASK_H__
