# GTest / LED Toggle

Example demonstrating a basic unit test for an LED blinking task using GoogleTest.
This test validates that the BlinkTask correctly toggles the LED state (ON/OFF) when its spin() method is called.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binaries are `demo_led_toggle` and `demo_led_toggle_test`, which will be
available in the `build` directory.

## Execute

If you run the example binary from the `build` directory, the expected output will be:

```bash
$ ./demo_led_toggle
🟢
```

### With GoogleTest

To run the unit tests directly:

```bash
$ ./demo_led_toggle_test
Running main() from /<some_path>/freertos_for_tests/demos/gtest/led_toggle/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from BlinkTaskTests
[ RUN      ] BlinkTaskTests.StepTogglesLed
[       OK ] BlinkTaskTests.StepTogglesLed (0 ms)
[ RUN      ] BlinkTaskTests.StepTogglesLedMultiple
[       OK ] BlinkTaskTests.StepTogglesLedMultiple (0 ms)
[----------] 2 tests from BlinkTaskTests (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

These tests ensure that the LED toggles between ON and OFF states consistently —
both for a single toggle and for multiple repetitions.

### With CTest

From the `build` directory, you can also use CTest to execute all tests:

```bash
$  ctest --output-on-failure
Test project /<some_path>/freertos_for_tests/demos/gtest/led_toggle/build
    Start 1: demo_led_toggle_test
1/1 Test #1: demo_led_toggle_test .............   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```
