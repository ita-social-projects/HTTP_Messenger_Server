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
#include "model/Message.h"
#include "model/Chat.h"

#define SQL_CONNECTION_STRING_LEN 1024

class MSSQLDatabase
{
public:
	MSSQLDatabase();
	~MSSQLDatabase();

	ISXModel::User GetUserFromDB(const std::string& user_login);
	std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& chat_title);
	bool CheckUser(const ISXModel::User& user);
	bool SaveUserToDB(const ISXModel::User& user);
	bool AddUserToChat(const std::string& user_login, const std::string& chat_title);
	bool RemoveUserFromChat(const std::string& user_login, const std::string& chat_title);
	bool RemoveUserFromDB(const std::string& user_login);

	ISXModel::Message GetMessageFromDB(const unsigned long& message_id);
	std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& chat_title);
	bool SaveMessageToDB(const ISXModel::Message& message);
	bool RemoveMessageFromDB(const unsigned long& message_id);

	ISXModel::Chat GetChatFromDB(const std::string& chat_title);
	std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_login);
	bool SaveChatToDB(const ISXModel::Chat& chat);
	bool RemoveChatFromDB(const std::string& chat_title);

private:
	void InitEnvironmentHandle();
	void InitConnectionHandle();
	void InitStatementHandle();
	void GetConnectionStringFromFile(const std::string& filename, SQLCHAR** output_ptr) const;
	bool ExecuteQuery(const std::string& query);
	ISXModel::User GetUserFromDB() const;
	ISXModel::Message GetMessageFromDB() const;
	ISXModel::Chat GetChatFromDB() const;

	SQLHANDLE m_sql_environment_handle;
	SQLHANDLE m_sql_connection_handle;
	SQLHANDLE m_sql_statement_handle;
};

