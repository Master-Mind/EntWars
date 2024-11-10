#include "TileMap.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
#include "../Core/fileutils.hpp"

const float tileCosts[TileType::count] = { 0.0f, 1.0f };
const float TileMap::tileSize = 10;
const std::array<sf::Color, TileType::count> tileColors = { sf::Color::Black, sf::Color::White };
const std::array<const char, TileType::count> tileChars = { '_', '#' };

void TileMap::Resize(int newWidth, int newHeight)
{
	std::vector<TileType> newTypes(newWidth * newHeight);
	std::vector<std::unique_ptr<sf::RectangleShape>> newTiles(newWidth * newHeight);

	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			newTypes[y * newWidth + x] = GetTile(x, y);
			newTiles[y * newWidth + x].swap(tileShapes[GetIndex(x, y)]);
		}
	}

	tiles.swap(newTypes);
	tileShapes.swap(newTiles);

	width = newWidth;
	height = newHeight;
}

std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path)
{
	std::string fileContents = ReadWholeFile(path).value();
	std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>();

	if (fileContents.size() < 2)
	{
		return tileMap;
	}

	return tileMap;
}

bool SaveTileMap(const std::filesystem::path& path, const TileMap& tileMap)
{
	std::string data;

	for (int y = 0; y < tileMap.height; ++y)
	{
		for (int x = 0; x < tileMap.width; ++x)
		{
			data += tileChars[tileMap.GetTile(x, y)];
		}

		data += '\n';
	}

	return true;
}