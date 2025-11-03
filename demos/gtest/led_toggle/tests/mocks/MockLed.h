#pragma once
#include "Led/Interfaces.h"

class MockLed : public Led::ILed {
private:
    State state ;
public:
    State GetState() { return state; };

    void Set(State state) override {
        this->state = state;
    }

    State Toggle() override {
        state = (state == State::On) ? State::Off : State::On;
        return state;
    }
};
