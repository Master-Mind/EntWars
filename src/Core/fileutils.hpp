#pragma once
#include <filesystem>

#define HANDLE_FILE_ERR(err) \
switch (err)\
{\
case 0:\
	break;\
case ENOENT:\
	return std::unexpected("File not found");\
case EACCES:\
	return std::unexpected("Access denied");\
case EINVAL:\
	return std::unexpected("Invalid mode");\
case EEXIST:\
	return std::unexpected("File already exists");\
case ENOMEM:\
	return std::unexpected("Out of memory");\
case EPERM:\
	return std::unexpected("Operation not permitted");\
case ESRCH:\
	return std::unexpected("No such process");\
case EAGAIN:\
	return std::unexpected("Resource temporarily unavailable");\
default:\
	return std::unexpected("Unknown error");\
}

inline std::expected<std::string, std::string> ReadWholeFile(const std::filesystem::path& path)
{
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, path.string().c_str(), "rb");

	HANDLE_FILE_ERR(err);

	size_t fileSize = file_size(path);

	char* buffer = new char[fileSize + 1];

	size_t numread = fread_s(buffer, fileSize, 1, fileSize, file);

	if (numread != fileSize)
	{
		delete[] buffer;
		fclose(file);
		return std::unexpected("Failed to read file");
	}

	std::string result(buffer, fileSize);

	delete[] buffer;
	fclose(file);

	return result;
}

inline std::expected<bool, std::string> WriteWholeFile(const std::filesystem::path& path, const std::string& contents)
{
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, path.string().c_str(), "wb");

	HANDLE_FILE_ERR(err);

	size_t numwritten = fwrite(contents.c_str(), 1, contents.size(), file);
	fclose(file);

	if (numwritten != contents.size())
	{
		return std::unexpected("Failed to write file");
	}

	return true;
}

inline std::expected<bool, std::string> MakeNewFile(const std::filesystem::path& path)
{
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, path.string().c_str(), "wb");

	HANDLE_FILE_ERR(err);

	fclose(file);

	return true;
}