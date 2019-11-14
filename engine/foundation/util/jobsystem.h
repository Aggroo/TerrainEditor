#pragma once
#include "core/singleton.h"
#include <atomic>
#include <functional>
#include "threadsaferingbuffer.h"
#include <condition_variable>

//------------------------------------------------------------------------------
/**

A job system to handle multithreading

Code created by github user @turanszkij

Small adjustments by me
*/

namespace JobSystem
{

/***
 *
 *
 */
struct JobDispatchArgs
{
	uint32_t jobIndex;
	uint32_t groupIndex;
};

// Defines a state of execution, can be waited on
struct Context
{
	std::atomic<uint32_t> counter{ 0 };
};

struct Job
{
	std::function<void()> task;
	Context* ctx;
};

/***
 *
 *
 */

void Initialize();


uint32_t GetThreadCount();


// Add a job to execute asynchronously. Any idle thread will execute this job.
void Execute(Context& ctx, const std::function<void()>& job);

// Divide a job onto multiple jobs and execute in parallel.
// jobCount	: how many jobs to generate for this task.
// groupSize	: how many jobs to execute per thread. Jobs inside a group execute serially. It might be worth to increase for small jobs
// func		: receives a wiJobDispatchArgs as parameter
void Dispatch(Context& ctx, uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);

// Check if any threads are working currently or not
bool IsBusy(const Context& ctx);

// Wait until all threads become idle
void Wait(const Context& ctx);

}