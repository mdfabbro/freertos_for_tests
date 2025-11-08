# GTest | Utils | Timers

We will create our own SW Timers and tests them

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_gtest_utils_timers` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_gtest_utils_timers 
Running main() from /<some_path>/freertos_for_tests/demos/gtest/utils/timers/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 15 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 2 tests from TimersSingletonTest
[ RUN      ] TimersSingletonTest.RunningListBasicOperations
[       OK ] TimersSingletonTest.RunningListBasicOperations (0 ms)
[ RUN      ] TimersSingletonTest.EnqueuedCommandsBasicOperations
...
[----------] Global test environment tear-down
[==========] 15 tests from 5 test suites ran. (0 ms total)
[  PASSED  ] 15 tests.
```
