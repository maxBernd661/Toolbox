#include "FileHelper.h"
#include <string>

namespace fs = std::filesystem;

namespace FileHelper 
{
	DirectoryData GetEntries(std::wstring_view path)
	{
		DirectoryData output = {};

		for (const DirectoryItem& entry : fs::directory_iterator(std::wstring(path)))
		{
			output.push_back(entry);
		}

		return output;
	}
}