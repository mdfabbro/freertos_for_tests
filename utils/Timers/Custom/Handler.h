#ifndef __TIMER_CUSTOM_HANDLER_H__
#define __TIMER_CUSTOM_HANDLER_H__

#include "LinkedList.h"

namespace CustomTimer {

using Ticks = unsigned int ;

class Handler;

// Handler handler;

class  __attribute__((packed)) Timer {
public:
    using Callback = void (*)(Timer*);
private:
    mutable const char * timerName {nullptr};   // Name
    Ticks period {0};                           // 
    Ticks nextExpiration {0};                   // When it will expire next time
    Callback callback {nullptr};                // The function that will be called when the timer expires.
    bool autoReload {false};
    bool enabled {false};

    Timer * next {nullptr};                     // We leave it here to avoid having to new/delete elements in the list.
    friend class Handler;

public:
    Timer( const char * timerName, Ticks period, Callback callback, bool autoReload );

    void start() ;

    void reset() ;

    void stop() ;

   /// void reload(bool enable) ; // TODO: It is a command!

    bool isEnabled() const {
        return enabled;
    }

    bool isAutoReload() const {
        return autoReload;
    }

    inline Ticks nextExpiryTicks() const {
        // Only valid when time is enabled!
        return nextExpiration;
    }

    inline Ticks periodInTicks() const {
        return period;
    }
};

struct TimerCommand;

namespace Command{
    enum Type{ START, RESET, STOP };
    void push(Type type, Timer *t);
    TimerCommand* popFront() ;
}

struct TimerCommand {
    TimerCommand(Command::Type type, CustomTimer::Timer *t) ;
    
    CustomTimer::Command::Type type;
    CustomTimer::Timer *timer;
    CustomTimer::Ticks ticks;
    TimerCommand *next {nullptr};
};

class Handler {
public:

    explicit Handler(
        LinkedList<TimerCommand> *_commands,
        LinkedList<Timer> *_running);

    void processCommands( );

private:
    LinkedList<TimerCommand> *commands;
    LinkedList<Timer> *running;
};


}

#endif // __TIMER_CUSTOM_HANDLER_H__