#ifndef __TIMERS_COMMAND_H__
#define __TIMERS_COMMAND_H__

#include "Queue/QueueLinked.h"
#include "Timer.h"

namespace Timers {

struct Command {
    enum TypeSet{ START, RESET, STOP, RELOAD, SINGLE_SHOT };
    TypeSet  type;
    Timer * timer;
    Ticks creation;

    Command(TypeSet type, Timer * timer, Ticks creation) : type(type), timer(timer), creation(creation) {
    }

    Command(TypeSet type, Timer * timer) : type(type), timer(timer), creation(getCurrentTicks()) {
    }

    
};

}

#endif // __TIMER_CUSTOM_HANDLER_H__