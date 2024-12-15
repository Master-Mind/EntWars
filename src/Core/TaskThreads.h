#pragma once
#include <thread>
#include <atomic_queue/atomic_queue.h>

struct Task
{
	virtual ~Task() = default;
	virtual void Execute() = 0;
};

struct KillThread : Task
{
	void Execute() override {}
};

struct SleepTask : Task
{
	int ms;
	SleepTask(int ms) : ms(ms) {}
	void Execute() override
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
};

typedef atomic_queue::AtomicQueue2<Task*, 128> TaskQueue;