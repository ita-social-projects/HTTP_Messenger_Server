#pragma once

#include <string>

#define USER_LOGIN_LEN 21
#define USER_PASSWORD_LEN 65
#define USER_ACCESS_TOKEN_LEN 33

namespace ISXModel
{
class User
{
public:
	User() = delete;
	User(const std::string& login, const std::string& password);
	User(unsigned long id, const std::string& login);

	unsigned long get_id() const;
	const std::string& get_login() const;
	const std::string& get_password() const;

private:
	unsigned long m_id;
	std::string m_login;
	std::string m_password;
};
}; // namespace ISXModel
