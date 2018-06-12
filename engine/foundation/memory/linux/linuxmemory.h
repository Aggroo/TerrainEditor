#pragma once
#ifndef MEMORY_LINUXMEMORY_H
#define MEMORY_LINUXMEMORY_H
/**
@file       linuxmemory.h
@date       02-03-2017
@copyright  See LICENCE file

@todo       Implement the different heaps defined in memoryconfig file.

@brief      Memory subsystem for linux platform

@see        linuxmemoryconfig.h
*/
#include "config.h"
#include "core/types.h"
#include "core/debug.h"
#include "foundation/memory/linux/linuxmemoryconfig.h"
#include <malloc.h>
#include <cstdlib>
#include <cstring>

namespace Memory
{

#if VORTEX_MEMORY_STATS
//Code for memory statistics
#endif

//----------------------------------------------------
/**
    Allocate a block of memory from the process heap.
    This always aligns the memory to be 16 byte aligned
*/
__forceinline void* Alloc(HeapType heapType, size_t size)
{
    assert(heapType < NumHeapTypes);

    void* allocPtr = nullptr;
    {
        allocPtr = memalign(16, size);
    }

    return allocPtr;
}

//-------------------------------------------------------
/**
    Reallocate a block of memory
*/
__forceinline void* Realloc(HeapType heapType, void* ptr, size_t size)
{
    assert(heapType < NumHeapTypes);
    
    void* allocPtr = realloc(ptr, size);

    return allocPtr;
}

//-----------------------------------------------------
/**
    Free a block of memory from the process heap
*/
__forceinline void Free(HeapType heapType, void* ptr)
{
    if(ptr != nullptr)
    {
        assert(heapType < NumHeapTypes);
        
        {
            free(ptr);
        }
    }
}

//-------------------------------------------------------
/**
    Copy a block of memory
*/
__forceinline void Copy(void* to, const void* from, size_t numBytes)
{
    if(numBytes > 0)
    {
        assert(from != nullptr);
        assert(to != nullptr);
        assert(from != to);
        memcpy(to, from, numBytes);
    }
}

//---------------------------------------------------------
/**
    Overwrites a block of memory with 0's
*/
__forceinline void Clear(void* ptr, size_t numBytes)
{
    memset(ptr, 0, numBytes);
}

//---------------------------------------------------------
/**
    Fills a block of memory with the specified byte value.
*/
__forceinline void Fill(void* ptr, size_t numBytes, unsigned char value)
{
    memset(ptr, value, numBytes);
}

//------------------------------------------------------
/**
    Duplicates a null-terminated string. 
    The memory will be allocated from the StringHeap (important when freeing the memory!)
*/
__forceinline char* DuplicateCStr(const char* from)
{
    assert(from != nullptr);
    size_t len = (uint) strlen(from) + 1;
    char* to = (char*) Memory::Alloc(Memory::StringDataHeap, len);
    Memory::Copy((void*)from, to, len);
    return to;
}

//--------------------------------------------------------
/**
    Test if two blocks of memory are overlapping
*/
__forceinline bool MemIsOverlapping(const unsigned char* srcPtr,
                                    size_t srcSize,
                                    const unsigned char* dstPtr,
                                    size_t dstSize)
{
    if (srcPtr == dstPtr)
    {
        return true;
    }
    else if (srcPtr > dstPtr)
    {
        return (srcPtr + srcSize) > dstPtr;
    }
    else
    {
        return (dstPtr + dstSize) > srcPtr;
    }
}

} //namespace Memory

#ifdef new
#undef new
#endif

#ifdef delete
#undef delete
#endif

//---------------------------------------------------------
/**
    Replacement of the global new operator
*/
__forceinline void* operator new(size_t size)
{
    return Memory::Alloc(Memory::ObjectHeap, size);
}

//---------------------------------------------------------
/**
    Replacement of the global new[] operator
*/
__forceinline void* operator new[](size_t size)
{
    return Memory::Alloc(Memory::ObjectArrayHeap, size);
}

//---------------------------------------------------------
/**
    Replacement of the global delete operator
*/
__forceinline void operator delete(void* p)
{
    Memory::Free(Memory::ObjectHeap, p);
}

//---------------------------------------------------------
/**
    Replacement of the global delete[] operator
*/
__forceinline void operator delete[](void* p)
{
    Memory::Free(Memory::ObjectHeap, p);
}

#endif