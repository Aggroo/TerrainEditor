#pragma once
#ifndef MEMORY_LINUXMEMORYCONFIG_H
#define MEMORY_LINUXMEMORYCONFIG_H
/**
@file       linuxmemoryconfig.h
@date       02-03-2017
@copyright  See LICENCE file

@brief      Configurations for memory
*/

#include "config.h"

namespace Memory
{
    //-------------------------------
    /**
        @todo   Implement and use all the different heaps

        Heap types are defined here. The main purpose for the different heap
        types is to decrease memory fragmentation and to improve cache
        usage by grouping similar data together. Platform ports may define
        platform-specific heap types, as long as only platform specific
        code uses those new heap types.
        
        NOTE: These are not used at the moment but will probably be implemented in the future.
    */
    enum HeapType
    {
        ///General heap for objects that does not fit any other category
        DefaultHeap = 0,    
        ///Heap for global new allocator
        ObjectHeap,         
        ///Heap for global new[] allocator
        ObjectArrayHeap,    
        ///Heap for resource data (animation buffers, meshdata etc.)
        ResourceHeap,       
        ///For memory that only should exist for a short period of time (encode/decode buffers etc.)
        FrameHeap,          
        ///Heap for string data
        StringDataHeap,     
        ///Heap for stream data (memory streams, file streams, etc.)
        StreamDataHeap,     
        ///Heap for physics engine allocations
        PhysicsHeap,        
        ///Heap for general application layer data
        AppHeap,            
        ///Heap for our network layer
        NetworkHeap,        
        ///Heap for UI data
        UIHeap,             

        NumHeapTypes,
        InvalidHeapType
    };
}

#endif