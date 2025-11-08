#include "SendCommand.h"

#include "Command.h"
#include "Lists.h"

using namespace Timers;

bool SendCommand::Start( Timer * t ) {
    return EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::START , t});
}
bool SendCommand::Reset( Timer * t ) {
    return EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::RESET , t});
}
bool SendCommand::Stop( Timer * t ) {
    return EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::STOP , t});
}
bool SendCommand::AutoReload( Timer * t ) {
    return EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::RELOAD , t});
}
bool SendCommand::SingleShot( Timer * t ) {
    return EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::SINGLE_SHOT , t});
}