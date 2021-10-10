#pragma once

#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

#include "IDatabase.h"
#include "ConfigFile.h"
#include "Security/TokenGenerator.h"

#define SQL_CONNECTION_STRING_LEN 1024

class MSSQLDatabase final : public IDatabase
{
public:
	MSSQLDatabase();
	~MSSQLDatabase();

	ISXModel::User GetUserFromDB(const std::string& user_login, const std::string& user_password) override;
	std::vector<ISXModel::User> GetChatParticipantsFromDB(const unsigned long& chat_id) override;
	std::vector<ISXModel::User> GetUsersFromDBLike(const std::string& search_string) override;
	std::string GenerateUserAccessToken(const std::string& user_login, const std::string& user_password) override;
	bool SaveUserToDB(const ISXModel::User& user) override;
	bool AddUserToChat(const unsigned long& user_id, const unsigned long& chat_id) override;
	bool RemoveUserFromChat(const unsigned long& user_id, const unsigned long& chat_id) override;
	bool RemoveUserAccessToken(const std::string& user_access_token) override;
	bool RemoveUserFromDB(const std::string& user_access_token) override;

	ISXModel::Message GetMessageFromDB(const unsigned long& message_id) override;
	std::vector<ISXModel::Message> GetChatMessagesFromDB(const unsigned long& chat_id) override;
	bool SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message) override;
	bool RemoveMessageFromDB(const unsigned long& message_id) override;

	ISXModel::Chat GetChatFromDB(const unsigned long& chat_id) override;
	std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_access_token) override;
	bool SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat) override;
	bool RemoveChatFromDB(const unsigned long& chat_id) override;

private:
	void InitEnvironmentHandle();
	void InitConnectionHandle();
	void InitStatementHandle();
	void FreeStatementHandle();
	bool ExecuteQuery(const std::string& query);
	ISXModel::User GetUserFromDB() const;
	ISXModel::Message GetMessageFromDB() const;
	ISXModel::Chat GetChatFromDB() const;
	unsigned long GetUserIdByAccessToken(const std::string& user_access_token);
	bool SaveUserAccessTokenToDB(const std::string& user_login, const std::string& user_access_token);

	SQLHANDLE m_sql_environment_handle;
	SQLHANDLE m_sql_connection_handle;
	SQLHANDLE m_sql_statement_handle;
	ConfigFile m_config_file;
	TokenGenerator m_token_generator;
};
