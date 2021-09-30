#pragma once

#include <cstring>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

#include "IDatabase.h"
#include "ConfigFile.h"

#define SQL_CONNECTION_STRING_LEN 1024

class MSSQLDatabase final : public IDatabase
{
public:
	MSSQLDatabase();
	~MSSQLDatabase();

	ISXModel::User GetUserFromDB(const std::string& user_login) override;
	std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& chat_title) override;
	bool CheckUser(const ISXModel::User& user) override;
	bool SaveUserToDB(const ISXModel::User& user) override;
	bool AddUserToChat(const std::string& user_login, const std::string& chat_title) override;
	bool RemoveUserFromChat(const std::string& user_login, const std::string& chat_title) override;
	bool RemoveUserFromDB(const std::string& user_login) override;

	ISXModel::Message GetMessageFromDB(const unsigned long& message_id) override;
	std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& chat_title) override;
	bool SaveMessageToDB(const ISXModel::Message& message) override;
	bool RemoveMessageFromDB(const unsigned long& message_id) override;

	ISXModel::Chat GetChatFromDB(const std::string& chat_title) override;
	std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_login) override;
	bool SaveChatToDB(const ISXModel::Chat& chat) override;
	bool RemoveChatFromDB(const std::string& chat_title) override;

private:
	void InitEnvironmentHandle();
	void InitConnectionHandle();
	void InitStatementHandle();
	void FreeStatementHandle();
	bool ExecuteQuery(const std::string& query);
	ISXModel::User GetUserFromDB() const;
	ISXModel::Message GetMessageFromDB() const;
	ISXModel::Chat GetChatFromDB() const;

	SQLHANDLE m_sql_environment_handle;
	SQLHANDLE m_sql_connection_handle;
	SQLHANDLE m_sql_statement_handle;
	ConfigFile m_config_file;
};

