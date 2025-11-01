#ifndef __SERIAL_SAMPLE_H__
#define __SERIAL_SAMPLE_H__

#include <FreeRTOS/Task.hpp>

#include "Serial/Interfaces.h"

// Structure to pass data to the SerialReaderTask
struct TaskData {
    Serial::ITransceiver* serialPort; // pointer to the serial interface
    TaskHandle_t notifyHandle;        // handle to the task to notify (StopSchedulerTask)
};

// Task that continuously reads from the serial port
void SerialReaderTask(void* arg);

// Task that waits for a notification to stop the scheduler
void StopSchedulerTask(void*);

#endif // __SERIAL_SAMPLE_H__