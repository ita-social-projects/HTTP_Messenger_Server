#pragma once

#include <string>

#define MESSAGE_CONTENT_LEN 255
#define MESSAGE_TIMESTAMP_LEN 24

namespace ISXModel
{
class Message
{
public:
	Message() = delete;
	Message(const std::string& content, unsigned long chat_id);
	Message(unsigned long id, const std::string& content, const std::string& sender, unsigned long chat_id, const std::string& timestamp);

	unsigned long get_id() const;
	const std::string& get_content() const;
	const std::string& get_sender() const;
	unsigned long get_chat_id() const;
	const std::string& get_timestamp() const;

private:
	unsigned long m_id;
	std::string m_content;
	std::string m_sender;
	unsigned long m_chat_id;
	std::string m_timestamp;
};
}; // namespace ISXModel
