#include "PathWorker.h"

#include <queue>
#include <EASTL/fixed_vector.h>
#include <functional>

static TaskQueue q;

const int pathsPerTask = 128;

//use one big promise to avoid atomic assigns for each path
typedef eastl::fixed_vector<std::vector<sf::Vector2f>, pathsPerTask, false> PathArray;

struct PathResponse
{
	//written to on worker thread
	PathArray paths;
	//written to on the caller thread
	eastl::fixed_vector<PathReciever*, pathsPerTask, false> pathRecievers;
};

typedef std::promise<PathResponse> PathPromise;
typedef std::future<PathResponse> PathFuture;

struct FindPaths : Task
{
	eastl::fixed_vector<sf::Vector2f, pathsPerTask, false> starts;
	eastl::fixed_vector<sf::Vector2f, pathsPerTask, false> ends;
	PathPromise pathPromises;
	eastl::fixed_vector< PathReciever*, pathsPerTask, false> pathRecievers;
	TileMap* threadLocalMap = nullptr;

	void Execute() override;
};

thread_local FindPaths* localtask = new FindPaths();
thread_local std::queue<PathFuture> pathFutures;

void DispatchTask()
{
	if (!localtask->starts.empty())
	{
		pathFutures.push(localtask->pathPromises.get_future());
		q.push(localtask);
		localtask = new FindPaths();
	}
}

void KillWorkers()
{
	q.push(new KillThread());
}

//non path worker thread code
void FindPath(const sf::Vector2f& start, const sf::Vector2f& end,
	PathReciever* pathReciever)
{
	assert(pathReciever);
	localtask->starts.push_back(start);
	localtask->ends.push_back(end);
	localtask->pathRecievers.emplace_back(pathReciever);

	if (localtask->starts.size() == pathsPerTask)
	{
		DispatchTask();
	}
}

void CheckPaths()
{
	while (!pathFutures.empty() && pathFutures.front().valid())
	{
		//weird hack to check if the future is ready without blocking
		auto status = pathFutures.front().wait_for(std::chrono::seconds(0));

		if (status != std::future_status::ready)
		{
			break;
		}

		PathResponse response = pathFutures.front().get();
		pathFutures.pop();
		for (int i = 0; i < response.paths.size(); ++i)
		{
			response.pathRecievers[i]->path = std::move(response.paths[i]);
			response.pathRecievers[i]->ready = true;
		}
	}
}

//path worker thread code
void FindPaths::Execute()
{
	assert(threadLocalMap);

	eastl::fixed_vector<std::vector<sf::Vector2f>, pathsPerTask, false> paths;
	paths.resize(starts.size());

	for (int i = 0; i < starts.size(); ++i)
	{
		paths[i] = threadLocalMap->AStarSearch(starts[i], ends[i]);
	}

	pathPromises.set_value({ std::move(paths), std::move(pathRecievers) });
}

void PathWorker(TileMap map)
{
	while (true)
	{
		Task* task = q.pop();
		if (task)
		{
			if (dynamic_cast<FindPaths*>(task))
			{
				FindPaths* findPaths = static_cast<FindPaths*>(task);
				findPaths->threadLocalMap = &map;
			}

			task->Execute();

			if (dynamic_cast<KillThread*>(task))
			{
				delete task;
				break;
			}

			delete task;
		}
	}
}
