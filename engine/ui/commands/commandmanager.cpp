//------------------------------------------------------------------------------
//  commandmanager.cpp
//  (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "commandmanager.h"
#include "ui/commands/commandbase.h"
#include "ui/commands/ChangeValueCommand.h"

namespace UI
{
Ptr<CommandBase> CommandManager::commands[1000] = {};
int CommandManager::commandSize = 0;
int CommandManager::commandPtr = -1;

CommandManager::CommandManager()
{

}

void CommandManager::AddCommand(Ptr<CommandBase> cmd)
{
	cmd->Execute();

	if (commandPtr < commandSize - 1) 
	{
		for (int i = commandSize - 1; i > commandPtr; i--)
		{
			delete commands[i];
		}

		commandSize = commandPtr + 1;
	}

	commands[commandSize] = cmd;
	commandSize++;

	if (commandSize > 1 && commands[commandSize - 1]->CanMerge() && commands[commandSize - 2]->CanMerge()) 
	{
		if (commands[commandSize - 1]->MergeWith(commands[commandSize - 2]))
		{
			delete commands[commandSize - 1];
			commandSize--;
		}
	}

	commandPtr = commandSize - 1;
}

void CommandManager::SetNoMergeMostRecent() 
{
	if (commandSize - 1 >= 0)
	{
		commands[commandSize - 1]->SetNoMerge();
	}
}

void CommandManager::Undo() 
{
	if (commandPtr >= 0)
	{
		commands[commandPtr]->Undo();
		commandPtr--;
	}
}

void CommandManager::Redo()
{
	int redoCommand = commandPtr + 1;
	if (redoCommand < commandSize && redoCommand >= 0)
	{
		commands[redoCommand]->Execute();
		commandPtr++;
	}
}

} // namespace UI
