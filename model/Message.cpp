#include "Message.h"

using namespace ISXModel;

Message::Message()
		: m_id(0)
		, m_content("")
		, m_sender_id(0)
		, m_chat_id(0)
		, m_timestamp("")
{}

Message::Message(const std::string& content, unsigned long sender_id, unsigned long chat_id)
		: Message(0, content, sender_id, chat_id, "")
{}

Message::Message(unsigned long id, const std::string& content, unsigned long sender_id, unsigned long chat_id, const std::string& timestamp)
		: m_id(id)
		, m_content(content)
		, m_sender_id(sender_id)
		, m_chat_id(chat_id)
		, m_timestamp(timestamp)
{}

unsigned long Message::get_id() const
{
	return m_id;
}

const std::string& Message::get_content() const
{
	return m_content;
}

unsigned long Message::get_sender_id() const
{
	return m_sender_id;
}

unsigned long Message::get_chat_id() const
{
	return m_chat_id;
}

const std::string& Message::get_timestamp() const
{
	return m_timestamp;
}

void Message::set_id(unsigned long id)
{
	this->m_id = id;
}

void Message::set_content(const std::string& content)
{
	this->m_content = content;
}

void Message::set_sender_id(unsigned long sender_id)
{
	this->m_sender_id = sender_id;
}

void Message::set_chat_id(unsigned long chat_id)
{
	this->m_chat_id = chat_id;
}

void Message::set_timestamp(const std::string& timestamp)
{
	this->m_timestamp = timestamp;
}

namespace ISXModel
{
std::ostream& operator<<(std::ostream& os, const Message& message)
{
	os << message.m_id << " \"" << message.m_content << "\" " << message.m_sender_id
	   << " " << message.m_chat_id << " \"" << message.m_timestamp << "\"" << std::endl;
	return os;
}
};
