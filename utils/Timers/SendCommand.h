#ifndef __TIMERS_SEND_COMMAND_H__
#define __TIMERS_SEND_COMMAND_H__

#include "Timer.h"

namespace Timers{

namespace SendCommand {
    bool Start( Timer * t );
    bool Reset( Timer * t );
    bool Stop( Timer * t );
    bool AutoReload( Timer * t );
    bool SingleShot( Timer * t );
}

}

#endif