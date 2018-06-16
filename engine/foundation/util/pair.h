#pragma once
/**
@class	Util::Pair
    
@brief A Key+Value Pair

    Key/Value pair objects are used by most assiociative container classes,
    like Dictionary or HashTable. 
    
@copyright  See LICENCE file
*/

//------------------------------------------------------------------------------
namespace Util
{
template<class KEYTYPE, class VALUETYPE> class Pair
{
public:
    /// default constructor
    Pair();
    /// constructor with key and value
	Pair(const KEYTYPE& k, const VALUETYPE& v);
    /// constructor with key and undefined value
	Pair(const KEYTYPE& k);
    /// copy constructor
	Pair(const Pair<KEYTYPE, VALUETYPE>& rhs);
    /// assignment operator
    void operator=(const Pair<KEYTYPE, VALUETYPE>& rhs);
    /// equality operator
    bool operator==(const Pair<KEYTYPE, VALUETYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const Pair<KEYTYPE, VALUETYPE>& rhs) const;
    /// greater operator
    bool operator>(const Pair<KEYTYPE, VALUETYPE>& rhs) const;
    /// greater-or-equal operator
    bool operator>=(const Pair<KEYTYPE, VALUETYPE>& rhs) const;
    /// lesser operator
    bool operator<(const Pair<KEYTYPE, VALUETYPE>& rhs) const;
    /// lesser-or-equal operator
    bool operator<=(const Pair<KEYTYPE, VALUETYPE>& rhs) const;
    /// read/write access to value
    VALUETYPE& Value();
    /// read access to key
    const KEYTYPE& Key() const;
    /// read access to key
    const VALUETYPE& Value() const;

protected:
    KEYTYPE keyData;
    VALUETYPE valueData;
};

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Pair<KEYTYPE, VALUETYPE>::Pair()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Pair<KEYTYPE, VALUETYPE>::Pair(const KEYTYPE& k, const VALUETYPE& v) :
    keyData(k),
    valueData(v)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This strange constructor is useful for search-by-key if
    the key-value-pairs are stored in a Util::Array.
*/
template<class KEYTYPE, class VALUETYPE>
Pair<KEYTYPE, VALUETYPE>::Pair(const KEYTYPE& k) :
    keyData(k)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Pair<KEYTYPE, VALUETYPE>::Pair(const Pair<KEYTYPE, VALUETYPE>& rhs) :
    keyData(rhs.keyData),
    valueData(rhs.valueData)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
void
Pair<KEYTYPE, VALUETYPE>::operator=(const Pair<KEYTYPE, VALUETYPE>& rhs)
{
    this->keyData = rhs.keyData;
    this->valueData = rhs.valueData;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
Pair<KEYTYPE, VALUETYPE>::operator==(const Pair<KEYTYPE, VALUETYPE>& rhs) const
{
    return (this->keyData == rhs.keyData);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
Pair<KEYTYPE, VALUETYPE>::operator!=(const Pair<KEYTYPE, VALUETYPE>& rhs) const
{
    return (this->keyData != rhs.keyData);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
Pair<KEYTYPE, VALUETYPE>::operator>(const Pair<KEYTYPE, VALUETYPE>& rhs) const
{
    return (this->keyData > rhs.keyData);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
Pair<KEYTYPE, VALUETYPE>::operator>=(const Pair<KEYTYPE, VALUETYPE>& rhs) const
{
    return (this->keyData >= rhs.keyData);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
Pair<KEYTYPE, VALUETYPE>::operator<(const Pair<KEYTYPE, VALUETYPE>& rhs) const
{
    return (this->keyData < rhs.keyData);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
Pair<KEYTYPE, VALUETYPE>::operator<=(const Pair<KEYTYPE, VALUETYPE>& rhs) const
{
    return (this->keyData <= rhs.keyData);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
VALUETYPE&
Pair<KEYTYPE, VALUETYPE>::Value()
{
    return this->valueData;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
const KEYTYPE&
Pair<KEYTYPE, VALUETYPE>::Key() const
{
    return this->keyData;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
const VALUETYPE&
Pair<KEYTYPE, VALUETYPE>::Value() const
{
    return this->valueData;
}

} // namespace Util
//------------------------------------------------------------------------------
    