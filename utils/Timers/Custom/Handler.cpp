#include "Handler.h"
#include "LinkedList.h"

static volatile unsigned int currentTicks {0};

LinkedList<CustomTimer::TimerCommand> timerCommandList {};

CustomTimer::TimerCommand::TimerCommand(CustomTimer::Command::Type type, CustomTimer::Timer *t) 
    :   type(type), 
        timer(t) , 
        ticks(currentTicks) { // We save here the current ticks! When we create it!
}    

CustomTimer::TimerCommand* CustomTimer::Command::popFront() {
    return timerCommandList.remove(timerCommandList.begin());
}

void CustomTimer::Command::push(CustomTimer::Command::Type type, CustomTimer::Timer *t) {
    timerCommandList.insertBack(new TimerCommand(type,t));
}

using namespace CustomTimer;
Timer::Timer( const char * timerName, Ticks period, Callback callback, bool autoReload) :
    timerName(timerName), period(period), callback(callback), autoReload(autoReload) {
};

void Timer::start() {
    Command::push(CustomTimer::Command::Type::START, this);
}

void Timer::reset() {
    Command::push(CustomTimer::Command::Type::RESET, this);
}

void Timer::stop() {
    Command::push(CustomTimer::Command::Type::STOP, this);
}

CustomTimer::Handler::Handler(
    LinkedList<CustomTimer::TimerCommand> *_commands,
    LinkedList<Timer> *_running) 
    : commands(_commands), running(_running) { }

void CustomTimer::Handler::processCommands( ) {
    TimerCommand* command = commands->begin();
    while( command ) { // As we delete the first command every time.
        switch(command->type) {
            case CustomTimer::Command::Type::START:
                command->timer->enabled = true;
                [[fallthrough]];
            case CustomTimer::Command::Type::RESET:
                // TODO: It could have expired....
                command->timer->nextExpiration = command->ticks + command->timer->period;
                break;
            case CustomTimer::Command::Type::STOP:
                // TODO: It could have expired....
                command->timer->enabled = false;
                break;
        }
        auto toDelete = commands->remove(command);
        delete toDelete;
        command = commands->begin();
    }
}