# freertos_for_tests

The goal of this repository is to provide FreeRTOS demos running on POSIX, which can be easily integrated into CI/CD pipelines.

## Demos

Available demos include:

1. **blink:** Typical blinking example.
2. **serial**: Simulates a serial port using a POSIX PTY. A task reads its input and notifies another task to stop the scheduler.
2. **gtest/basic:** Just a test to make sure the task is valid.
3. **gtest/task_called:** We will try to run a task and make sure it was called.
4. **gtest/leak_detector:** This test demonstrates how to run a minimal task that performs a simple operation, while using `LeakDetector` to check for memory leaks.