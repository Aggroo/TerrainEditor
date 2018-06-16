//------------------------------------------------------------------------------
//  sysfunc.cpp
//  
//  @copyright  See LICENSE file
//------------------------------------------------------------------------------
#include "config.h"
#include <unistd.h>
#include <ctime>
#include "linuxsysfunc.h"
#include "core/refcounted.h"
#include <errno.h>
#include "IO/console.h"

namespace Linux
{
using namespace Util;

bool volatile SysFunc::SetupCalled = false;
const Core::ExitHandler* SysFunc::ExitHandlers = 0;
//System::SystemInfo SysFunc::systemInfo;
    
//------------------------------------------------------------------------------
/**
    This method must be called at application start before any threads
    are spawned. It is used to setup static objects beforehand (i.e.
    private heaps of various utility classes). Doing this eliminates
    the need for fine-grained locking in the affected classes.
*/
void
SysFunc::Setup()
{
    if (!SetupCalled)
    {
         SetupCalled = true;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by Application::Exit(), or otherwise must be
    called right before the end of the programs main() function. The method
    will properly shutdown the Nebula3 runtime environment, and report 
    refcounting and memory leaks (debug builds only). This method will not
    return.
*/
void
SysFunc::Exit(int exitCode)
{
   
    // first produce a RefCount leak report
    #if _DEBUG
    Core::RefCounted::DumpRefCountingLeaks();
    #endif

    // call exit handlers
    const Core::ExitHandler* exitHandler = SysFunc::ExitHandlers;
    while (0 != exitHandler)
    {
        exitHandler->OnExit();
        exitHandler = exitHandler->Next();
    }

    // call static shutdown methods
    //Blob::Shutdown();

    // shutdown global factory object
    Core::Factory::Destroy();

    // report mem leaks
    #if VORTEX_MEMORY_ADVANCED_DEBUGGING
    Memory::DumpMemoryLeaks();
    #endif   

    // finally terminate the process
    exit(exitCode);
}

//------------------------------------------------------------------------------
/**
*/
void
SysFunc::Exception(const char* error)
{
    IO::Console::Instance()->Print(error, IO::EXCEPTION);
    fprintf(stderr, error);
	IO::Console::Instance()->SaveLog("dump.log");
    abort();
}

//------------------------------------------------------------------------------
/**
*/
void
SysFunc::MessageWindow(const char* msg)
{
    // don't do anything except write message to console
    fprintf(stdout, msg);
}


//------------------------------------------------------------------------------
/**
    Sleep for a specified amount of seconds, give up time slice.
*/
void
SysFunc::Sleep(double sec)
{
    int milliSecs = int(sec * 1000.0);
    usleep(milliSecs);
}

//------------------------------------------------------------------------------
/**
*/
Util::String SysFunc::GetTimeFormatted()
{
	Util::String str;
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	
	if (tm.tm_hour >= 10)
		str.AppendInt(tm.tm_hour);
	else
	{
		str.AppendInt(0);
		str.AppendInt(tm.tm_hour);
	}
	str.Append(":");
	
	if (tm.tm_min >= 10)
		str.AppendInt(tm.tm_min);
	else
	{
		str.AppendInt(0);
		str.AppendInt(tm.tm_min);
	}
	str.Append(":");

	if (tm.tm_sec >= 10)
		str.AppendInt(tm.tm_sec);
	else
	{
		str.AppendInt(0);
		str.AppendInt(tm.tm_sec);
	}

	return str;
}


//------------------------------------------------------------------------------
/**
    Put a message on the debug console.
*/
void
SysFunc::DebugOut(const char* msg)
{
    //fprintf(stderr, msg);
    IO::Console::Instance()->Print(msg);
}

//------------------------------------------------------------------------------
/**
*/
//const System::SystemInfo*
//SysFunc::GetSystemInfo()
//{
//    return &SysFunc::systemInfo;
//}

//------------------------------------------------------------------------------
/**
    Register a new exit handler. This method is called at startup time
    from the constructor of static exit handler objects. This is the only
    supported way to register exit handlers. The method will return
    a pointer to the next exit handler in the forward linked list 
    (or 0 if this is the first exit handler).
*/
const Core::ExitHandler*
SysFunc::RegisterExitHandler(const Core::ExitHandler* exitHandler)
{
    const Core::ExitHandler* firstHandler = ExitHandlers;
    ExitHandlers = exitHandler;
    return firstHandler;
}
} // namespace Posix
