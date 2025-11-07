# GTest / Utils / Queues

Some tests for the Queues utils

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binaries is `demo_utils_queue_test`, which will be
available in the `build` directory.

### With GoogleTest

To run the unit tests directly:

```bash
$ ./demo_utils_queue_test
Running main() from /<some_path>/freertos_for_tests/demos/gtest/utils/queues/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 36 tests from 4 test suites.
[----------] Global test environment set-up.
[----------] 13 tests from DequeFixedTest
[ RUN      ] DequeFixedTest.PushBackShouldInsertAtEnd
[       OK ] DequeFixedTest.PushBackShouldInsertAtEnd (0 ms)
[ RUN      ] DequeFixedTest.PushFrontShouldInsertAtBeginning
[       OK ] DequeFixedTest.PushFrontShouldInsertAtBeginning (0 ms)
...
[----------] Global test environment tear-down
[==========] 36 tests from 4 test suites ran. (0 ms total)
[  PASSED  ] 36 tests.
```

These tests ensure that the LED toggles between ON and OFF states consistently —
both for a single toggle and for multiple repetitions.

### With CTest

From the `build` directory, you can also use CTest to execute all tests:

```bash
$  ctest --output-on-failure
Test project /<some_path>/freertos_for_tests/demos/gtest/utils/queues/build
    Start 1: demo_utils_queue_test
1/1 Test #1: demo_utils_queue_test ............   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```
