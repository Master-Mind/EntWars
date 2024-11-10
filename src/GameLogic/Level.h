#pragma once
#include "TileMap.h"

#include <filesystem>
#include <memory>
#include <string>
#include <expected>

struct Level
{
	std::unique_ptr<TileMap> tileMap;
	std::string name;
	std::filesystem::path path;
	std::filesystem::path tileMapPath;
};

std::expected<std::unique_ptr<Level>, std::string> LoadLevel(const std::filesystem::path& path);
std::expected<bool, std::string> SaveLevel(const std::filesystem::path& path, std::unique_ptr<Level> &level);
std::unique_ptr<Level> MakeLevel();