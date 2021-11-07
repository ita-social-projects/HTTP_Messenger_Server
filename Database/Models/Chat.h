#pragma once

#include <string>

constexpr auto CHAT_MAX_TITLE_LEN = 51;
constexpr auto CHAT_MAX_IMAGE_LEN = INT_MAX;

namespace ISXModel
{
class Chat
{
public:
	Chat() = delete;
	Chat(const std::wstring& title);
	Chat(unsigned long id, const std::wstring& title);
	Chat(unsigned long id, const std::wstring& title, const std::string& image);

	unsigned long get_id() const;
	const std::wstring& get_title() const;
	const std::string& get_image() const;

private:
	unsigned long m_id;
	std::wstring m_title;
	std::string m_image;
};
}; // namespace ISXModel
