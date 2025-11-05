# Timers / Led PWM

Example showing how to use a FreeRTOS software timer to implement a
software-based PWM for an LED.

The LED's ON/OFF times are adjusted dynamically according to a given
duty cycle (`pwm`) and total period (`period`). The timer automatically
reschedules itself to achieve the desired brightness effect.

**NOTE:** The timer callback runs in the FreeRTOS *Timer Service Task*
context, so its execution priority is defined by `configTIMER_TASK_PRIORITY`.
This example is mainly for demonstration purposes and timing verification.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_timer_led_pwd ` will be available in the `build` directory.

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_timer_led_pwd 800 30
Starting LED PWM with period = 800 ms, duty = 30%
🟢  |  Current ticks:    85601
```

### Input parameters

This example accepts two optional parameters:

1. **period (in milliseconds)**: Defines the total duration of one full ON/OFF LED cycle.
For example, 800 means the LED completes one full blink cycle every 800 ms. *Default:* 1000 ms
2. **pwm (0–100)**: Defines the duty cycle, i.e., the percentage of time the LED stays ON during the period.
For example, 30 means the LED is ON for 30% of the time and OFF for 70%. *Default:* 50 %

