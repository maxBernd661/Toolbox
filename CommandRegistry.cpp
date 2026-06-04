#include "CommandRegistry.h"
#include <Windows.h>
#include "FileHelper.h"
#include <cstring>

CommandResult Exit(CommandParams args)
{
	PostQuitMessage(0);
	return {};
}	

CommandResult ListDir(CommandParams args)
{
	if (args.size() < 2)
	{
		return { L"path required." };
	}
	try
	{
		CommandResult output = {};
		std::wstring_view path = args[1];

		DirectoryData data = FileHelper::GetEntries(path);
		for (const auto& entry : data) 
		{
			output.push_back(entry.path().wstring());
		}

		return output;

	}
	catch (const std::exception& ex)
	{
		std::wstring message(ex.what(), ex.what() + std::strlen(ex.what()));
		return { L"Error", message};
	}
}

CommandResult ShowHelp(CommandParams args)
{
	std::vector<std::wstring> result = { L"available commands:" };
	for(const TBCommand& command: CommandRegistry::Instance().GetCommands())
	{
		result.push_back(command.ToString());
	}

	return result;
}

void CommandRegistry::Build()
{
	Add(TBCommand(L"help", L"shows this message", ShowHelp));
	Add(TBCommand(L"exit", L"closes the application", Exit));
	Add(TBCommand(L"ls", L"lists files in the current directory", ListDir));
}