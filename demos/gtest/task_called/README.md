# GTest | Basic

We will try to call a task and make sure it was called.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_task_called` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_task_called 
Running main() from /home/gigames/temp/freertos/freertos_for_tests/demos/gtest/task_called/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from FreeRTOSTest
[ RUN      ] FreeRTOSTest.MinimalTaskRuns
[       OK ] FreeRTOSTest.MinimalTaskRuns (2000 ms)
[----------] 1 test from FreeRTOSTest (2000 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (2000 ms total)
[  PASSED  ] 1 test.
```
