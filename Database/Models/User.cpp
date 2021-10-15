#include "User.h"

using namespace ISXModel;

User::User()
		: m_id(0)
		, m_login("")
		, m_password("")
{}

User::User(const std::string& login, const std::string& password)
	: User(0, login, password, "")
{}

User::User(unsigned long id, const std::string& login, const std::string& password, const std::string& access_token)
		: m_id(id)
		, m_login(login)
		, m_password(password)
		, m_access_token(access_token)
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

const std::string& User::get_access_token() const
{
	return m_access_token;
}

void User::set_id(unsigned long id)
{
	this->m_id = id;
}

void User::set_login(const std::string& login)
{
	this->m_login = login;
}

void User::set_password(const std::string& password)
{
	this->m_password = password;
}

void User::set_access_token(const std::string& access_token)
{
	this->m_access_token = access_token;
}
