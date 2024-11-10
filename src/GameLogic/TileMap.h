#pragma once
#include <expected>
#include <filesystem>
#include <memory>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>

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

	void Resize(int newWidth, int newHeight);

private:
	int GetIndex(size_t x, size_t y) const
	{
		return y * width + x;
	}

	std::vector<TileType> tiles;
	std::vector<std::unique_ptr<sf::RectangleShape>> tileShapes;
};

std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path);
[[nodiscard]] bool SaveTileMap(const std::filesystem::path& path, const TileMap& tileMap);