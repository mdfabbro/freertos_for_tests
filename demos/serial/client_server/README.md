# Serial (Client/Server)

This demo shows how to simulate bidirectional serial communication between two separate FreeRTOS applications using POSIX PTYs — one acting as a device and the other as a host.
Each application runs its own minimal FreeRTOS scheduler and communicates through a virtual serial channel:

1. `AliveTask`: periodically sends an alive message containing its device ID through the serial client.
2. `SerialReaderTask`: runs on the server side, continuously reads incoming messages, and prints the device IDs of connected clients.

This setup emulates a realistic serial link between an embedded device and a host system, allowing both ends to run independently while exchanging structured messages.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected outputs `demo_serial_client` and `demo_serial_server` will be available in the `build` directory

## Execute

First you need to launch the server in one terminal:

```bash
$ ./demo_serial_server 
Listening on port: /tmp/serial_test
```

Now from other terminals, you can create clients by doing:

```bash
./demo_serial_client 10
```

**NOTE**: The `demo_serial_client` expects an input from 0 to 255 with the client ID, allowing you to launch multiple clients in multiple terminals running in parallel.

Once at least a client is running, the `demo_serial_server` outpul will print:

```bash
[Reader] Alive from device: 0x0A
[Reader] Alive from device: 0x37
[Reader] Alive from device: 0x0A
```
