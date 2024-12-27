#pragma once
#include <expected>
#include <filesystem>
#include <memory>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>

#include "EASTL/hash_set.h"
#include "SFML/Graphics/RenderWindow.hpp"

struct PathNode;

// NOTE: when adding a type here, also add to:
// 1. tileCosts
// 2. tileColors
// 3. tileChars
// 4. asciiToTileType
enum TileType : char
{
	empty,
	wall,
	error,
	headquarters,
	count
};

extern const float tileCosts[TileType::count];

struct TileMap
{
	int width = 0;
	int height = 0;
	const static float tileSize;
	[[nodiscard]] TileType GetTile(size_t x, size_t y) const
	{
		return tiles[GetIndex(x,y)];
	}

	void SetTile(size_t x, size_t y, TileType val)
	{
		tiles[GetIndex(x, y)] = val;
	}

	[[nodiscard]] bool IsInBounds(int x, int y) const
	{
		return x >= 0 && x < width && y >= 0 && y < height;
	}

	[[nodiscard]] bool IsEmpty(size_t x, size_t y) const
	{
		return GetTile(x, y) == TileType::empty;
	}

	[[nodiscard]] bool IsWall(size_t x, size_t y) const
	{
		return GetTile(x, y) == TileType::wall;
	}

	[[nodiscard]] float GetCost(size_t x, size_t y) const
	{
		return tileCosts[GetTile(x, y)];
	}
	[[nodiscard]] float GetCost(const sf::Vector2i& pos) const
	{
		return GetCost(pos.x, pos.y);
	}
	[[nodiscard]] bool IsEmpty(const sf::Vector2i& pos) const
	{
		return IsEmpty(pos.x, pos.y);
	}
	[[nodiscard]] bool IsWall(const sf::Vector2i& pos) const
	{
		return IsWall(pos.x, pos.y);
	}
	[[nodiscard]] bool IsInBounds(const sf::Vector2i& pos) const
	{
		return IsInBounds(pos.x, pos.y);
	}

#ifndef NDEBUG
	void SetDebugTile(int x, int y, bool val)
	{
		debugtiles[GetIndex(x, y)] = val;
	}

	void ClearDebugTiles()
	{
		std::fill(debugtiles.begin(), debugtiles.end(), false);
	}
#endif

	void Resize(int newWidth, int newHeight);

	void Render(sf::RenderWindow& window);

	friend std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path);

	std::vector<sf::Vector2f> AStarSearch(sf::Vector2f start, sf::Vector2f end, bool debug = false);
private:
	int GetIndex(size_t x, size_t y) const
	{
		return y * width + x;
	}

	int GetIndex(const sf::Vector2i &pos) const
	{
		return GetIndex(pos.x, pos.y);
	}
	void VisitNeighor(const PathNode& current, int x, int y,
		eastl::hash_set<sf::Vector2i>& closedSet, std::vector<float>& costSoFar, std::vector<bool>& visited);
	std::vector<TileType> tiles;
#ifndef NDEBUG
	std::vector<bool> debugtiles;
#endif
};

std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path);
[[nodiscard]] bool SaveTileMap(const std::filesystem::path& path, const TileMap& tileMap);