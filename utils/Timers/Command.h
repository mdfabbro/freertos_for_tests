#ifndef __TIMERS_COMMAND_H__
#define __TIMERS_COMMAND_H__

#include "Queue/QueueLinked.h"
#include "Timer.h"

namespace Timers {

struct Command {
    enum TypeSet{ START, RESET, STOP, RELOAD, SINGLE_SHOT };
    TypeSet  type;
    Ticks creation;
    Timer * timer;

    Command(TypeSet type, Ticks creation, Timer * timer) : type(type), creation(creation) , timer(timer) {
    }
};

}

#endif // __TIMER_CUSTOM_HANDLER_H__