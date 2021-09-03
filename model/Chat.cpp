#include "Chat.h"

Chat::Chat()
		: m_id(0)
		, m_title("")
{}

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

void Chat::set_id(unsigned long id)
{
	this->m_id = id;
}

void Chat::set_title(const std::string& title)
{
	this->m_title = title;
}

std::ostream& operator<<(std::ostream& os, const Chat& chat)
{
	os << chat.m_id << " \"" << chat.m_title << "\"" << std::endl;
	return os;
}
