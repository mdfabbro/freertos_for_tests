# GTest | Leak Detector

This test demonstrates how to run a minimal FreeRTOS task that performs a simple operation, 
while using `LeakDetector` to check for memory leaks.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_leak_detector` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_leak_detector 
Running main() from /home/gigames/temp/freertos/freertos_for_tests/demos/gtest/leak_detection/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from FreeRTOSTest
[ RUN      ] FreeRTOSTest.MinimalTask_NoLeak
[       OK ] FreeRTOSTest.MinimalTask_NoLeak (10 ms)
[ RUN      ] FreeRTOSTest.MinimalTask_LeakerLeaks
[       OK ] FreeRTOSTest.MinimalTask_LeakerLeaks (10 ms)
[----------] 2 tests from FreeRTOSTest (20 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (20 ms total)
[  PASSED  ] 2 tests.
```

### With CTest

From `build` directory, you can use `ctest --output-on-failure`, like this:

```bash
$  ctest --output-on-failure
Test project /<some_path>/freertos_for_tests/demos/gtest/leak_detection/build
    Start 1: demo_leak_detector
1/1 Test #1: demo_leak_detector ...............   Passed    0.02 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.03 sec
```