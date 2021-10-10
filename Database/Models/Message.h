#pragma once

#include <string>

#define MESSAGE_CONTENT_LEN 255
#define MESSAGE_TIMESTAMP_LEN 24

namespace ISXModel
{
class Message
{
public:
	Message();
	Message(const std::string& content, unsigned long chat_id);
	Message(unsigned long id, const std::string& content, unsigned long sender_id, unsigned long chat_id, const std::string& timestamp);

	unsigned long get_id() const;
	const std::string& get_content() const;
	unsigned long get_sender_id() const;
	unsigned long get_chat_id() const;
	const std::string& get_timestamp() const;
	void set_id(unsigned long id);
	void set_content(const std::string& content);
	void set_sender_id(unsigned long sender_id);
	void set_chat_id(unsigned long chat_id);
	void set_timestamp(const std::string& timestamp);

private:
	unsigned long m_id;
	std::string m_content;
	unsigned long m_sender_id;
	unsigned long m_chat_id;
	std::string m_timestamp;
};
}; // namespace ISXModel
