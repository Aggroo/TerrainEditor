#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::Factory

    Provides the central object factory mechanism for Vortex, based on Nebula3's factory. 
	Classes which are derived from RefCounted register themselves automatically
    to the central Factory object through the __DeclareClass and
    __ImplementClass macros.


    @copyright	See LICENCE file
*/
#include "foundation/util/array.h"
#include "foundation/util/string.h"
#include "foundation/util/fourcc.h"
#include "foundation/util/dictionary.h"
#include "foundation/util/hashtable.h"
#include "core/ptr.h"

//------------------------------------------------------------------------------
namespace Core 
{
class RefCounted;
class Rtti;

//------------------------------------------------------------------------------
class Factory
{
public:
    /// get pointer to singleton instance (cannot use singleton.h!)
    static Factory* Instance();
    /// static instance destruction method
    static void Destroy();

    /// register a RTTI object with the factory (with class name and class fourcc code)
    void Register(const Rtti* rtti, const Util::String& className, const Util::FourCC& classFourCC);
    /// register a RTTI object with the factory (without fourcc code)
    void Register(const Rtti* rtti, const Util::String& className);
    /// check if a class exists by class name
    bool ClassExists(const Util::String& className) const;
    /// check if a class exists by FourCC code
    bool ClassExists(const Util::FourCC classFourCC) const;
    /// get class rtti object by name
    const Rtti* GetClassRtti(const Util::String& className) const;
    /// get class rtti object by fourcc code
    const Rtti* GetClassRtti(const Util::FourCC& classFourCC) const;
    /// create an object by class name
    RefCounted* Create(const Util::String& className) const;
    /// create an object by FourCC code
    RefCounted* Create(const Util::FourCC classFourCC) const;

private:
    /// constructor is private
    Factory();
    /// destructor is private
    ~Factory();

    static Factory* Singleton;
    Util::HashTable<Util::String, const Rtti*> nameTable;		// for fast lookup by class name
    Util::Dictionary<Util::FourCC, const Rtti*> fourccTable;	// for fast lookup by fourcc code
};

} // namespace Foundation
//------------------------------------------------------------------------------
