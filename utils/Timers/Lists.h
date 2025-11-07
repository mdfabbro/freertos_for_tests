#ifndef __TIMERS_LISTS_H__
#define __TIMERS_LISTS_H__

#include "Queue/QueueLinked.h"
#include "Timer.h"
#include "Command.h"

namespace Timers {
class Running {
private:
    QueueLinked<Timer> list {};
    Running() = default;
    Running(const Running &) = delete;
    Running & operator=(const Running &) = delete;

public:
    static QueueLinked<Timer>& GetList() {
        static Running running;
        return running.list;
    }
};

class EnqueuedCommands {
private:
    QueueLinked<Command> list {};
    EnqueuedCommands() = default;

public:
    EnqueuedCommands(const EnqueuedCommands &) = delete;
    EnqueuedCommands& operator=(const EnqueuedCommands &) = delete;

    static QueueLinked<Command>& GetList() {
        static EnqueuedCommands commands;
        return commands.list;
    }
};

}

#endif // __TIMER_CUSTOM_LINKED_LIST_H__