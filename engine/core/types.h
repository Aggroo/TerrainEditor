#pragma once
/**
* @file         types.h
* @brief        Basic type defines 
* @details      Describes the basic types that we use in Vortex and defines types that might be platform specific.
* @date         28-02-2017
* @author       Fredrik Lindahl
* @copyright    See LICENCE file
*/
#include <cstddef>
#include <cstdint>

#if _MSC_VER
#define VORTEX_ALIGN16 __declspec(align(16))
#elif __GNUC__
#define __forceinline inline
#define VORTEX_ALIGN16 __attribute__((aligned(16)))
#define __cdecl //define to nothing
#define stricmp strcasecmp
#define strnicmp strncasecmp
#else
#define VORTEX_ALIGN16
#endif

#ifdef NULL
#undef NULL
#define NULL nullptr
#endif

//#include "foundation/memory/memory.h"

#define InvalidIndex -1

typedef uint64_t	uint64;
typedef uint32_t	uint32;
typedef int64_t		int64;
typedef int32_t		int32;
typedef uint16_t	uint16;
typedef int16_t		int16;
typedef uint8_t		uint8;
typedef int8_t		int8;
typedef uint8_t		uchar;
typedef size_t      index_t;

typedef uintptr_t   ptr_t;

typedef unsigned int	uint;
typedef unsigned short	ushort;

typedef uint8_t      byte;

typedef uint8_t		ubyte;
typedef float		float32;
typedef double		float64;
