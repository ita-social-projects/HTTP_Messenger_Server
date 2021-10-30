#pragma once

#include <string>

constexpr auto CHAT_MAX_TITLE_LEN = 51;

namespace ISXModel
{
class Chat
{
public:
	Chat() = delete;
	Chat(const std::wstring& title);
	Chat(unsigned long id, const std::wstring& title);

	unsigned long get_id() const;
	const std::wstring& get_title() const;

private:
	unsigned long m_id;
	std::wstring m_title;
};
}; // namespace ISXModel
