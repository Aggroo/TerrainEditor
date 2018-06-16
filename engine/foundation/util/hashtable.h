#pragma once
/**
@class	Util::HashTable

@brief	Organizes key/value pairs by a hash code.
	
	Looks very similar to a Dictionary, but may provide better search times (up to O(1))
    by computing a (ideally unique) hash code on the key and using that as an
    index into an array. The flipside is that the key class must provide
    a hash code and the memory footprint may be larger then Dictionary.
    
    The default capacity is 128. Matching the capacity against the number
    of expected elements in the hash table is one key to get optimal 
    insertion and search times, the other is to provide a good (and fast) 
    hash code computation which produces as few collissions as possible 
    for the key type.

    The key class must implement the following method in order to
    work with the HashTable:    
    index_t HashCode() const;

    The Util::String class implements this method as an example.
    Internally the hash table is implemented as a fixed array
    of sorted arrays. The fixed array is indexed by the hash code
    of the key, the sorted arrays contain all values with identical
    keys.
	
@copyright  See LICENCE file
*/

#include "fixedarray.h"
#include "array.h"
#include "pair.h"

//------------------------------------------------------------------------------
namespace Util
{
template<class KEYTYPE, class VALUETYPE> class HashTable
{
public:
    /// default constructor
    HashTable();
    /// constructor with capacity
    HashTable(size_t capacity);
    /// copy constructor
    HashTable(const HashTable<KEYTYPE, VALUETYPE>& rhs);
    /// assignment operator
    void operator=(const HashTable<KEYTYPE, VALUETYPE>& rhs);
    /// read/write [] operator, assertion if key not found
    VALUETYPE& operator[](const KEYTYPE& key) const;
    /// return current number of values in the hashtable
	size_t Size() const;
    /// return fixed-size capacity of the hash table
	size_t Capacity() const;
    /// clear the hashtable
    void Clear();
    /// return true if empty
    bool IsEmpty() const;
    /// add a key/value pair object to the hash table
    void Add(const Pair<KEYTYPE, VALUETYPE>& kvp);
    /// add a key and associated value
    void Add(const KEYTYPE& key, const VALUETYPE& value);
    /// erase an entry
    void Erase(const KEYTYPE& key);
    /// return true if key exists in the array
    bool Contains(const KEYTYPE& key) const;
    /// return array of all key/value pairs in the table (slow)
    Array<Pair<KEYTYPE, VALUETYPE> > Content() const;
	/// get all keys as an Util::Array (slow)
	Array<KEYTYPE> KeysAsArray() const;
	/// get all keys as an Util::Array (slow)
	Array<VALUETYPE> ValuesAsArray() const;
private:
    FixedArray<Array<Pair<KEYTYPE, VALUETYPE>>> hashArray;
	size_t size;
};

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Array<KEYTYPE>
Util::HashTable<KEYTYPE, VALUETYPE>::KeysAsArray() const
{
	Util::Array<KEYTYPE> keys;
	for (int i = 0; i < this->hashArray.Size(); i++)
	{
		for (int j = 0; j < this->hashArray[i].Size();j++)
		{ 
			keys.Append(this->hashArray[i][j].Key());
		}
	}
	return keys;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Array<VALUETYPE>
Util::HashTable<KEYTYPE, VALUETYPE>::ValuesAsArray() const
{
	Util::Array<VALUETYPE> vals;
	for (int i = 0; i < this->hashArray.Size(); i++)
	{
		for (int j = 0; j < this->hashArray[i].Size(); j++)
		{
			vals.Append(this->hashArray[i][j].Value());
		}
	}
	return vals;
}
//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
HashTable<KEYTYPE, VALUETYPE>::HashTable() :
    hashArray(128),
    size(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
HashTable<KEYTYPE, VALUETYPE>::HashTable(size_t capacity) :
    hashArray(capacity),
    size(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
HashTable<KEYTYPE, VALUETYPE>::HashTable(const HashTable<KEYTYPE, VALUETYPE>& rhs) :
    hashArray(rhs.hashArray),
    size(rhs.size)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
void
HashTable<KEYTYPE, VALUETYPE>::operator=(const HashTable<KEYTYPE, VALUETYPE>& rhs)
{
    if (this != &rhs)
    {
        this->hashArray = rhs.hashArray;
        this->size = rhs.size;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
VALUETYPE&
HashTable<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key) const
{
    // get hash code from key, trim to capacity
    int hashIndex = key.HashCode() % (int)this->hashArray.Size();
    const Array<Pair<KEYTYPE, VALUETYPE> >& hashElements = this->hashArray[hashIndex];
    int numHashElements = (int)hashElements.Size();
    if (1 == numHashElements)
    {
        // no hash collissions, just return the only existing element
        return hashElements[0].Value();
    }
    else
    {
        // here's a hash collision, find the right key
        // with a binary search
        index_t hashElementIndex = hashElements.BinarySearchIndex(key);
        return hashElements[hashElementIndex].Value();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
size_t
HashTable<KEYTYPE, VALUETYPE>::Size() const
{
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
size_t
HashTable<KEYTYPE, VALUETYPE>::Capacity() const
{
    return this->hashArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
void
HashTable<KEYTYPE, VALUETYPE>::Clear()
{
    int i;
    int num = (int)this->hashArray.Size();
    for (i = 0; i < num; i++)
    {
        this->hashArray[i].Clear();
    }
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
HashTable<KEYTYPE, VALUETYPE>::IsEmpty() const
{
    return (0 == this->size);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
void
HashTable<KEYTYPE, VALUETYPE>::Add(const Pair<KEYTYPE, VALUETYPE>& kvp)
{
    int hashIndex = kvp.Key().HashCode() % (int)this->hashArray.Size();
    this->hashArray[hashIndex].InsertSorted(kvp);
    this->size++;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
void
HashTable<KEYTYPE, VALUETYPE>::Add(const KEYTYPE& key, const VALUETYPE& value)
{
    Pair<KEYTYPE, VALUETYPE> kvp(key, value);
    this->Add(kvp);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
void
HashTable<KEYTYPE, VALUETYPE>::Erase(const KEYTYPE& key)
{
    index_t hashIndex = key.HashCode() % this->hashArray.Size();
    Array<Pair<KEYTYPE, VALUETYPE> >& hashElements = this->hashArray[hashIndex];
    index_t hashElementIndex = hashElements.BinarySearchIndex(key);
    hashElements.EraseIndex(hashElementIndex);
    this->size--;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
bool
HashTable<KEYTYPE, VALUETYPE>::Contains(const KEYTYPE& key) const
{
    if (this->size > 0)
    {
        int hashIndex = key.HashCode() % (int)this->hashArray.Size();
        Array<Pair<KEYTYPE, VALUETYPE> >& hashElements = this->hashArray[hashIndex];
        index_t hashElementIndex = hashElements.BinarySearchIndex(key);
        return (InvalidIndex != hashElementIndex);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Array<Pair<KEYTYPE, VALUETYPE> >
HashTable<KEYTYPE, VALUETYPE>::Content() const
{
    Array<Pair<KEYTYPE, VALUETYPE> > result;
    int i;
    int num = this->hashArray.Size();
    for (i = 0; i < num; i++)
    {
        if (this->hashArray[i].Size() > 0)
        {
            result.AppendArray(this->hashArray[i]);
        }
    }
    return result;
}

} // namespace Util
