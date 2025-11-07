#include "SendCommand.h"

#include "Command.h"
#include "Lists.h"

using namespace Timers;

bool SendCommand::Start( Timer * t ) {
    EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::START , currentTicks, t});
}
bool SendCommand::Reset( Timer * t ) {
    EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::RESET , currentTicks, t});
}
bool SendCommand::Stop( Timer * t ) {
    EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::STOP , currentTicks, t});
}
bool SendCommand::Reload( Timer * t ) {
    EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::RELOAD , currentTicks, t});
}
bool SendCommand::SingleShot( Timer * t ) {
    EnqueuedCommands::GetList().push(new Command{ Command::TypeSet::SINGLE_SHOT , currentTicks, t});
}