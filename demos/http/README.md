# HTTP Server

This demo shows how to run a minimal HTTP server in a POSIX environment using FreeRTOS tasks. The server accepts multiple clients (configurable) and processes HTTP requests asynchronously using a task-based approach:

1. `ReaderTask`: continuously checks for available clients and enqueues their IDs.
2. `ResponderTask`: dequeues client IDs, reads the HTTP request, and sends a simple HTTP response.

This setup allows you to simulate FreeRTOS-style task scheduling on a PC and test asynchronous request handling without an actual microcontroller.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_http` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_http 
[HTTP][Server] Listening on port: 8080
```

The program will be blocked there, until we do an HTTP request. If you open another terminal and run:

```bash
$ curl http://localhost:8080
Hello from FreeRTOS server!
```

**NOTE**: *We can also open a browser a use the URL [http://localhost:8080](http://localhost:8080)

The `demo_http` will output in the original terminal:

```bash
[HTTP][Server] Client '4' connected
```

