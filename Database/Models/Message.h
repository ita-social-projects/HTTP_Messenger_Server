#pragma once

#include <string>

constexpr auto MESSAGE_MAX_CONTENT_LEN = 255;
constexpr auto MESSAGE_MAX_TIMESTAMP_LEN = 24;

namespace ISXModel
{
class Message
{
public:
	Message() = delete;
	Message(const std::wstring& content, unsigned long chat_id);
	Message(unsigned long id, const std::wstring& content, const std::string& sender, unsigned long chat_id, const std::string& timestamp);

	unsigned long get_id() const;
	const std::wstring& get_content() const;
	const std::string& get_sender() const;
	unsigned long get_chat_id() const;
	const std::string& get_timestamp() const;

private:
	unsigned long m_id;
	std::wstring m_content;
	std::string m_sender;
	unsigned long m_chat_id;
	std::string m_timestamp;
};
}; // namespace ISXModel
