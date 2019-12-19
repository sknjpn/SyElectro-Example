#include "Command.h"

#include "Command_RemoveObject.h"
#include "Command_RemoveWire.h"
#include "Command_SetObject.h"
#include "Command_SetWire.h"
#include "Command_Switch.h"
#include "Command_MoveObject.h"

shared_ptr<Command> Command::MakeCommand(Deserializer<ByteArray>& reader)
{
	char cmdType;
	reader >> cmdType;

	shared_ptr<Command> command;

	if (cmdType == 0) command = MakeShared<Command_RemoveObject>();
	if (cmdType == 1) command = MakeShared<Command_RemoveWire>();
	if (cmdType == 2) command = MakeShared<Command_SetObject>();
	if (cmdType == 3) command = MakeShared<Command_SetWire>();
	if (cmdType == 4) command = MakeShared<Command_Switch>();
	if (cmdType == 5) command = MakeShared<Command_MoveObject>();

	if (!command) throw Error(U"存在しないコマンド");

	command->load(reader);

	return command;
}
