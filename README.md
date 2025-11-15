# freertos_for_tests

This repository provides FreeRTOS demos running on POSIX, designed to be easily integrated into CI/CD pipelines.

It includes both simple examples and more complex integration scenarios, such as compiling a client and a server that 
communicate with each other.

Many examples rely on C++ wrappers around FreeRTOS and use standard C++ libraries to simulate behavior that would normally run
on a microcontroller. These dependencies exist only for simulation and testing purposes and can be removed when targeting real 
embedded platforms.

## Demos

Available demos include:

1. **blink:** Typical blinking example.
2. **serial/basic**: Simulates a serial port. A task reads its input and notifies another task to stop the scheduler.
3. **serial/client_server**: Simulates bidirectional serial communication between two FreeRTOS applications (a client and a server).
The client periodically sends alive messages with its device ID, while the server reads and displays them.
4. **gtest/basic:** Just a test to make sure the task is valid.
5. **gtest/led_toggle:** Unit tests using a mocked LED, including a basic blinking example that validates the LED toggle behavior 
under FreeRTOS task scheduling.
6. **gtest/leak_detector:** This test demonstrates how to run a minimal task that performs a simple operation, while using 
`LeakDetector` to check for memory leaks.
7. **gtest/priority_messages**: Unit tests that validate the legacy message–priority protocol, ensuring compatibility when ordering 
messages by emergency status, origin, destination, and message ID.
8. **gtest/utils/queues:** Some tests for the different types of queues available in `utils`.
9. **gtest/utils/strings:** Some tests for the `string` class available in `utils`.
10. **gtest/utils/timers:** We will create our own SW Timers and tests them.
11. **http/server**: Minimal HTTP server running on a POSIX system with FreeRTOS tasks. Uses a ReaderTask to detect incoming clients
and a ResponderTask to handle requests asynchronously, allowing testing of FreeRTOS-style scheduling and multi-client HTTP handling 
on a PC.
12. **timers/led_pwm**: Demonstrates the use of a FreeRTOS software timer to implement a simple LED PWM. The timer adjusts the 
LED’s ON/OFF times based on the configured duty cycle (`pwm`) and total period (`period`).
13. **isr/task_notify**: Simulates an ADC interrupt feeding a circular buffer, with a deferred task notifying a filter task to 
apply a moving average.
14. **isr/wakeup**: Demonstrates interrupt-driven task synchronization. A simulated ISR (using `std::thread`) triggers a 
BinarySemaphore that wakes a waiting task, illustrating deferred interrupt processing in FreeRTOS.
