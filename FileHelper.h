#pragma once
#include <vector>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

using DirectoryItem = fs::directory_entry;
using DirectoryData = std::vector<DirectoryItem>;

namespace FileHelper
{
	DirectoryData GetEntries(std::wstring_view path);
};

