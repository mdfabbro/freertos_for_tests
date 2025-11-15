# GTest / Utils / Strings

Some tests for the Strings utils

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binaries is `demo_utils_strings_test`, which will be
available in the `build` directory.

### With GoogleTest

To run the unit tests directly:

```bash
$ ./demo_utils_strings_test
Running main() from /<some_path>/freertos_for_tests/demos/gtest/utils/strings/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 31 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 31 tests from StringTests
[ RUN      ] StringTests.Constructors
[       OK ] StringTests.Constructors (0 ms)
[ RUN      ] StringTests.SaturateAfterInsert
[       OK ] StringTests.SaturateAfterInsert (0 ms)
...
[----------] Global test environment tear-down
[==========] 31 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 31 tests.
```

These tests ensure that the LED toggles between ON and OFF states consistently —
both for a single toggle and for multiple repetitions.

### With CTest

From the `build` directory, you can also use CTest to execute all tests:

```bash
$  ctest --output-on-failure
Test project /<some_path>/freertos_for_tests/demos/gtest/utils/strings/build
    Start 1: demo_utils_strings_test
1/1 Test #1: demo_utils_strings_test ..........   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```
