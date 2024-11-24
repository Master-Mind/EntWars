#pragma once
#include <expected>
#include <filesystem>
#include <memory>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>

#include "SFML/Graphics/RenderWindow.hpp"

enum TileType : char
{
	empty,
	wall,
	count
};

extern const float tileCosts[TileType::count];

struct TileMap
{
	int width;
	int height;
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

	std::vector<sf::Vector2f> AStarSearch(sf::Vector2f start, sf::Vector2f end);
private:
	int GetIndex(size_t x, size_t y) const
	{
		return y * width + x;
	}

	std::vector<TileType> tiles;
#ifndef NDEBUG
	std::vector<bool> debugtiles;
#endif
};

std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path);
[[nodiscard]] bool SaveTileMap(const std::filesystem::path& path, const TileMap& tileMap);