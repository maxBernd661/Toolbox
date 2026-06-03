#include "CommandRegistry.h"
#include <Windows.h>

CommandResult Exit(CommandParams args)
{
	PostQuitMessage(0);
	return {};
}	

CommandResult ListDir(CommandParams args)
{
	return {};
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