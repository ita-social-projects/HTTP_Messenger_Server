#pragma once

#include <string>

#define USER_LOGIN_LEN 21
#define USER_PASSWORD_LEN 21
#define USER_ACCESS_TOKEN_LEN 33

namespace ISXModel
{
class User
{
public:
	User();
	User(const std::string& login, const std::string& password);
	User(unsigned long id, const std::string& login, const std::string& password, const std::string& access_token);

	unsigned long get_id() const;
	const std::string& get_login() const;
	const std::string& get_password() const;
	const std::string& get_access_token() const;
	void set_id(unsigned long id);
	void set_login(const std::string& login);
	void set_password(const std::string& password);
	void set_access_token(const std::string& access_token);

private:
	unsigned long m_id;
	std::string m_login;
	std::string m_password;
	std::string m_access_token;
};
}; // namespace ISXModel
