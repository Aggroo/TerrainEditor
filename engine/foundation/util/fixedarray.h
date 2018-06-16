#pragma once
#include "foundation/util/array.h"

namespace Util
{
template<class TYPE> 
class FixedArray
{
public:
    /// define element iterator
    typedef TYPE* Iterator;

    /// default constructor
    FixedArray();
    /// constructor with size
    FixedArray(size_t s);
    /// constructor with size and initial value
    FixedArray(size_t s, const TYPE& initialValue);
    /// copy constructor
    FixedArray(const FixedArray<TYPE>& rhs);
    /// destructor
    ~FixedArray();
    /// assignment operator
    void operator=(const FixedArray<TYPE>& rhs);
    /// write [] operator
    TYPE& operator[](uint index) const;
    /// equality operator
    bool operator==(const FixedArray<TYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const FixedArray<TYPE>& rhs) const;

    /// set number of elements (clears existing content)
    void SetSize(size_t s);
    /// get number of elements
    size_t Size() const;
    /// resize array without deleting existing content
    void Resize(size_t newSize);
    /// return true if array if empty (has no elements)
    bool IsEmpty() const;
    /// clear the array, free elements
    void Clear();
    /// fill the entire array with a value
    void Fill(const TYPE& val);
    /// fill array range with element
    void Fill(uint first, size_t num, const TYPE& val);
    /// get iterator to first element
    Iterator Begin() const;
    /// get iterator past last element
    Iterator End() const;
    /// find identical element in unsorted array (slow)
    Iterator Find(const TYPE& val) const;
    /// find index of identical element in unsorted array (slow)
    uint FindIndex(const TYPE& val) const;
    /// sort the array
    void Sort();
    /// do a binary search, requires a sorted array
    uint BinarySearchIndex(const TYPE& val) const;
    /// return content as Array (slow!)
    Array<TYPE> AsArray() const;

private:
    /// delete content
    void Delete();
    /// allocate array for given size
    void Alloc(size_t s);
    /// copy content
    void Copy(const FixedArray<TYPE>& src);

    size_t size;
    TYPE* elements;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
FixedArray<TYPE>::FixedArray() :
    size(0),
    elements(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Delete()
{
    if (this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Alloc(size_t s)
{
    if (s > 0)
    {
        this->elements = new TYPE[s];
    }
    this->size = s;
}

//------------------------------------------------------------------------------
/**
    NOTE: only works on deleted array. This is intended.
*/
template<class TYPE> void
FixedArray<TYPE>::Copy(const FixedArray<TYPE>& rhs)
{
    if (this != &rhs)
    {
        this->Alloc(rhs.size);
        uint i;
        for (i = 0; i < this->size; i++)
        {
            this->elements[i] = rhs.elements[i];
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
FixedArray<TYPE>::FixedArray(size_t s) :
    size(0),
    elements(0)
{
    this->Alloc(s);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
FixedArray<TYPE>::FixedArray(size_t s, const TYPE& initialValue) :
    size(0),
    elements(0)
{
    this->Alloc(s);
    this->Fill(initialValue);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
FixedArray<TYPE>::FixedArray(const FixedArray<TYPE>& rhs) :
    size(0),
    elements(0)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
FixedArray<TYPE>::~FixedArray()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::operator=(const FixedArray<TYPE>& rhs)
{
    this->Delete();
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE&
FixedArray<TYPE>::operator[](uint index) const
{
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
FixedArray<TYPE>::operator==(const FixedArray<TYPE>& rhs) const
{
    if (this->size != rhs.size)
    {
        return false;
    }
    else
    {
        #if NEBULA3_BOUNDSCHECKS
        n_assert(this->elements && rhs.elements);
        #endif
        uint i;
        size_t num = this->size;
        for (i = 0; i < num; i++)
        {
            if (this->elements[i] != rhs.elements[i])
            {
                return false;
            }
        }
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
FixedArray<TYPE>::operator!=(const FixedArray<TYPE>& rhs) const
{
    return !(*this == rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::SetSize(size_t s)
{
    this->Delete();
    this->Alloc(s);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Resize(size_t newSize)
{
    // allocate new array and copy over old elements
    TYPE* newElements = 0;
    if (newSize > 0)
    {
        newElements = new TYPE[newSize];
        size_t numCopy = this->size;
        if (numCopy > newSize) numCopy = newSize;
        uint i;
        for (i = 0; i < numCopy; i++)
        {
            newElements[i] = this->elements[i];
        }
    }

    // delete old elements
    this->Delete();

    // set content to new elements
    this->elements = newElements;
    this->size = newSize;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> size_t
FixedArray<TYPE>::Size() const
{
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
FixedArray<TYPE>::IsEmpty() const
{
    return 0 == this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Clear()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Fill(const TYPE& val)
{
    uint i;
    for (i = 0; i < this->size; i++)
    {
        this->elements[i] = val;
    }
}       

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Fill(uint first, size_t num, const TYPE& val)
{
    uint i;
    for (i = first; i < (first + num); i++)
    {
        this->elements[i] = val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename FixedArray<TYPE>::Iterator
FixedArray<TYPE>::Begin() const
{
    return this->elements;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename FixedArray<TYPE>::Iterator
FixedArray<TYPE>::End() const
{
    return this->elements + this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename FixedArray<TYPE>::Iterator
FixedArray<TYPE>::Find(const TYPE& elm) const
{
    uint i;
    for (i = 0; i < this->size; i++)
    {
        if (elm == this->elements[i])
        {
            return &(this->elements[i]);
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> uint
FixedArray<TYPE>::FindIndex(const TYPE& elm) const
{
    uint i;
    for (i = 0; i < this->size; i++)
    {
        if (elm == this->elements[i])
        {
            return i;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
FixedArray<TYPE>::Sort()
{
    std::sort(this->Begin(), this->End());
}

//------------------------------------------------------------------------------
/**
    @todo hmm, this is copy-pasted from Array...
*/
template<class TYPE> uint
FixedArray<TYPE>::BinarySearchIndex(const TYPE& elm) const
{
    size_t num = this->Size();
    if (num > 0)
    {
        uint half;
        uint lo = 0;
	    uint hi = num - 1;
	    uint mid;
        while (lo <= hi) 
        {
            if (0 != (half = num/2)) 
            {
                mid = lo + ((num & 1) ? half : (half - 1));
                if (elm < this->elements[mid])
                {
                    hi = mid - 1;
                    num = num & 1 ? half : half - 1;
                } 
                else if (elm > this->elements[mid]) 
                {
                    lo = mid + 1;
                    num = half;
                } 
                else
                {
                    return mid;
                }
            } 
            else if (num) 
            {
                if (elm != this->elements[lo])
                {
                    return InvalidIndex;
                }
                else      
                {
                    return lo;
                }
            } 
            else 
            {
                break;
            }
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> Array<TYPE>
FixedArray<TYPE>::AsArray() const
{
    Array<TYPE> result;
    result.Reserve(this->size);
    uint i;
    for (i = 0; i < this->size; i++)
    {
        result.Append(this->elements[i]);
    }
    return result;
}

}