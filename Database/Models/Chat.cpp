#include "Chat.h"

using namespace ISXModel;

Chat::Chat(const std::wstring& title)
		: Chat(0, title, "")
{}

Chat::Chat(unsigned long id, const std::wstring& title)
		: Chat(id, title, "")
{}

Chat::Chat(unsigned long id, const std::wstring& title, const std::string& image)
		: m_id(id)
		, m_title(title)
		, m_image(image)
{}

unsigned long Chat::get_id() const
{
	return m_id;
}

const std::wstring& Chat::get_title() const
{
	return m_title;
}

const std::string& Chat::get_image() const
{
	return m_image;
}
