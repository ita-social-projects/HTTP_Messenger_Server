#include "User.h"

using namespace ISXModel;

User::User(const std::string& login, const std::string& password)
		: m_id(0)
		, m_login(login)
		, m_password(password)
		, m_image("")
{}

User::User(unsigned long id, const std::string& login)
		: User(id, login, "")
{}

User::User(unsigned long id, const std::string& login, const std::string& image)
		: m_id(id)
		, m_login(login)
		, m_password("")
		, m_image(image)
{}

unsigned long User::get_id() const
{
	return m_id;
}

const std::string& User::get_login() const
{
	return m_login;
}

const std::string& User::get_password() const
{
	return m_password;
}

const std::string& User::get_image() const
{
	return m_image;
}
