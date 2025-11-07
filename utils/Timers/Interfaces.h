#ifndef __TIMERS_INTERFACES_H__
#define __TIMERS_INTERFACES_H__

namespace Timers {

class Timer;

struct IController {
    virtual ~IController() noexcept {};

    virtual void start() = 0;
    virtual void reset() = 0;
    virtual void stop() = 0;
};

struct IControllerWithReload : public IController{
    virtual ~IControllerWithReload() noexcept {};

    virtual void reload(bool enable) = 0;
};

}
#endif