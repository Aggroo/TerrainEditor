#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::Singleton
  
    Implements a system specific Singleton
    
    @copyright	See licence file
*/
#include "config.h"

#if __WIN32__
#include "core/windows/win32singleton.h"
#elif __LINUX__
#include "core/linux/linuxsingleton.h"
#else
#error "IMPLEMENT ME!"
#endif
