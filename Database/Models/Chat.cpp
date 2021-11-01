#include "Chat.h"

using namespace ISXModel;

Chat::Chat(const std::wstring& title)
		: Chat(0, title)
{}

Chat::Chat(unsigned long id, const std::wstring& title)
		: m_id(id)
		, m_title(title)
{}

unsigned long Chat::get_id() const
{
	return m_id;
}

const std::wstring& Chat::get_title() const
{
	return m_title;
}
