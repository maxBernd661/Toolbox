#pragma once
#include <string>
#include <string_view>

enum class MessageType
{
	System = 0,
	User = 1,
	Pipe = 2
};

inline std::wstring_view ToString(MessageType type)
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

	Message(std::wstring_view data, MessageType type) : data(data), type(type)
	{
	}

	std::wstring Get() const
	{
		std::wstring result(ToString(type));
		result += L" ";
		result += data;
		return result;
	}

private:
	std::wstring data;
	MessageType type;
};
