//------------------------------------------------------------------------------
//  @file		threadpool.cpp
//  @copyright	See LICENCE file
//------------------------------------------------------------------------------
#include "config.h"
#include "jobsystem.h"
#include "foundation/math/math.h"

namespace JobSystem
{

uint32_t numThreads = 0;
Util::ThreadSafeRingBuffer<Job, 256> jobQueue;
std::condition_variable wakeCondition;
std::mutex wakeMutex;

// This function executes the next item from the job queue. Returns true if successful, false if there was no job available
inline bool Work()
{
	Job job;

	if (jobQueue.pop_front(job))
	{
		job.task(); // execute job
		job.ctx->counter.fetch_sub(1);
		return true;
	}
	return false;
}

void Initialize()
{
	// Retrieve the number of hardware threads in this system:
	auto numCores = std::thread::hardware_concurrency();

	// Calculate the actual number of worker threads we want (-1 main thread):
	numThreads = Math::Max(1u, numCores - 1);

	for (uint32_t threadID = 0; threadID < numThreads; ++threadID)
	{
		std::thread worker([] {

			std::function<void()> job;

			while (true)
			{
				if (!Work())
				{
					// no job, put thread to sleep
					std::unique_lock<std::mutex> lock(wakeMutex);
					wakeCondition.wait(lock);
				}
			}

		});

		worker.detach();
	}

}

uint32_t GetThreadCount()
{
	return numThreads;
}

void Execute(Context& ctx, const std::function<void()>& job)
{
	// Context state is updated:
	ctx.counter.fetch_add(1);

	// Try to push a new job until it is pushed successfully:
	while (!jobQueue.push_back({ job, &ctx })) { wakeCondition.notify_all(); }

	// Wake any one thread that might be sleeping:
	wakeCondition.notify_one();
}

void Dispatch(Context& ctx, uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job)
{
	if (jobCount == 0 || groupSize == 0)
	{
		return;
	}

	// Calculate the amount of job groups to dispatch (overestimate, or "ceil"):
	const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

	// Context state is updated:
	ctx.counter.fetch_add(groupCount);

	for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex)
	{
		// For each group, generate one real job:
		const auto& jobGroup = [jobCount, groupSize, job, groupIndex]() {

			// Calculate the current group's offset into the jobs:
			const uint32_t groupJobOffset = groupIndex * groupSize;
			const uint32_t groupJobEnd = Math::Min(groupJobOffset + groupSize, jobCount);

			JobDispatchArgs args;
			args.groupIndex = groupIndex;

			// Inside the group, loop through all job indices and execute job for each index:
			for (uint32_t i = groupJobOffset; i < groupJobEnd; ++i)
			{
				args.jobIndex = i;
				job(args);
			}
		};

		// Try to push a new job until it is pushed successfully:
		while (!jobQueue.push_back({ jobGroup, &ctx })) { wakeCondition.notify_all(); }
	}

	// Wake any threads that might be sleeping:
	wakeCondition.notify_all();
}

bool IsBusy(const Context& ctx)
{
	// Whenever the context label is greater than zero, it means that there is still work that needs to be done
	return ctx.counter.load() > 0;
}

void Wait(const Context& ctx)
{
	// Wake any threads that might be sleeping:
	wakeCondition.notify_all();

	// Waiting will also put the current thread to good use by working on an other job if it can:
	while (IsBusy(ctx)) { Work(); }
}

}

