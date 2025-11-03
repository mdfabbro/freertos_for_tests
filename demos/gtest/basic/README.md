# GTest | Basic

Just a test to make sure the task is valid.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_gtest_blink` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_gtest_blink
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from BlinkTaskTest
[ RUN      ] BlinkTaskTest.Creation
[       OK ] BlinkTaskTest.Creation (0 ms)
[----------] 1 test from BlinkTaskTest (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```

### With CTest

From `build` directory, you can use `ctest --output-on-failure`, like this:

```bash
$ ctest --output-on-failure 
Test project /<some_path>/freertos_for_tests/demos/gtest/basic/build
    Start 1: demo_tests
1/1 Test #1: demo_tests .......................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.02 sec
```