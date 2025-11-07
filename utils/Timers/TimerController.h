#ifndef __TIMERS_TIMER_CONTROLLER_H__
#define __TIMERS_TIMER_CONTROLLER_H__

#include "Interfaces.h"
#include "Command.h"
#include "Lists.h"

namespace Timers{

void TimerController() {
    auto* command = EnqueuedCommands::GetList().front(); 
    while( command ) {
        auto * timer = command->timer;
        switch (command->type)
        {
        case Command::TypeSet::START:
            timer->nextExpiration = timer->period + command->creation;
            if(!timer->isEnabled()) {
                timer->enabled = true;
                Running::GetList().push(timer);
            }   
            break;

        case Command::TypeSet::RESET:
            if(timer->isEnabled()) {
                if(timer->nextExpiryTicks() >= command->creation) {
                    if(timer->callback)
                        timer->callback(timer);
                    if(!timer->autoReload) {
                        timer->enabled = false;
                    }
                }
            }
            timer->nextExpiration = timer->period + command->creation;
            break;

        case Command::TypeSet::STOP:
            if(timer->isEnabled()) {
                if(timer->nextExpiryTicks() >= command->creation) {
                    if(timer->callback)
                        timer->callback(timer);
                }
                timer->enabled = false;
            }   
            break;

        default:
            break;
        }
        delete command;
        EnqueuedCommands::GetList().pop();
        command = EnqueuedCommands::GetList().front();
    }

    size_t index {0};
    Timer * timer = Running::GetList().at(index); 
    while(timer) {
        if(timer->isEnabled()) {
            if(timer->nextExpiryTicks() >= currentTicks) {
                if(timer->callback) {
                    timer->callback(timer);
                }
                if(!timer->autoReload) {
                    timer->enabled = false;
                } else {
                    timer->nextExpiration = timer->period + timer->nextExpiration;
                }
            } 
        }
        if(!timer->isEnabled()) {
            Running::GetList().erase(index);
        } else {
            ++index;
        }
        timer = Running::GetList().at(index); 
    }
}

}

#endif