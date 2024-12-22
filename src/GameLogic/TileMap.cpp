#include "TileMap.h"
#include "../pch.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <EASTL/hash_set.h>
#include <EASTL/priority_queue.h>

#include "../Core/fileutils.hpp"

const float tileCosts[TileType::count] = { 0.0f, 1.1f, 0.0f, 1.1f, 1.1f };
const float TileMap::tileSize = 10;
const std::array<sf::Color, TileType::count> tileColors =
{
	sf::Color::Magenta,
	sf::Color(135,62,35),
	sf::Color::Magenta,
	sf::Color::Blue, 
	sf::Color::Red
};
const std::array<const char, TileType::count> tileChars = { '_', '#', '!', 'B', 'R' };
const float hweight = 1.5f;

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

#ifndef NDEBUG
	debugtiles.resize(newWidth * newHeight);
#endif

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

struct PathNode
{
	sf::Vector2i pos;
	float g;
	float h;
	float f;

	bool operator<(const PathNode& other) const
	{
		return f > other.f;
	}
};

static float EuclideanHeuristic(const sf::Vector2i &cur, const sf::Vector2i &end)
{
	return std::sqrtf(static_cast<float>((cur.x - end.x) * (cur.x - end.x) + (cur.y - end.y) * (cur.y - end.y)));
}

//octile takes ~30% longer than euclidean in the worst case (full map search) and I don't know why ._.
static float OctileHeuristic(const sf::Vector2i& cur, const sf::Vector2i& end)
{
	int dx = abs(cur.x - end.x);
	int dy = abs(cur.y - end.y);

	return dx + dy + (std::numbers::sqrt2_v<float> - 2) * std::min(dx, dy);
}

// Provide a custom hash function for sf::Vector2i
namespace eastl {
	template <>
	struct hash<sf::Vector2i> {
		size_t operator()(const sf::Vector2i& v) const {
			size_t h1 = hash<int>{}(v.x);
			size_t h2 = hash<int>{}(v.y);
			return h1 ^ (h2 << 1); // Combine the two hashes
		}
	};
}

std::vector<sf::Vector2f> TileMap::AStarSearch(sf::Vector2f start, sf::Vector2f end, bool debug)
{
	PathNode startTile = {
		sf::Vector2i(static_cast<int>(std::floor(start.x / tileSize)),
		static_cast<int>(std::floor(start.y / tileSize))),
		0.0f,
		0.0f,
		0.0f
	};
	sf::Vector2i endTile = sf::Vector2i(static_cast<int>(std::floor(end.x / tileSize)),
		static_cast<int>(std::floor(end.y / tileSize)));
	thread_local eastl::priority_queue<PathNode> openSet; //threadlocal to minimize memory allocations
	//thread_local eastl::hash_set<sf::Vector2i> closedSet;
	thread_local std::vector<sf::Vector2i> cameFrom(width * height);
	thread_local std::vector<float> costSoFar(width * height);
	thread_local std::vector<bool> closed(width * height);
	thread_local std::vector<bool> opened(width * height);

	cameFrom.resize(width * height);
	costSoFar.resize(width * height);
	closed.clear();
	closed.resize(width * height, false);
	opened.clear();
	opened.resize(width * height, false);
	//closedSet.clear();

	//empty the open set in case it wasn't last time
	while (!openSet.empty())
	{
		openSet.pop();
	}

#ifndef NDEBUG
	if (debug)
	{
		debugtiles.clear();
		debugtiles.resize(width * height, false);
	}
#endif

	openSet.push(startTile);
	cameFrom[GetIndex(startTile.pos.x, startTile.pos.y)] = startTile.pos;
	costSoFar[GetIndex(startTile.pos.x, startTile.pos.y)] = 0.0f;

	while (!openSet.empty())
	{
		PathNode current = openSet.top();
		openSet.pop();

		if (current.pos == endTile)
		{
			std::vector<sf::Vector2f> path;
			sf::Vector2i currentPos = current.pos;

			while (currentPos != startTile.pos)
			{
				path.push_back(sf::Vector2f(currentPos.x * tileSize + tileSize / 2, currentPos.y * tileSize + tileSize / 2));
				currentPos = cameFrom[GetIndex(currentPos.x, currentPos.y)];
			}

			path.push_back(start);

			return path;
		}

		//closedSet.insert(current.pos);
		closed[GetIndex(current.pos)] = true;
		opened[GetIndex(current.pos)] = false;

		// lambda so that I don't have to make a ton of arguments in a full function
		auto visitNeighbor = [&](const int x, const int y, const float cost)
			{
				sf::Vector2i neighborPos = current.pos + sf::Vector2i(x, y);

				if (!IsInBounds(neighborPos) || IsWall(neighborPos))
				{
					return;
				}

				float newG = current.g + cost;
				float newH = hweight * EuclideanHeuristic(neighborPos, endTile);
				float newF = newG + newH;
				PathNode neighbor = { neighborPos, newG, newH, newF };

				if (opened[GetIndex(neighborPos)] && newG < costSoFar[GetIndex(neighborPos)])
				{
					opened[GetIndex(neighborPos)] = false;
				}

				if (closed[GetIndex(neighborPos)] && newG < costSoFar[GetIndex(neighborPos)])
				{
					closed[GetIndex(neighborPos)] = false;
				}

				if (!closed[GetIndex(neighborPos)] && !opened[GetIndex(neighborPos)])
				{
#ifndef NDEBUG
					if (debug)
					{
						SetDebugTile(neighborPos.x, neighborPos.y, true);
					}
#endif

					costSoFar[GetIndex(neighborPos)] = newG;
					openSet.push(neighbor);
					cameFrom[GetIndex(neighborPos)] = current.pos;
					opened[GetIndex(neighborPos)] = true;
				}
			};

		bool upwall = IsWall(current.pos + sf::Vector2i(0, -1));
		bool downwall = IsWall(current.pos + sf::Vector2i(0, 1));
		bool leftwall = IsWall(current.pos + sf::Vector2i(-1, 0));
		bool rightwall = IsWall(current.pos + sf::Vector2i(1, 0));

		if (!upwall && !leftwall)
		{
			visitNeighbor(-1, -1, std::numbers::sqrt2_v<float>);
		}

		visitNeighbor(0, -1, 1);

		if (!upwall && !rightwall)
		{
			visitNeighbor(1, -1, std::numbers::sqrt2_v<float>);
		}

		visitNeighbor(-1, 0, 1);
		visitNeighbor(1, 0, 1);

		if (!downwall && !leftwall)
		{
			visitNeighbor(-1, 1, std::numbers::sqrt2_v<float>);
		}

		visitNeighbor(0, 1,1);

		if (!downwall && !rightwall)
		{
			visitNeighbor(1, 1, std::numbers::sqrt2_v<float>);
		}
	}

	return {};
}

std::expected<std::unique_ptr<TileMap>, std::string> LoadTileMap(const std::filesystem::path& path)
{
	std::string fileContents = ReadWholeFile(path).value();
	std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>();
	TileType asciiToTileType[128] = {TileType::error};
	asciiToTileType['_'] = TileType::empty;
	asciiToTileType['#'] = TileType::wall;
	asciiToTileType['R'] = TileType::redWorkerBarracks;
	asciiToTileType['B'] = TileType::blueWorkerBarracks;

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
#ifndef NDEBUG
			if (asciiToTileType[fileContents[i]] == TileType::error)
			{
				std::cerr << "Invalid character in tilemap file: " << fileContents[i] << std::endl;
			}
#endif
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