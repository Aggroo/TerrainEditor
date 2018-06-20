//------------------------------------------------------------------------------
//  @file		threadpool.cpp
//  @copyright	See LICENCE file
//------------------------------------------------------------------------------
#include "config.h"
#include "threadpool.h"
namespace Util
{

ThreadPool::ThreadPool() : running(true), taskSize(0u), jobSize(0u)
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

ThreadPool::~ThreadPool()
{
	std::unique_lock<std::mutex> lock(mutex);

	running = false;

	cvWorker.notify_all();

	lock.unlock();

	for (auto& t : threads)
		t.join();
}

int ThreadPool::TaskCreate()
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

bool ThreadPool::TaskFinished(int task) const
{
	std::lock_guard<std::mutex> lock(mutex);

	return pendingTasksCount[task] == 0;
}

void ThreadPool::TaskPerform(int task, const Job& job)
{
	std::lock_guard<std::mutex> lock(mutex);

	jobSize++;
	jobTasks.push_back(task);
	jobs.push_back(job);
	pendingTasksCount[task]++;

	cvWorker.notify_one();
}

void ThreadPool::TaskWait(int task)
{
	std::unique_lock<std::mutex> lock(mutex);

	while (pendingTasksCount[task] > 0)
		cvMaster.wait(lock);
	
	availableTasks.push_back(task);
}

void ThreadPool::WorkMain()
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

