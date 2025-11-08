# freertos_for_tests

The goal of this repository is to provide FreeRTOS demos running on POSIX, which can be easily integrated into CI/CD pipelines.

Since we are using FreeRTOS-Cpp, some examples rely on *standard C++ libraries* in portions of code that would normally run only on the microcontroller. If these libraries are not available, the code can be easily adapted to remove the dependency.

## Demos

Available demos include:

1. **blink:** Typical blinking example.
2. **serial/basic**: Simulates a serial port. A task reads its input and notifies another task to stop the scheduler.
3. **serial/client_server**: Simulates bidirectional serial communication between two FreeRTOS applications (a client and a server). The client periodically sends alive messages with its device ID, while the server reads and displays them.
4. **gtest/basic:** Just a test to make sure the task is valid.
5. **gtest/led_toggle:** Unit tests using a mocked LED, including a basic blinking example that validates the LED toggle behavior under FreeRTOS task scheduling.
6. **gtest/leak_detector:** This test demonstrates how to run a minimal task that performs a simple operation, while using `LeakDetector` to check for memory leaks.
7. **gtest/utils/queues:** Some tests for the different types of queues available in `utils`.
8. **gtest/utils/timers:** We will create our own SW Timers and tests them.
9. **http/server**: Minimal HTTP server running on a POSIX system with FreeRTOS tasks. Uses a ReaderTask to detect incoming clients and a ResponderTask to handle requests asynchronously, allowing testing of FreeRTOS-style scheduling and multi-client HTTP handling on a PC.
10. **timers/led_pwm**: Demonstrates the use of a FreeRTOS software timer to implement a simple LED PWM. The timer adjusts the LED’s ON/OFF times based on the configured duty cycle (`pwm`) and total period (`period`).
