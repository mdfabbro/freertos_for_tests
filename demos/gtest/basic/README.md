# GTest | Basic

Just a test to make sure the task is valid.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_tests` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_tests
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
