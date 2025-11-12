# ISR / Wake up

Example demonstrating how to simulate a software interrupt in FreeRTOS using C++ threads.
This example creates two tasks:

* **ISRCaller**: periodically triggers a simulated interrupt by calling vPortGenerateSimulatedInterrupt().

* **Deferred**: waits on a binary semaphore that is released from the simulated ISR.

The ISR (triggered asynchronously through a std::thread) calls syncISR.give_ISR(), which wakes up the Deferred task.
When the semaphore is successfully taken, the task prints a message confirming it was unblocked by the ISR.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binarie is `demo_isr_wake_up`, which will be available in the `build` directory.

## Execute

If you run the example binary from the `build` directory, the expected output will be:

```bash
$ ./demo_isr_wake_up 
[Deferred] Called from ISR!
[Deferred] Called from ISR!
```

