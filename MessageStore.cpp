#include "MessageStore.h"

void MessageStore::AddUser(const std::wstring_view message)
{
	messages.emplace_back(message, MessageType::User);
}

void MessageStore::AddSystem(const std::wstring_view message)
{
	messages.emplace_back(message, MessageType::System);
}

std::span<const Message> MessageStore::Get(size_t amt)
{
	const size_t count = std::min(amt, messages.size());
	
	return std::span<const Message>(messages.data() + messages.size() - count, count);
}

const Message& MessageStore::GetLast() const
{
	return messages.back();
}

