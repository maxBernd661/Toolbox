#pragma once
#include <vector>
#include "Message.h"
#include <span>

class MessageStore
{
public:

	MessageStore() {
		messages = {};
		messages.emplace_back(L"ToolBox", MessageType::System);
	}

	void AddUser(const std::wstring_view message);
	void AddSystem(const std::wstring_view message);
	std::span<const Message> Get(size_t amt);
	const Message& GetLast() const;

private:
	std::vector<Message> messages;
};


