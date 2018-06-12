#pragma once
#include <stddef.h>
#include <algorithm>
#include <cassert>

#ifndef InvalidIndex
#define InvalidIndex -1
#endif

namespace Util
{
  
template<class TYPE>
class Array
{
public:
    /// define iterator
    typedef TYPE* Iterator;

    /// constructor with default parameters
    Array();
    /// constuctor with initial size and grow size
    Array(size_t initialCapacity, size_t initialGrow);
    /// constructor with initial size, grow size and initial values
    Array(size_t initialSize, size_t initialGrow, const TYPE& initialValue);
    /// copy constructor
    Array(const Array<TYPE>& rhs);
    /// destructor
    ~Array();

    /// assignment operator
    void operator=(const Array<TYPE>& rhs);
    /// [] operator
    TYPE& operator[](const size_t& index) const;
    /// equality operator
    bool operator==(const Array<TYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const Array<TYPE>& rhs) const;
    /// convert to "anything"
    template<typename T> T As() const;

    /// append element to end of array
    void Append(const TYPE& elm);
    /// append the contents of an array to this array
    void AppendArray(const Array<TYPE>& rhs);
    /// increase capacity to fit N more elements into the array
    void Reserve(size_t num);
    /// get number of elements in array
    size_t Size() const;
    /// get overall allocated size of array in number of elements
    size_t Capacity() const;
    /// return reference to first element
    TYPE& Front() const;
    /// return reference to last element
    TYPE& Back() const;
    /// return true if array empty
    bool IsEmpty() const;

    /// erase element at index, keep sorting intact. NOTE: Calls destructor. See Remove-functions for not calling destructors.
    void EraseIndex(size_t index);
    /// erase element pointed to by iterator, keep sorting intact. NOTE: Calls destructor. See Remove-functions for not calling destructors.
    Iterator Erase(Iterator iter);
    /// erase element at index, fill gap by swapping in last element, destroys sorting!. NOTE: Calls destructor. See Remove-functions for not calling destructors.
    void EraseIndexSwap(size_t index);
    /// erase element at iterator, fill gap by swapping in last element, destroys sorting!. NOTE: Calls destructor. See Remove-functions for not calling destructors.
    Iterator EraseSwap(Iterator iter);

	/// remove element at index, keep sorting intact. NOTE: Does not call destructor. See Erase-functions for calling destructors.
	void RemoveIndex(size_t index);
	/// remove element pointed to by iterator, keep sorting intact. NOTE: Does not call destructor. See Erase-functions for calling destructors.
	Iterator Remove(Iterator iter);
	/// remove element at index, fill gap by swapping in last element, destroys sorting!. NOTE: Does not call destructor. See Erase-functions for calling destructors.
	void RemoveIndexSwap(size_t index);
	/// remove element at iterator, fill gap by swapping in last element, destroys sorting!. NOTE: Does not call destructor. See Erase-functions for calling destructors.
	Iterator RemoveSwap(Iterator iter);

    /// insert element before element at index
    void Insert(size_t index, const TYPE& elm);
    /// insert element into sorted array, return index where element was included
    size_t InsertSorted(const TYPE& elm);
    /// insert element at the first non-identical position, return index of inclusion position
	size_t InsertAtEndOfIdenticalRange(size_t startIndex, const TYPE& elm);
    /// test if the array is sorted, this is a slow operation!
    bool IsSorted() const;
    /// clear array (calls destructors)
    void Clear();
    /// reset array (does NOT call destructors)
    void Reset();
    /// return iterator to beginning of array
    Iterator begin() const;
    /// return iterator to end of array
    Iterator end() const;
    /// find identical element in array, return iterator
    Iterator Find(const TYPE& elm) const;
    /// find identical element in array, return index, InvalidIndex if not found
	size_t FindIndex(const TYPE& elm) const;
    /// fill array range with element
	void Fill(size_t first, size_t num, const TYPE& elm);
    /// clear contents and preallocate with new attributes
    void Realloc(size_t capacity, size_t grow);
    /// returns new array with elements which are not in rhs (slow!)
    Array<TYPE> Difference(const Array<TYPE>& rhs);
    /// sort the array
    void Sort();
	/// sort with custom function
	void SortWithFunc(bool (*func)(const TYPE& lhs, const TYPE& rhs));
    /// do a binary search, requires a sorted array
	size_t BinarySearchIndex(const TYPE& elm) const;

private:
    /// destroy an element (call destructor without freeing memory)
    void Destroy(TYPE* elm);
    /// copy content
    void Copy(const Array<TYPE>& src);
    /// delete content
    void Delete();
    /// grow array
    void Grow();
    /// grow array to target size
    void GrowTo(size_t newCapacity);
    /// move elements, grows array if needed
    void Move(size_t fromIndex, size_t toIndex);

    static const size_t MinGrowSize = 16;
    static const size_t MaxGrowSize = 65536; // FIXME: big grow size needed for mesh tools
    size_t grow;                             // grow by this number of elements if array exhausted
    size_t capacity;                         // number of elements allocated
    size_t size;                             // number of elements in array
    TYPE* elements;                         // pointer to element array
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array() :
    grow(8),
    capacity(0),
    size(0),
    elements(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array(size_t _capacity, size_t _grow) :
    grow(_grow),
    capacity(_capacity),
    size(0)
{
    if (0 == this->grow)
    {
        this->grow = 16;
    }
    if (this->capacity > 0)
    {
        this->elements = new TYPE[this->capacity];
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array(size_t initialSize, size_t _grow, const TYPE& initialValue) :
    grow(_grow),
    capacity(initialSize),
    size(initialSize)
{
    if (0 == this->grow)
    {
        this->grow = 16;
    }
    if (initialSize > 0)
    {
        this->elements = new TYPE[this->capacity];
		size_t i;
        for (i = 0; i < initialSize; i++)
        {
            this->elements[i] = initialValue;
        }
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Copy(const Array<TYPE>& src)
{

    this->grow = src.grow;
    this->capacity = src.capacity;
    this->size = src.size;
    if (this->capacity > 0)
    {
        this->elements = new TYPE[this->capacity];
		size_t i;
        for (i = 0; i < this->size; i++)
        {
            this->elements[i] = src.elements[i];
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Delete()
{
    this->grow = 0;
    this->capacity = 0;
    this->size = 0;
    if (this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Destroy(TYPE* elm)
{
    elm->~TYPE();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array(const Array<TYPE>& rhs) :
    grow(0),
    capacity(0),
    size(0),
    elements(0)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::~Array()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Realloc(size_t _capacity, size_t _grow)
{
    this->Delete();
    this->grow = _grow;
    this->capacity = _capacity;
    this->size = 0;
    if (this->capacity > 0)
    {
        this->elements = new TYPE[this->capacity];
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void 
Array<TYPE>::operator=(const Array<TYPE>& rhs)
{
    if (this != &rhs)
    {
        if ((this->capacity > 0) && (rhs.size <= this->capacity))
        {
            // source array fits into our capacity, copy in place
            assert(0 != this->elements);
			size_t i;
            for (i = 0; i < rhs.size; i++)
            {
                this->elements[i] = rhs.elements[i];
            }

            // properly destroy remaining original elements
            for (; i < this->size; i++)
            {
                this->Destroy(&(this->elements[i]));
            }
            this->grow = rhs.grow;
            this->size = rhs.size;
        }
        else
        {
            // source array doesn't fit into our capacity, need to reallocate
            this->Delete();
            this->Copy(rhs);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::GrowTo(size_t newCapacity)
{
    TYPE* newArray = new TYPE[newCapacity];
    if (this->elements)
    {
        // copy over contents
		size_t i;
        for (i = 0; i < this->size; i++)
        {
            newArray[i] = this->elements[i];
        }

        // discard old array and update contents
        delete[] this->elements;
    }
    this->elements  = newArray;
    this->capacity = newCapacity;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Grow()
{
    size_t growToSize;
    if (0 == this->capacity)
    {
        growToSize = this->grow;
    }
    else
    {
        // grow by half of the current capacity, but never more then MaxGrowSize
        size_t growBy = this->capacity >> 1;
        if (growBy == 0)
        {
            growBy = MinGrowSize;
        }
        else if (growBy > MaxGrowSize)
        {
            growBy = MaxGrowSize;
        }
        growToSize = this->capacity + growBy;
    }
    this->GrowTo(growToSize);
}

//------------------------------------------------------------------------------
/**
    30-Jan-03   floh    serious bugfixes!
	07-Dec-04	jo		bugfix: neededSize >= this->capacity => neededSize > capacity	
*/
template<class TYPE> void
Array<TYPE>::Move(size_t fromIndex, size_t toIndex)
{
    // nothing to move?
    if (fromIndex == toIndex)
    {
        return;
    }

    // compute number of elements to move
    int num = (int)this->size - fromIndex;

    // check if array needs to grow
    int neededSize = (int)toIndex + num;
    while (neededSize > this->capacity)
    {
        this->Grow();
    }

    if (fromIndex > toIndex)
    {
        // this is a backward move
        int i;
        for (i = 0; i < num; i++)
        {
            this->elements[toIndex + i] = this->elements[fromIndex + i];
        }

        // destroy remaining elements
        for (i = ((int)fromIndex + i) - 1; i < (int)this->size; i++)
        {
            this->Destroy(&(this->elements[i]));
        }
    }
    else
    {
        // this is a forward move
        long long i;  // NOTE: this must remain signed for the following loop to work!!!
        for (i = num - 1; i >= 0; --i)
        {
            this->elements[toIndex + i] = this->elements[fromIndex + i];
        }

        // destroy freed elements
        for (i = int(fromIndex); i < int(toIndex); i++)
        {
            this->Destroy(&(this->elements[i]));
        }
    }

    // adjust array size
    this->size = toIndex + num;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Append(const TYPE& elm)
{
    // grow allocated space if exhausted
    if (this->size == this->capacity)
    {
        this->Grow();
    }
    this->elements[this->size++] = elm;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::AppendArray(const Array<TYPE>& rhs)
{
	size_t i;
    size_t num = rhs.Size();
    for (i = 0; i < num; i++)
    {
        this->Append(rhs[i]);
    }
}

//------------------------------------------------------------------------------
/**
    This increases the capacity to make room for N elements. If the
    number of elements is known before appending the elements, this 
    method can be used to prevent reallocation. If there is already
    enough room for N more elements, nothing will happen.
    
    NOTE: the functionality of this method has been changed as of 26-Apr-08,
    it will now only change the capacity of the array, not its size.
*/
template<class TYPE> void
Array<TYPE>::Reserve(size_t num)
{
    size_t neededCapacity = this->size + num;
    if (neededCapacity > this->capacity)
    {
        this->GrowTo(neededCapacity);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> size_t
Array<TYPE>::Size() const
{
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> size_t
Array<TYPE>::Capacity() const
{
    return this->capacity;
}

//------------------------------------------------------------------------------
/**
    Access an element. This method will NOT grow the array, and instead do
    a range check, which may throw an assertion.
*/
template<class TYPE> TYPE&
Array<TYPE>::operator[](const size_t& index) const
{
    return this->elements[index];
}
//------------------------------------------------------------------------------
/**
    The equality operator returns true if all elements are identical. The
    TYPE class must support the equality operator.
*/
template<class TYPE> bool
Array<TYPE>::operator==(const Array<TYPE>& rhs) const
{
    if (rhs.Size() == this->Size())
    {
		size_t i;
        size_t num = this->Size();
        for (i = 0; i < num; i++)
        {
            if (!(this->elements[i] == rhs.elements[i]))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    The inequality operator returns true if at least one element in the 
    array is different, or the array sizes are different.
*/
template<class TYPE> bool
Array<TYPE>::operator!=(const Array<TYPE>& rhs) const
{
    return !(*this == rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE&
Array<TYPE>::Front() const
{
    return this->elements[0];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE&
Array<TYPE>::Back() const
{
    return this->elements[this->size - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool 
Array<TYPE>::IsEmpty() const
{
    return (this->size == 0);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::EraseIndex(size_t index)
{
    if (index == (this->size - 1))
    {
        // special case: last element
        this->Destroy(&(this->elements[index]));
        this->size--;
    }
    else
    {
        this->Move(index + 1, index);
    }
}

//------------------------------------------------------------------------------
/**    
    NOTE: this method is fast but destroys the sorting order!
*/
template<class TYPE> void
Array<TYPE>::EraseIndexSwap(size_t index)
{
    // swap with last element, and destroy last element
	size_t lastElementIndex = this->size - 1;
    if (index < lastElementIndex)
    {
        this->elements[index] = this->elements[lastElementIndex];
    }
    this->Destroy(&(this->elements[lastElementIndex]));
    this->size--;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::Erase(typename Array<TYPE>::Iterator iter)
{
	this->EraseIndex(size_t(iter - this->elements));
    return iter;
}

//------------------------------------------------------------------------------
/**
    NOTE: this method is fast but destroys the sorting order!
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::EraseSwap(typename Array<TYPE>::Iterator iter)
{
	//HACK: Couldn't resolve external symbol... The linker really doesn't want to find the definition for this function, so i just copied it for now!
	//this->EraseSwapIndex(uint(iter - this->elements));

	//---- COPY OF 'EraseSwapIndex' FUNCTION -------
	size_t index = size_t(iter - this->elements);
	// swap with last element, and destroy last element
	size_t lastElementIndex = this->size - 1;
	if (index < lastElementIndex)
	{
		this->elements[index] = this->elements[lastElementIndex];
	}
	this->Destroy(&(this->elements[lastElementIndex]));
	this->size--;
	//-------------------

    return iter;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::RemoveIndex(size_t index)
{
	if (index == (this->size - 1))
	{
		// special case: last element
		this->size--;
	}
	else
	{
		this->Move(index + 1, index);
	}
}

//------------------------------------------------------------------------------
/**
NOTE: this method is fast but destroys the sorting order!
*/
template<class TYPE> void
Array<TYPE>::RemoveIndexSwap(size_t index)
{
	// swap with last element, and destroy last element
	size_t lastElementIndex = this->size - 1;
	if (index < lastElementIndex)
	{
		this->elements[index] = this->elements[lastElementIndex];
	}
	this->size--;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::Remove(typename Array<TYPE>::Iterator iter)
{
	this->RemoveIndex(size_t(iter - this->elements));
	return iter;
}

//------------------------------------------------------------------------------
/**
NOTE: this method is fast but destroys the sorting order!
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::RemoveSwap(typename Array<TYPE>::Iterator iter)
{
	//HACK: Couldn't resolve external symbol... The linker really doesn't want to find the definition for this function, so i just copied it for now!
	//this->RemoveSwapIndex(uint(iter - this->elements));

	//---- COPY OF 'RemoveSwapIndex' FUNCTION -------
	size_t index = size_t(iter - this->elements);
	// swap with last element, and destroy last element
	size_t lastElementIndex = this->size - 1;
	if (index < lastElementIndex)
	{
		this->elements[index] = this->elements[lastElementIndex];
	}
	this->size--;
	//-------------------

	return iter;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Array<TYPE>::Insert(size_t index, const TYPE& elm)
{
    if (index == this->size)
    {
        // special case: append element to back
        this->Append(elm);
    }
    else
    {
        this->Move(index, index + 1);
        this->elements[index] = elm;
    }
}

//------------------------------------------------------------------------------
/**
    The current implementation of this method does not shrink the 
    preallocated space. It simply sets the array size to 0.
*/
template<class TYPE> void
Array<TYPE>::Clear()
{
	size_t i;
    for (i = 0; i < this->size; i++)
    {
        this->Destroy(&(this->elements[i]));
    }
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
    This is identical with Clear(), but does NOT call destructors (it just
    resets the size member. USE WITH CARE!
*/
template<class TYPE> void
Array<TYPE>::Reset()
{
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::begin() const
{
    return this->elements;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::end() const
{
    return this->elements + this->size;
}

//------------------------------------------------------------------------------
/**
    Find element in array, return iterator, or 0 if element not
    found.
    @param  elm     element to find
    @return         element iterator, or 0 if not found
*/
template<class TYPE> typename Array<TYPE>::Iterator
Array<TYPE>::Find(const TYPE& elm) const
{
	size_t index;
    for (index = 0; index < this->size; index++)
    {
        if (this->elements[index] == elm)
        {
            return &(this->elements[index]);
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Find element in array, return element index, or InvalidIndex if element not
    found.
    @param  elm     element to find
    @return         index to element, or InvalidIndex if not found
*/
template<class TYPE> size_t
Array<TYPE>::FindIndex(const TYPE& elm) const
{
	size_t index;
    for (index = 0; index < this->size; index++)
    {
        if (this->elements[index] == elm)
        {
            return index;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Fills an array range with the given element value. Will grow the
    array if necessary
    @param  first   index of first element to start fill
    @param  num     num elements to fill
    @param  elm     fill value
*/
template<class TYPE> void
Array<TYPE>::Fill(size_t first, size_t num, const TYPE& elm)
{
    if ((first + num) > this->size)
    {
        this->GrowTo(first + num);
    }
	size_t i;
    for (i = first; i < (first + num); i++)
    {
        this->elements[i] = elm;
		this->size++;
    }
}

//------------------------------------------------------------------------------
/**
    Returns a new array with all element which are in rhs, but not in this.
    Carefull, this method may be very slow with large arrays!
    @todo this method is broken, check test case to see why!
*/
template<class TYPE> Array<TYPE>
Array<TYPE>::Difference(const Array<TYPE>& rhs)
{
    Array<TYPE> diff;
	size_t i;
    size_t num = rhs.Size();
    for (i = 0; i < num; i++)
    {
        if (0 == this->Find(rhs[i]))
        {
            diff.Append(rhs[i]);
        }
    }
    return diff;
}

//------------------------------------------------------------------------------
/**
    Sorts the array. This just calls the STL sort algorithm.
*/
template<class TYPE> void
Array<TYPE>::Sort()
{
    std::sort(this->begin(), this->end());
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
Util::Array<TYPE>::SortWithFunc(bool (*func)(const TYPE& lhs, const TYPE& rhs))
{
	std::sort(this->begin(), this->end(), func);
}

//------------------------------------------------------------------------------
/**
    Does a binary search on the array, returns the index of the identical
    element, or InvalidIndex if not found
*/
template<class TYPE> size_t
Array<TYPE>::BinarySearchIndex(const TYPE& elm) const
{
	int num = (int)this->Size();
    if (num > 0)
    {
		int half;
		int lo = 0;
		int hi = num - 1;
		int mid;
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
            else if (0 != num) 
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
    This tests, whether the array is sorted. This is a slow operation
    O(n).
*/
template<class TYPE> bool
Array<TYPE>::IsSorted() const
{
    if (this->size > 1)
    {
		size_t i;
        for (i = 0; i < this->size - 1; i++)
        {
            if (this->elements[i] > this->elements[i + 1])
            {
                return false;
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    This inserts an element at the end of a range of identical elements
    starting at a given index. Performance is O(n). Returns the index
    at which the element was added.
*/
template<class TYPE> size_t
Array<TYPE>::InsertAtEndOfIdenticalRange(size_t startIndex, const TYPE& elm)
{
	size_t i = startIndex + 1;
    for (; i < this->size; i++)
    {
        if (this->elements[i] != elm)
        {
            this->Insert(i, elm);
            return i;
        }
    }

    // fallthrough: new element needs to be appended to end
    this->Append(elm);
    return (this->Size() - 1);
}

//------------------------------------------------------------------------------
/**
    This inserts the element into a sorted array. Returns the index
    at which the element was inserted.
*/
template<class TYPE> size_t
Array<TYPE>::InsertSorted(const TYPE& elm)
{
    int num = (int)this->Size();
    if (num == 0)
    {
        // array is currently empty
        this->Append(elm);
        return this->Size() - 1;
    }
    else
    {
		int half;
		int lo = 0;
		int hi = num - 1;
		int mid;
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
                    // element already exists at [mid], append the
                    // new element to the end of the range
                    return this->InsertAtEndOfIdenticalRange(mid, elm);
                }
            } 
            else if (0 != num) 
            {
                if (elm < this->elements[lo])
                {
                    this->Insert(lo, elm);
                    return lo;
                }
                else if (elm > this->elements[lo])
                {
                    this->Insert(lo + 1, elm);
                    return lo + 1;
                }
                else      
                {
                    // element already exists at [low], append 
                    // the new element to the end of the range
                    return this->InsertAtEndOfIdenticalRange(lo, elm);
                }
            } 
            else 
            {
                this->Insert(lo, elm);
                return lo;
            }
        }
        if (elm < this->elements[lo])
        {
            this->Insert(lo, elm);
            return lo;
        }
        else if (elm > this->elements[lo])
        {
            this->Insert(lo + 1, elm);
            return lo + 1;
        }
        else
        {
            // can't happen(?)
        }
    }
    // can't happen
    //n_error("Array::InsertSorted: Can't happen!");
    return InvalidIndex;
}

}