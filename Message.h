#pragma once
#include <string>
#include <format>

enum class MessageType
{
	System = 0,
	User = 1,
	Pipe = 2
};

std::wstring_view ToString(MessageType type)
{
	switch(type)
	{
	case MessageType::System:
		return L"*";
	case MessageType::User:
		return L">";
	case MessageType::Pipe:
		return L"~";
	default:
		return L"*";
	}
}

class Message
{
public:

	Message(const std::wstring& data, MessageType type) : data(data), type(type)
	{
	}

	std::wstring Get() const
	{
		return std::format(L"{} {}",ToString(type), data);
	}

private:
	std::wstring data;
	MessageType type;
};
