#pragma once
//------------------------------------------------------------------------------
/**
@class Util::ThreadPool

A thread pool class containing 
- A pool of threads 
- A queue of tasks
- Functions to manage the scheduling

All thread objects are stored in a list. 
Threads are joined in the constructor

A job is a function enqueued into the thread pool. 
If a thread is available when we enqueue our job, then it is woken up
and the job is processed. If all threads are busy, the job stays in 
the queue, and will be picked up when a worker thread becomes available again.

A task is a collection of jobs. Every job belongs to a task, and a task 
can have many jobs, or even no job at all. It is possible for the master 
thread to wait for a task to be completed, that is, to wait until every job 
attached to that task returns. That's how we will achieve synchronization.

@copyright	See LICENCE file
*/

#include "core/types.h"
#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>
#include "core/singleton.h"

namespace Util
{

class ThreadPool
{
__DeclareSingleton(ThreadPool)

public:
	using Job = std::function<void(void)>;

	~ThreadPool();

	///Creates a task, reuses a free task if available
	int TaskCreate();
	///Checks if a task is finished
	bool TaskFinished(int task) const;
	///Add a job to a task
	void TaskPerform(int task, const Job& job);
	///Make the worker threads sleep when the job queue is empty
	void TaskWait(int task);

private:
	void WorkMain();

	bool running;
	mutable std::mutex mutex;
	std::vector<std::thread> threads;

	///Number of tasks
	size_t taskSize;
	///List of free tasks, to reuse memory
	std::vector<int> availableTasks;
	///Per-task count of how many jobs are pending for the task
	std::vector<uint> pendingTasksCount;

	size_t jobSize;
	///Maps every job to the task it belongs to
	std::vector<int> jobTasks;

	///Stores the job functions linked to a job
	std::vector<Job> jobs;

	///Used for the work threads
	std::condition_variable cvWorker;
	///Used for TaskWait
	std::condition_variable cvMaster;
};

inline ThreadPool::ThreadPool() : running(true), taskSize(0u), jobSize(0u)
{
	size_t threadCount;

	threadCount = std::thread::hardware_concurrency();

	if (threadCount == 0)
	{
		threadCount = 1;
	}

	threads.reserve(threadCount);

	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&ThreadPool::WorkMain, this);
	}
}

inline ThreadPool::~ThreadPool()
{
	std::unique_lock<std::mutex> lock(mutex);

	running = false;

	cvWorker.notify_all();

	lock.unlock();

	for (auto& t : threads)
		t.join();
}

inline int ThreadPool::TaskCreate()
{
	std::lock_guard<std::mutex> lock(mutex);
	int task;

	if (availableTasks.empty())
	{
		task = static_cast<int>(taskSize);
		taskSize++;
		pendingTasksCount.resize(taskSize);
	}
	else
	{
		task = availableTasks.back();
		availableTasks.pop_back();
	}
	pendingTasksCount[task] = 0u;

	return task;
}

inline bool ThreadPool::TaskFinished(int task) const
{
	std::lock_guard<std::mutex> lock(mutex);

	return pendingTasksCount[task] == 0;
}

inline void ThreadPool::TaskPerform(int task, const Job& job)
{
	std::lock_guard<std::mutex> lock(mutex);

	jobSize++;
	jobTasks.push_back(task);
	jobs.push_back(job);
	pendingTasksCount[task]++;

	cvWorker.notify_one();
}

inline void ThreadPool::TaskWait(int task)
{
	std::unique_lock<std::mutex> lock(mutex);

	while (pendingTasksCount[task] > 0)
		cvMaster.wait(lock);

	availableTasks.push_back(task);
}

inline void ThreadPool::WorkMain()
{
	std::unique_lock<std::mutex> lock(mutex);

	while (running)
	{
		cvWorker.wait(lock);

		for (;;)
		{
			if (jobSize == 0)
				break;

			int task = jobTasks.back();
			Job job = jobs.back();
			jobTasks.pop_back();
			jobs.pop_back();
			jobSize--;
			lock.unlock();

			job();

			lock.lock();
			pendingTasksCount[task]--;

			if (pendingTasksCount[task] == 0)
				cvMaster.notify_all();

		}
	}
}

}
