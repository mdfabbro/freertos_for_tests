#ifndef __LED_INTERFACES_H__
#define __LED_INTERFACES_H__

namespace Led {

struct ILed {
    enum class State { Off, On };
    
    virtual ~ILed() = default;

    virtual void Set(State state) = 0;
    virtual State Toggle() = 0;
};

}

#endif // __LED_INTERFACES_H__