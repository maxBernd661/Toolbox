#include "FileHelper.h"

namespace fs = std::filesystem;

namespace FileHelper 
{
	DirectoryData GetEntries(std::wstring_view path)
	{
		DirectoryData output = {};

		for (const DirectoryItem& entry : fs::directory_iterator(path))
		{
			output.push_back(entry);
		}

		return output;
	}
}