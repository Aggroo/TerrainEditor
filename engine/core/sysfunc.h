#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::SysFunc
    
    Wrap some platform specific low-level functions.
    
    @copyright	See licence file
*/
#include "config.h"
#if __WIN32__
#include "core/windows/win32sysfunc.h"
namespace Core
{
class SysFunc : public Win32::SysFunc
{
    // empty
};
} // namespace Core
#elif ( __LINUX__ )
#include "core/linux/linuxsysfunc.h"
namespace Core
{
class SysFunc : public Linux::SysFunc
{
    // empty
};
} // namespace Core
#else
#error "Core::SysFunc not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
