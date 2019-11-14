#pragma once
#include <atomic>

namespace Util
{
/***
*
*
*/
class SpinLock
{
private:
	std::atomic_flag lck = ATOMIC_FLAG_INIT;
public:
	void lock()
	{
		while (!try_lock()) {}
	}
	bool try_lock()
	{
		return !lck.test_and_set(std::memory_order_acquire);
	}

	void unlock()
	{
		lck.clear(std::memory_order_release);
	}
};


/***
* Fixed size very simple thread safe ring buffer
* 
*/
template <typename T, size_t capacity>
class ThreadSafeRingBuffer
{
public:
	// Push an item to the end if there is free space
	//	Returns true if succesful
	//	Returns false if there is not enough space
	inline bool push_back(const T& item)
	{
		bool result = false;
		lock.lock();
		size_t next = (head + 1) % capacity;
		if (next != tail)
		{
			data[head] = item;
			head = next;
			result = true;
		}
		lock.unlock();
		return result;
	}

	// Get an item if there are any
	//	Returns true if succesful
	//	Returns false if there are no items
	inline bool pop_front(T& item)
	{
		bool result = false;
		lock.lock();
		if (tail != head)
		{
			item = data[tail];
			tail = (tail + 1) % capacity;
			result = true;
		}
		lock.unlock();
		return result;
	}

private:
	T data[capacity];
	size_t head = 0;
	size_t tail = 0;
	SpinLock lock;
};

}