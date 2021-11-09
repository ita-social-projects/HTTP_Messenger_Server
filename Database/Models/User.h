#pragma once

#include <string>

constexpr auto USER_MAX_LOGIN_LEN = 21;
constexpr auto USER_MAX_ACCESS_TOKEN_LEN = 33;
constexpr auto USER_MAX_IMAGE_LEN = 5'242'881; // 5 MB

namespace ISXModel
{
class User
{
public:
	User() = delete;
	User(const std::string& login, const std::string& password);
	User(unsigned long id, const std::string& login);
	User(unsigned long id, const std::string& login, const std::string& image);

	unsigned long get_id() const;
	const std::string& get_login() const;
	const std::string& get_password() const;
	const std::string& get_image() const;

private:
	unsigned long m_id;
	std::string m_login;
	std::string m_password;
	std::string m_image;
};
}; // namespace ISXModel
