//------------------------------------------------------------------------------
//  @file		debug.cc
//  @copyright	See licence file
//------------------------------------------------------------------------------
#include "config.h"
#include "core/types.h"
#include "foundation/util/string.h"
#include "core/sysfunc.h"
#include <cstdarg>
//#include "IO/console.h"

//------------------------------------------------------------------------------
/**
This function is called by _assert() when the assertion fails.
*/
void
_exception(const char* exp, const char* file, int line)
{
	//if (IO::Console::HasInstance())
	//{
	//	n_error("*** NEBULA ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
	//}
	//else
	{
		Util::String msg;
		msg.Format("*** VORTEX ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
		Core::SysFunc::Exception(msg.AsCharPtr());
	}
}

//------------------------------------------------------------------------------
/**
This function is called by _assert2() when the assertion fails.
*/
void
_exception2(const char* exp, const char* msg, const char* file, int line)
{
	//if (IO::Console::HasInstance())
	//{
	//	n_error("*** NEBULA ASSERTION ***\nprogrammer says: %s\nexpression: %s\nfile: %s\nline: %d\n", msg, exp, file, line);
	//}
	//else
	{
		Util::String fmt;
		fmt.Format("*** VORTEX ASSERTION ***\n\nmessage:\n %s\n\nexpression: %s\nfile: %s\nline: %d\n", msg, exp, file, line);
		Core::SysFunc::Exception(fmt.AsCharPtr());
	}
}


//------------------------------------------------------------------------------
/**
	Formatted exception function
*/
void
_exception_fmt(const char *	exp, const char *fmt, const char *file, int line, ...)
{
	Util::String msg;
	va_list argList;
	va_start(argList, line);
	msg.FormatArgList(fmt, argList);
	va_end(argList);
	Util::String format = Util::String::Sprintf("*** VORTEX ASSERTION ***\n\nmessage:\n %s\n\nfile : %s\nline : %d\nexpression : %s\n", msg.AsCharPtr(), file, line, exp);
	Core::SysFunc::Exception(format.AsCharPtr());

}

//------------------------------------------------------------------------------
/**
	This function is called when a serious situation is encountered.
	This function does not abort the application.
*/
void __cdecl
_error(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	
	Util::String str;
	str.FormatArgList(msg, argList);
	_printf(str.AsCharPtr());
	//IO::Console::Instance()->Print(str, IO::ERROR);
	
	va_end(argList);
}

//------------------------------------------------------------------------------
/**
	This function is called when a warning should be issued which doesn't
	require abortion of the application.
*/
void __cdecl
_warning(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);

	Util::String str;
	str.FormatArgList(msg, argList);
	_printf(str.AsCharPtr());
	//IO::Console::Instance()->Print(str, IO::WARNING);

	va_end(argList);
}

//------------------------------------------------------------------------------
/**
	This function is called when a message should be displayed to the
	user which requires a confirmation (usually displayed as a MessageBox).
*/
void __cdecl
_confirm(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);

	Util::String str;
	str.FormatArgList(msg, argList);
	Core::SysFunc::MessageWindow(str.AsCharPtr());

	va_end(argList);
}

//------------------------------------------------------------------------------
/**
	Terrains's printf replacement. Will redirect the text to the console
	and/or logfile.
*/
void __cdecl
_printf(const char *msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	Util::String s;
	s.FormatArgList(msg, argList);
	printf(s.AsCharPtr());
	//IO::Console::Instance()->Print(s, IO::MESSAGE);
	va_end(argList);
}

//------------------------------------------------------------------------------
/**
	Put process to sleep.
*/
void
_sleep(double sec)
{
	Core::SysFunc::Sleep(sec);
}

//------------------------------------------------------------------------------
/**
	Places a breakpoint that the debugger will stop at
*/
void
_break()
{
#ifndef __WIN32__
	_error("Break not implemented\n");
#else
	_CrtDbgBreak();
#endif
}
