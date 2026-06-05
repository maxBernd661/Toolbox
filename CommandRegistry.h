#pragma once
#include <vector>
#include "TBCommand.h"

class CommandRegistry
{
public:
	static CommandRegistry& Instance() 
	{
		static CommandRegistry instance;
		return instance;
	}

	void Add(TBCommand command)
	{
		commands.push_back(std::move(command));

	}

	[[nodiscard]] const std::vector<TBCommand>& GetCommands() const
	{
		return commands;
	}

	void Build();

private:
	std::vector<TBCommand> commands;
};

