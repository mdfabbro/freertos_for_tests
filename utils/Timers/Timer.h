#ifndef __TIMERS_TIMER_H__
#define __TIMERS_TIMER_H__

namespace Timers {

using Ticks = unsigned int ;

void incrementCurrentTicks(Ticks toAdd);
Ticks getCurrentTicks();
void resetCurrentTicks();

class Timer {
public:
    using Callback = void (*)(Timer*);
protected:
    mutable const char * timerName {nullptr};   // Name
    Ticks period {0};                           // 
    Ticks nextExpiration {0};                   // When it will expire next time
    Callback callback {nullptr};                // The function that will be called when the timer expires.
    bool autoReload {false};
    bool enabled {false};

public:
    Timer( const char * timerName, Ticks period, Callback callback, bool autoReload ) 
        :   timerName(timerName),
            period(period),
            callback(callback),
            autoReload(autoReload) {}

    virtual ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = default;
    Timer& operator=(Timer&&) = default;


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

    friend void TimerController();
};


}

#endif // __TIMER_CUSTOM_HANDLER_H__