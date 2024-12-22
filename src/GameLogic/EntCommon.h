#pragma once
#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>
#include "PathWorker.h"

struct Level;
extern entt::registry registry;

struct Position
{
	sf::Vector2f pos;
};

struct Velocity
{
	sf::Vector2f vel;
};

struct PathFollower
{
	PathReciever reciever;
	sf::Vector2f target;
};

struct CircleRenderer
{
	float radius;
	sf::Color color;
};

void LogicInit(const std::unique_ptr<Level>& level);
void LogicUpdate();
void LogicDeinit();