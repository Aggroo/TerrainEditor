#pragma once
/**
@file       memory.h
@class      Memory::Memory

@brief      Memory subsystem
*/
#include "core/types.h"

#if (__WIN32__)
    #include "foundation/memory/windows/winmemory.h"
#elif ( __LINUX__ )
    #include "foundation/memory/linux/linuxmemory.h"
#else
    #error "UNKNOWN PLATFORM"
#endif