#pragma once
#include <string>
#include <string_view>
#include <functional>
#include <span>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

using CommandResult = std::vector<std::wstring>;
using CommandParams = std::span<const std::wstring>;
using CommandAction = std::function<CommandResult(CommandParams)>;

class TBCommand
{
public:
	TBCommand(const std::wstring& name, const std::wstring& description, CommandAction action)
	{
		internalName = name;
		interalDescription = description;
		internalAction = std::move(action);
	}

	std::wstring_view GetName() const { return internalName; }
	std::wstring_view GetDescription() const { return interalDescription; }

	std::wstring ToString() const 
	{
		return internalName + L" - " + interalDescription;
	}

	CommandResult Execute(const CommandParams args) const
	{
		return internalAction(args);
	}

private:
	std::wstring internalName;
	std::wstring interalDescription;
	CommandAction internalAction;
};

