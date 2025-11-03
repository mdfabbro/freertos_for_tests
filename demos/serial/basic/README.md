# Serial (Basic)

This demo shows how to simulate a serial communication channel using a PTY on a POSIX system. It runs a minimal FreeRTOS scheduler with two tasks:

1. `SerialReaderTask`: continuously reads from the serial port and prints any received data. If it detects the character '1', it notifies the second task.
2. `StopSchedulerTask`: waits for the notification and calls to stop the scheduler.

This example allows you to interact with the running FreeRTOS tasks from another terminal using a simple echo command, simulating external input to the system.

We will use `xTaskCreate`, `xTaskNotifyGive` and `ulTaskNotifyTake` to avoid relying on standard C++ libraries in portions of code that may later be exported to a microcontroller.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_serial_basic` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_serial_basic 
Listening on port: /tmp/serial_test
To stop the program, try this in a terminal:
    $ echo -n '1' > /tmp/serial_test
```

The program will be blocked there, until we write a `1` in `/tmp/serial_test`. If you open another terminal and run:

```bash
echo -n '1' > /tmp/serial_test
```

The `demo_serial_basic` will output in the original terminal:

```bash
received: 1
Stopping scheduler!
Scheduler finished, exiting program
```