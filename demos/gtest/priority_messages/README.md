# GTest / Priority Messages

You are working with a legacy system that uses a custom message–priority protocol. 
Before migrating the code, you want to ensure full backward compatibility, so you decide to 
write a set of tests. To do that, you first need to clearly understand and document how the 
message–processing rules work.

Each message contains three priority-relevant fields:

1. **Origin**: Device that generated the message.
    - Priority: 0 = highest priority, 255 = lowest.
2. **Destination**: Target device of the message.
    - Priority: 0 = highest priority, 255 = lowest.
3. **Message ID**: Type of message.
    - Priority order:
        255 → Emergency (highest possible priority)
        254 … 0 → Normal messages (higher number = higher priority)

If a message has `MessageID = 255`, it is treated as Emergency.
Priority rules for emergencies:
1. All emergency messages are processed before any non-emergency message.
2. If multiple emergency messages exist, their priority is determined using:
    1. Origin (ascending: lower = higher priority)
    2. Destination (ascending)

In other words:

*Among emergencies, the device that generated the message takes priority.
If two come from the same device, the one targeting the highest-priority destination goes first.*

For non-emergency messages, the priority is:
1. Origin — message from the highest-priority device is considered first.
2. Destination — if multiple messages share the same origin, the target device breaks the tie.
3. Message ID — if all the above are the same, compare Message IDs (254 highest, 0 lowest).

**Summerizing**:

- Emergency messages always interrupt the normal flow and must be handled immediately.
- If there are several emergencies, handle them based on which device sent them (lowest origin number first).
- For regular messages, priority is determined by the sender, then the receiver, and finally the message type.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binaries is `demo_priority_messages_test`, which will be
available in the `build` directory.

### With GoogleTest

To run the unit tests directly:

```bash
$ ./demo_priority_messages_test 
Running main() from /<some_path>/freertos_for_tests/demos/gtest/priority_messages/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 8 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 5 tests from PriorityCircularBufferTest
[ RUN      ] PriorityCircularBufferTest.AccessWithAtAndOperatorIndex
[       OK ] PriorityCircularBufferTest.AccessWithAtAndOperatorIndex (0 ms)
[ RUN      ] PriorityCircularBufferTest.InsertShouldPlaceHighestFirstAfterReorder
...
[----------] Global test environment tear-down
[==========] 8 tests from 2 test suites ran. (0 ms total)
[  PASSED  ] 8 tests.
```

### With CTest

From the `build` directory, you can also use CTest to execute all tests:

```bash
$ ctest
Test project /<some_path>//freertos_for_tests/demos/gtest/priority_messages/build
    Start 1: demo_priority_messages_test
1/1 Test #1: demo_priority_messages_test ......   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.03 sec
```
