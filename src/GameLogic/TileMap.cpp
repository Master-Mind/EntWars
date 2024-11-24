#include "TileMap.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
#include <cassert>

#include "../Core/fileutils.hpp"

const float tileCosts[TileType::count] = { 0.0f, 1.0f };
const float TileMap::tileSize = 100;
const std::array<sf::Color, TileType::count> tileColors = { sf::Color::Magenta, sf::Color(135,62,35)};
const std::array<const char, TileType::count> tileChars = { '_', '#' };

void TileMap::Resize(int newWidth, int newHeight)
{
	std::vector<TileType> newTypes(newWidth * newHeight);
	std::vector<std::unique_ptr<sf::RectangleShape>> newTiles(newWidth * newHeight);

	for (size_t x = 0; x < std::min(width, newWidth); ++x)
	{
		for (size_t y = 0; y < std::min(height, newHeight); ++y)
		{
			newTypes[y * newWidth + x] = GetTile(x, y);
		}
	}

	tiles.swap(newTypes);

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
		}
	}
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