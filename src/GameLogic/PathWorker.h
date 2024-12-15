#pragma once
#include <future>

#include "../pch.h"
#include "TileMap.h"
#include "../Core/TaskThreads.h"

//deep copy of the map since it'll be handled on multiple threads
void PathWorker(TileMap map);

struct PathReciever
{
	std::vector<sf::Vector2f> path;
	bool ready = false;
};

void FindPath(const sf::Vector2i& start, const sf::Vector2i& end,
	PathReciever* pathReciever);
void CheckPaths();
void DispatchTask();
//the most bourgeois function in the game
void KillWorkers();