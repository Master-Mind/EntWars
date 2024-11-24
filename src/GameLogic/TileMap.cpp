#include "TileMap.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
#include <cassert>

#include "../Core/fileutils.hpp"

const float tileCosts[TileType::count] = { 0.0f, 1.1f };
const float TileMap::tileSize = 10;
const std::array<sf::Color, TileType::count> tileColors = { sf::Color::Magenta, sf::Color(135,62,35)};
const std::array<const char, TileType::count> tileChars = { '_', '#' };



void TileMap::Resize(int newWidth, int newHeight)
{
	std::vector<TileType> newTypes(newWidth * newHeight);

	for (size_t x = 0; x < std::min(width, newWidth); ++x)
	{
		for (size_t y = 0; y < std::min(height, newHeight); ++y)
		{
			newTypes[y * newWidth + x] = GetTile(x, y);
		}
	}

	tiles.swap(newTypes);
	debugtiles.resize(newWidth * newHeight);

	width = newWidth;
	height = newHeight;
}

void TileMap::Render(sf::RenderWindow& window)
{
	sf::RectangleShape shape(sf::Vector2f(tileSize, tileSize));

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (GetTile(x, y) != TileType::empty)
			{
				shape.setPosition(x * tileSize, y * tileSize);
				shape.setFillColor(tileColors[GetTile(x, y)]);
				window.draw(shape);
			}

#ifndef NDEBUG
			if (debugtiles[GetIndex(x, y)])
			{
				shape.setPosition(x * tileSize, y * tileSize);
				shape.setFillColor(sf::Color(0, 0, 128, 128));
				window.draw(shape);
			}
#endif
		}
	}
}

std::vector<sf::Vector2f> TileMap::AStarSearch(sf::Vector2f start, sf::Vector2f end)
{
	sf::Vector2i startTile = sf::Vector2i(static_cast<int>(std::floor(start.x / tileSize)), 
		static_cast<int>(std::floor(start.y / tileSize)));
	sf::Vector2i endTile = sf::Vector2i(static_cast<int>(std::floor(end.x / tileSize)),
		static_cast<int>(std::floor(end.y / tileSize)));


}

std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path)
{
	std::string fileContents = ReadWholeFile(path).value();
	std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>();
	TileType asciiToTileType[128];
	asciiToTileType['_'] = TileType::empty;
	asciiToTileType['#'] = TileType::wall;

	if (fileContents.size() < 2)
	{
		return tileMap;
	}

	size_t found = fileContents.find_first_of('\n');

	tileMap->width = static_cast<int>(found != std::string::npos ? found : fileContents.size());

	for (int i = 0; i < fileContents.size(); ++i)
	{
		if (fileContents[i] == '\n')
		{
			tileMap->height++;
		}
		else
		{
			tileMap->tiles.push_back(asciiToTileType[fileContents[i]]);
		}
	}

#ifndef NDEBUG
	tileMap->debugtiles.resize(tileMap->tiles.size());
#endif

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

	bool succ = WriteWholeFile(path, data).value();

	assert(succ);

	return true;
}