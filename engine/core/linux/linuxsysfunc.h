#pragma once
//------------------------------------------------------------------------------
/**
    @class Posix::SysFunc
    
    Provides Posix specific helper functions.
    
    @copyright  See LICENSE file
*/
#include "core/types.h"
#include "core/exithandler.h"
#include "foundation/util/string.h"

namespace System
{
    class SystemInfo;
}

//------------------------------------------------------------------------------
namespace Linux
{

class SysFunc
{
public:
    /// setup lowlevel static objects (must be called before spawning any threads)
    static void Setup();
    /// exit process, and to proper cleanup, memleak reporting, etc...
    static void Exit(int exitCode);
    /// display an error message box
    static void Exception(const char* error);
    /// print a message on the debug console
    static void DebugOut(const char* msg);
    /// display a message box which needs to be confirmed by the user
    static void MessageWindow(const char* msg);
    /// sleep for a specified amount of seconds
    static void Sleep(double sec);
    /// get system information
    //static const System::SystemInfo* GetSystemInfo();
    /// get system time as a string with format "hh:mm:ss"
	static Util::String GetTimeFormatted();

private:    
    friend class Core::ExitHandler;
    /// register an exit handler which will be called from within Exit()
    static const Core::ExitHandler* RegisterExitHandler(const Core::ExitHandler* exitHandler);

    static bool volatile SetupCalled;
    static const Core::ExitHandler* ExitHandlers;     // forward linked list of exit handlers
    //static System::SystemInfo systemInfo;
};

};
//------------------------------------------------------------------------------
