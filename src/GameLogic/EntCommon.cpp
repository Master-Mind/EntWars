#include "EntCommon.h"
#include "../Core/Time.h"
#include "Level.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "../Core/TheWindow.h"
#include "../Core/VectorMath.hpp"
#include <EASTL/fixed_vector.h>
#include "Systems/Units.h"

entt::registry registry;

static eastl::fixed_vector<std::thread, 1, false> pathWorkers;

void LogicInit(const std::unique_ptr<Level>& level)
{
	for (int x = 0; x < level->tileMap->width; ++x)
	{
		for (int y = 0; y < level->tileMap->height; ++y)
		{
			if (level->tileMap->GetTile(x, y) == TileType::headquarters)
			{
				auto entity = registry.create();
				registry.emplace<Position>(entity, sf::Vector2f(x * TileMap::tileSize, y * TileMap::tileSize));
				registry.emplace<Velocity>(entity, sf::Vector2f(0, 0));
				registry.emplace<PathFollower>(entity, PathReciever{}, sf::Vector2f{ 574, 64 });
				registry.emplace<CircleRenderer>(entity, TileMap::tileSize / 2, sf::Color::Blue);
			}
		}
	}

	for (int i = 0; i < pathWorkers.capacity(); ++i)
	{
		pathWorkers.emplace_back(PathWorker, *level->tileMap);
	}
}

void LogicUpdate()
{
	auto pathview = registry.view<Position, Velocity, PathFollower>();

	for (auto entity : pathview)
	{
		auto& pos = pathview.get<Position>(entity);
		auto& follower = pathview.get<PathFollower>(entity);
		auto& vel = pathview.get<Velocity>(entity);
		auto& path = follower.reciever.path;

		//asyncronously find path
		if (follower.target.x != FLT_MAX)
		{
			FindPath(pos.pos, follower.target, &follower.reciever);
			follower.target = { FLT_MAX, FLT_MAX };
		}

		//once the path is found, follow it
		if (path.size() > 0)
		{
			sf::Vector2f target = path.back();
			sf::Vector2f dir = target - pos.pos;

			if (VecLenSquared(dir) < 1.0f)
			{
				path.pop_back();
			}
			else
			{
				vel.vel = VecNorm(dir) * 100.0f;
			}
		}
		else
		{
			vel.vel = { 0, 0 };
		}
	}

	static sf::Clock workerClock;

	CheckPaths();

	if (workerClock.getElapsedTime().asMilliseconds() > 3)
	{
		DispatchTask();
		workerClock.restart();
	}

	auto velociview = registry.view<Position, Velocity>();
	for (auto entity : velociview)
	{
		auto& pos = velociview.get<Position>(entity);
		auto& vel = velociview.get<Velocity>(entity);
		pos.pos += vel.vel * Time().dt;
	}

	auto renderview = registry.view<Position, CircleRenderer>();
	for (auto entity : renderview)
	{
		auto& pos = renderview.get<Position>(entity);
		auto& renderer = renderview.get<CircleRenderer>(entity);
		
		sf::CircleShape circle(renderer.radius);
		circle.setFillColor(renderer.color);
		circle.setPosition(pos.pos + sf::Vector2f{-renderer.radius, -renderer.radius});
		GetWindow().draw(circle);
	}
}

void LogicDeinit()
{
	registry.clear();

	for (int i = 0; i < pathWorkers.size(); ++i)
	{
		KillWorkers();
	}

	for (int i = 0; i < pathWorkers.size(); ++i)
	{
		pathWorkers[i].join();
	}

	pathWorkers.clear();
}
