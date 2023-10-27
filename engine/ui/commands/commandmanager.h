#pragma once
//------------------------------------------------------------------------------
/**
    Command Manager for undo and redo history

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"

namespace UI
{

class CommandBase;
class ChangeValueCommand;

class CommandManager
{
__DeclareSingleton(CommandManager);

public:

    void AddCommand(Ptr<CommandBase> cmd);
    void SetNoMergeMostRecent();

    void Undo();
    void Redo();

private:
    static Ptr<CommandBase> commands[1000];
    static int commandSize;
    static int commandPtr;
};
} // namespace UI
