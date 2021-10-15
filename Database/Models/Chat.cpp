#include "Chat.h"

using namespace ISXModel;

Chat::Chat(const std::string& title)
		: Chat(0, title)
{}

Chat::Chat(unsigned long id, const std::string& title)
		: m_id(id)
		, m_title(title)
{}

unsigned long Chat::get_id() const
{
	return m_id;
}

const std::string& Chat::get_title() const
{
	return m_title;
}
