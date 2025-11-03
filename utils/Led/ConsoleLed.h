#ifndef __CONSOLE_LED_H__
#define __CONSOLE_LED_H__

#include "Interfaces.h"
#include <iostream>

namespace Led {

class ConsoleLed : public ILed {
private:
    State state {State::Off};
public:

    void Set(State state) override {
        if (state != this->state) {
            Toggle();   
        }
    }

    State Toggle() override {
        if (state == State::On) {
            std::cout << "\r⚫" << std::flush;
            state = State::Off;
        } else {
            std::cout << "\r🟢" << std::flush;
            state = State::On;
        }
        return state;
    }
};

}

#endif // __LED_H__