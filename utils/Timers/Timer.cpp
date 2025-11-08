#include "Timer.h"

static volatile unsigned int currentTicks {0};

void Timers::incrementCurrentTicks(Timers::Ticks toAdd) {
    currentTicks += toAdd;
}

void Timers::resetCurrentTicks() {
    currentTicks = 0;
}

Timers::Ticks Timers::getCurrentTicks() {
    return currentTicks;
}
