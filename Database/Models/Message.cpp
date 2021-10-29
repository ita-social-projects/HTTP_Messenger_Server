#include "Message.h"

using namespace ISXModel;

Message::Message(const std::wstring& content, unsigned long chat_id)
		: Message(0, content, "", chat_id, "")
{}

Message::Message(unsigned long id, const std::wstring& content, const std::string& sender, unsigned long chat_id, const std::string& timestamp)
		: m_id(id)
		, m_content(content)
		, m_sender(sender)
		, m_chat_id(chat_id)
		, m_timestamp(timestamp)
{}

unsigned long Message::get_id() const
{
	return m_id;
}

const std::wstring& Message::get_content() const
{
	return m_content;
}

const std::string& Message::get_sender() const
{
	return m_sender;
}

unsigned long Message::get_chat_id() const
{
	return m_chat_id;
}

const std::string& Message::get_timestamp() const
{
	return m_timestamp;
}
