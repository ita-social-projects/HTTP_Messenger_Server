#pragma once

#include <string>
#include <ostream>

#define CHAT_TITLE_LEN 51

namespace ISXModel
{
class Chat
{
public:
	Chat();
	Chat(const std::string& title);
	Chat(unsigned long id, const std::string& title);

	unsigned long get_id() const;
	const std::string& get_title() const;
	void set_id(unsigned long id);
	void set_title(const std::string& title);

private:
	friend std::ostream& operator<<(std::ostream& os, const Chat& chat);

	unsigned long m_id;
	std::string m_title;
};
};
