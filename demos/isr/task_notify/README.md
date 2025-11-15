# ISR / Task Notify

This example demonstrates how to simulate an ADC interrupt and deferred processing in a FreeRTOS-like.
The simulation involves multiple tasks communicating via task notifications (`xTaskNotifyFromISR` / `xTaskNotify`).

The example consists of three main components:

- `vPortGenerateSimulatedInterrupt`: simulates an ADC hardware interrupt. It periodically generates samples 
of a noisy sine wave and notifies the ADCDeferred task that new data is available.
- `ADCDeferred`: acts as a deferred processing task. It receives notifications from the ADC ISR, stores the 
incoming samples in a circular buffer, and tracks partially filled blocks. Once a block (1/4 of the buffer)
is filled, it notifies the Filter task to process the data.
- `Filter`: waits for notifications from the `ADCDeferred` task. When a notification is received, it applies
a moving average filter to the newly filled block of samples in the buffer.

This simulation demonstrates a typical producer-deferred-consumer pattern in embedded systems, where:
- The ISR produces data.
- The deferred task (`ADCDeferred`) handles buffering and prepares data.
- The consumer task (`Filter`) processes the buffered data in chunks.

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

