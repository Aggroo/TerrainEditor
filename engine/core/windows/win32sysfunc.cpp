//------------------------------------------------------------------------------
//  @file		sysfunc.cc
//  @copyright	See LICENCE file
//------------------------------------------------------------------------------
#include "config.h"
#include "core/windows/win32sysfunc.h"
#include "core/refcounted.h"
//#include "debug/minidump.h"
//#include "foundation/util/blob.h"
//#include "foundation/util/guid.h"
//#include "net/socket/socket.h"
//#include "debug/minidump.h"
//#include "threading/thread.h"
//#include "util/globalstringatomtable.h"
//#include "util/localstringatomtable.h"  
//#include "system/systeminfo.h"
//#include "debug/win32/win32stacktrace.h"
//#include "IO/console.h"
#include <iomanip>
#include <ctime>
#include <process.h>
#include <windows.h>

namespace Win32
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
        //Memory::SetupHeaps();
        //Memory::Heap::Setup();
		//Blob::Setup();
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

    // shutdown the C runtime, this cleans up static objects but doesn't shut 
    // down the process
    _cexit();

    // call static shutdown methods
    //Blob::Shutdown();

    // shutdown global factory object
    Core::Factory::Destroy();

    // report mem leaks
    #if VORTEX_MEMORY_ADVANCED_DEBUGGING
	Memory::DumpMemoryLeaks();
    #endif   

    // finally terminate the process
    ExitProcess(exitCode);
}

//------------------------------------------------------------------------------
/**
    This displays a Win32 error message box and quits the program
*/
void
SysFunc::Exception(const char* error)
{
#ifdef _DEBUG
    OutputDebugString(error);	
#endif
    
	/*
		Callstack
	*/
	//Util::Array<Util::String> stacktrace = Win32StackTrace::GenerateStackTrace();
	Util::String format = "Stack trace not implemented! :D";
	//for (int i = 7; i < Math::min(17,stacktrace.Size()); i++)
	//{
	//	format.Append(stacktrace[i]);
	//	format.Append("\n");
	//}
	format.Format("%s\nCallstack:\n%s", error, format.AsCharPtr());

	//IO::Console::Instance()->Print(format, IO::EXCEPTION);
	//IO::Console::Instance()->SaveLog("dump.log");

	format.Append("\nMore information can be found in the generated dump.log file.\n");

	MessageBox(NULL, format.AsCharPtr(), "VORTEX SYSTEM ERROR", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST | MB_ICONERROR);

    abort();
}

//------------------------------------------------------------------------------
/**
    This displays a Win32 message box
*/
void
SysFunc::MessageWindow(const char* msg)
{
	int result = MessageBox(NULL,
							msg,
							"Vortex Message",
							MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST | MB_ICONINFORMATION
							);
}	

//------------------------------------------------------------------------------
/**
    Sleep for a specified amount of seconds, give up time slice.
*/
void
SysFunc::Sleep(double sec)
{
    int milliSecs = int(sec * 1000.0);
	//Windows-specific sleep
	Sleep(milliSecs);
}

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
	//IO::Console::Instance()->Print(msg);
    OutputDebugString(msg);
}

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

//------------------------------------------------------------------------------
/**
*/
//const System::SystemInfo*
//SysFunc::GetSystemInfo()
//{
//    return &SysFunc::systemInfo;
//}


} // namespace Win32