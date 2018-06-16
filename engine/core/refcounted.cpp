//------------------------------------------------------------------------------
//  refcounted.cc
//  (C) 2006 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "config.h"
#include "core/refcounted.h"

namespace Core
{
__ImplementRootClass(Core::RefCounted, 'REFC');

#if NEBULA3_DEBUG
using namespace Util;

Threading::CriticalSection RefCounted::criticalSection;
ThreadLocal bool RefCounted::isInCreate = false;
RefCountedList RefCounted::list;

#endif

//------------------------------------------------------------------------------
/**
*/
RefCounted::~RefCounted()
{
    _assert(0 == this->refCount);
    //#if _DEBUG
    //    _assert(!this->destroyed);
    //    _assert(0 != this->listIterator);
    //    criticalSection.Enter();
    //    list.Remove(this->listIterator);
    //    criticalSection.Leave();
    //    this->listIterator = 0;
    //    this->destroyed = true;
    //#endif
}

//------------------------------------------------------------------------------
/**
    This method should be called as the very last before an application 
    exits.
*/
void
RefCounted::DumpRefCountingLeaks()
{
    //#if _DEBUG
    //list.DumpLeaks();    
    //#endif
}

//------------------------------------------------------------------------------
/**
    Gather per-class stats.
*/
//#if _DEBUG
//Dictionary<String,RefCounted::Stats>
//RefCounted::GetOverallStats()
//{
//    Dictionary<String,Stats> result;
//    criticalSection.Enter();
//    // iterate over refcounted list
//    RefCountedList::Iterator iter;
//    for (iter = list.Begin(); iter != list.End(); iter++)
//    {
//        RefCounted* cur = *iter;
//        if (!result.Contains(cur->GetClassName()))
//        {
//            Stats newStats;
//            newStats.className   = cur->GetClassName();
//            newStats.classFourCC = cur->GetClassFourCC();
//            newStats.numObjects  = 1;
//            newStats.overallRefCount = cur->GetRefCount();
//            newStats.instanceSize = cur->GetRtti()->GetInstanceSize();
//            result.Add(cur->GetClassName(), newStats);
//        }
//        else
//        {
//            Stats& stats = result[cur->GetClassName()];
//            stats.numObjects++;
//            stats.overallRefCount += cur->GetRefCount();
//        }
//    }
//    criticalSection.Leave();
//    return result;
//}
//#endif

} // namespace Foundation