#include "Level.h"

#include "TileMap.h"
#include "../Core/fileutils.hpp"

#include <rfl.hpp>
#include <rfl/json.hpp>

struct LevelDef
{
	std::filesystem::path tileMapPath;
};

std::expected< std::unique_ptr<Level>, std::string> LoadLevel(const std::filesystem::path& path)
{
	std::string fileContents = ReadWholeFile(path).value();
	LevelDef def = rfl::json::read<LevelDef>(fileContents).value();

	auto tileMapResult = LoadTileMap(def.tileMapPath);

	if (!tileMapResult)
	{
		return std::unexpected(tileMapResult.error());
	}

	std::unique_ptr<Level> level = std::make_unique<Level>();

	level->tileMap = std::move(tileMapResult.value());
	level->name = path.stem().string();
	level->path = path;
	level->tileMapPath = def.tileMapPath;

	return level;
}

std::expected<bool, std::string> SaveLevel(const std::filesystem::path& path, std::unique_ptr<Level> &level)
{
	LevelDef def;

	def.tileMapPath = level->tileMapPath;

	auto data = rfl::json::write(def);

	auto result = WriteWholeFile(path, data);

	if (!result.has_value())
	{
		return std::unexpected(result.error());
	}

	result = SaveTileMap(level->tileMapPath, *level->tileMap);

	if (!result.has_value())
	{
		return std::unexpected(result.error());
	}

	return true;
}

std::unique_ptr<Level> MakeLevel()
{
	std::unique_ptr<Level> level = std::make_unique<Level>();

	level->tileMap = std::make_unique<TileMap>();

	return level;
}

