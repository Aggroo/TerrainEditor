//------------------------------------------------------------------------------
//  exithandler.cpp
//	@copyright	See LICENSE file
//------------------------------------------------------------------------------
#include "config.h"
#include "core/exithandler.h"
#include "core/sysfunc.h"

namespace Core
{

//------------------------------------------------------------------------------
/**
*/
ExitHandler::ExitHandler()
{
    this->nextExitHandler = SysFunc::RegisterExitHandler(this);
}        

//------------------------------------------------------------------------------
/**
*/
ExitHandler::~ExitHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
const ExitHandler*
ExitHandler::Next() const
{
    return this->nextExitHandler;
}

//------------------------------------------------------------------------------
/**
*/
void
ExitHandler::OnExit() const
{
    // empty, override in subclass!
}

} // namespace Core
