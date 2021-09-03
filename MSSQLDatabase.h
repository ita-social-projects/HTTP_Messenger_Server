#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

#include "model/User.h"
#include "model/Chat.h"

#define SQL_CONNECTION_STRING_LEN 1024

class MSSQLDatabase
{
public:
	MSSQLDatabase();
	~MSSQLDatabase();

	User GetUserFromDB(const std::string& user_login);
	bool SaveUserToDB(const User& user);
	bool AddUserToChat(const std::string& user_login, const std::string& chat_title);

	Chat GetChatFromDB(const std::string& chat_title);
	std::vector<Chat> GetUserChatsFromDB(const std::string& user_login);

private:
	void InitEnvironmentHandle();
	void InitConnectionHandle();
	void InitStatementHandle();
	void GetConnectionStringFromFile(const std::string& filename, SQLCHAR** output_ptr) const;
	bool ExecuteQuery(const std::string& query);
	User GetUserFromDB() const;
	Chat GetChatFromDB() const;

	SQLHANDLE m_sql_environment_handle;
	SQLHANDLE m_sql_connection_handle;
	SQLHANDLE m_sql_statement_handle;
};

