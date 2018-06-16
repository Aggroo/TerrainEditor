#pragma once
/**
@class	Util::Dictionary

@brief	A list of key+value pairs
	
	Relatively quick value retrieval by key at roughly O(log n)
	Internally the dictionary is implemented as a sorted array.

@note	Insertion performance:
		Key/value pairs are inserted with the Add() method, which normally
		calls the Util::Array::InsertSorted() method internally. If many
		insertions are performed at once, it may be beneficial to call
		BeginBulkAdd() before, and EndBulkAdd() after adding the
		key/value pairs. Between BeginBulkAdd() and EndBulkAdd(), the
		Add() method will just append the new elements to the internal
		array, and only call Util::Array::Sort() inside EndBulkAdd().

		Any methods which require the internal array to be sorted will
		throw an assertion between BeginBulkAdd() and EndBulkAdd().

@copyright  See LICENCE file
*/

#include "core/types.h"
#include "array.h"
#include "pair.h"

namespace Util
{
template<class KEYTYPE, class VALUETYPE> class Dictionary
{
public:
    /// default constructor
    Dictionary();
    /// copy constructor
    Dictionary(const Dictionary<KEYTYPE, VALUETYPE>& rhs);
    /// assignment operator
    void operator=(const Dictionary<KEYTYPE, VALUETYPE>& rhs);
    /// read/write [] operator
    VALUETYPE& operator[](const KEYTYPE& key);
    /// read-only [] operator
    const VALUETYPE& operator[](const KEYTYPE& key) const;
    /// return number of key/value pairs in the dictionary
    size_t Size() const;
    /// clear the dictionary
    void Clear();
    /// return true if empty
    bool IsEmpty() const;
    /// reserve space (useful if number of elements is known beforehand)
    void Reserve(size_t numElements);
    /// begin a bulk insert (array will be sorted at End)
    void BeginBulkAdd();
    /// add a key/value pair
    void Add(const Pair<KEYTYPE, VALUETYPE>& kvp);
    /// add a key and associated value
    void Add(const KEYTYPE& key, const VALUETYPE& value);
    /// end a bulk insert (this will sort the internal array)
    void EndBulkAdd();
    /// erase a key and its associated value
    void Erase(const KEYTYPE& key);
    /// erase a key at index
    void EraseAtIndex(index_t index);
    /// find index of key/value pair (InvalidIndex if doesn't exist)
    index_t FindIndex(const KEYTYPE& key) const;
    /// return true if key exists in the array
    bool Contains(const KEYTYPE& key) const;
    /// get a key at given index
    const KEYTYPE& KeyAtIndex(index_t index) const;
    /// access to value at given index
    VALUETYPE& ValueAtIndex(index_t index);
    /// get a value at given index
    const VALUETYPE& ValueAtIndex(index_t index) const;
    /// get key/value pair at index
    Pair<KEYTYPE, VALUETYPE>& KeyValuePairAtIndex(index_t index) const;
    /// get all keys as an Util::Array
    Array<KEYTYPE> KeysAsArray() const;
    /// get all keys as an Util::Array
    Array<VALUETYPE> ValuesAsArray() const;
    /// get all keys as (typically) an array
    template<class RETURNTYPE> RETURNTYPE KeysAs() const;
    /// get all keys as (typically) an array
    template<class RETURNTYPE> RETURNTYPE ValuesAs() const;

protected:
    /// make sure the key value pair array is sorted
    void SortIfDirty() const;

    Array<Pair<KEYTYPE, VALUETYPE> > keyValuePairs;
    bool inBulkInsert;
};

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::Dictionary() :
    inBulkInsert(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::Dictionary(const Dictionary<KEYTYPE, VALUETYPE>& rhs) :
    keyValuePairs(rhs.keyValuePairs),
    inBulkInsert(false)
{
    //empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::operator=(const Dictionary<KEYTYPE, VALUETYPE>& rhs)
{
    this->keyValuePairs = rhs.keyValuePairs;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::Clear()
{
    this->keyValuePairs.Clear();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
size_t
Dictionary<KEYTYPE, VALUETYPE>::Size() const
{
    return this->keyValuePairs.Size();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
bool
Dictionary<KEYTYPE, VALUETYPE>::IsEmpty() const
{
    return (0 == this->keyValuePairs.Size());
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::Add(const Pair<KEYTYPE, VALUETYPE>& kvp)
{
    if (this->inBulkInsert)
    {
        this->keyValuePairs.Append(kvp);
    }
    else
    {
        this->keyValuePairs.InsertSorted(kvp);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::Reserve(size_t numElements)
{
    this->keyValuePairs.Reserve(numElements);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::BeginBulkAdd()
{
    this->inBulkInsert = true;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::EndBulkAdd()
{
    this->keyValuePairs.Sort();
    this->inBulkInsert = false;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::Add(const KEYTYPE& key, const VALUETYPE& value)
{
    Pair<KEYTYPE, VALUETYPE> kvp(key, value);
    if (this->inBulkInsert)
    {
        this->keyValuePairs.Append(kvp);
    }
    else
    {
        this->keyValuePairs.InsertSorted(kvp);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::Erase(const KEYTYPE& key)
{
    index_t eraseIndex = this->keyValuePairs.BinarySearchIndex(key);
    this->keyValuePairs.EraseIndex(eraseIndex);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
void
Dictionary<KEYTYPE, VALUETYPE>::EraseAtIndex(index_t index)
{
    this->keyValuePairs.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
index_t
Dictionary<KEYTYPE, VALUETYPE>::FindIndex(const KEYTYPE& key) const
{
    return this->keyValuePairs.BinarySearchIndex(key);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
bool
Dictionary<KEYTYPE, VALUETYPE>::Contains(const KEYTYPE& key) const
{
    return (InvalidIndex != this->keyValuePairs.BinarySearchIndex(key));
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
const KEYTYPE&
Dictionary<KEYTYPE, VALUETYPE>::KeyAtIndex(index_t index) const
{
    return this->keyValuePairs[index].Key();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::ValueAtIndex(index_t index)
{
    return this->keyValuePairs[index].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
const VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::ValueAtIndex(index_t index) const
{
    return this->keyValuePairs[index].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
Pair<KEYTYPE, VALUETYPE>&
Dictionary<KEYTYPE, VALUETYPE>::KeyValuePairAtIndex(index_t index) const
{
    return this->keyValuePairs[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key)
{
    int keyValuePairIndex = (int)this->FindIndex(key);
    return this->keyValuePairs[keyValuePairIndex].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
const VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key) const
{
    int keyValuePairIndex = (int)this->FindIndex(key);
    return this->keyValuePairs[keyValuePairIndex].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
template<class RETURNTYPE>
RETURNTYPE
Dictionary<KEYTYPE, VALUETYPE>::ValuesAs() const
{
    RETURNTYPE result(this->Size(),this->Size());
    index_t i;
    for (i = 0; i < this->keyValuePairs.Size(); i++)
    {
        result.Append(this->keyValuePairs[i].Value());
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Array<VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::ValuesAsArray() const
{
    return this->ValuesAs<Array<VALUETYPE>>();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> 
template<class RETURNTYPE>
RETURNTYPE
Dictionary<KEYTYPE, VALUETYPE>::KeysAs() const
{
    RETURNTYPE result(this->Size(),this->Size());
    index_t i;
    for (i = 0; i < this->keyValuePairs.Size(); i++)
    {
        result.Append(this->keyValuePairs[i].Key());
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Array<KEYTYPE>
Dictionary<KEYTYPE, VALUETYPE>::KeysAsArray() const
{
    return this->KeysAs<Array<KEYTYPE>>();
}

} // namespace Util
