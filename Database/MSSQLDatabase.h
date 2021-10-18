#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

#include "IDatabase.h"
#include "ConfigFile.h"
#include "Security/SHA256.h"
#include "Security/TokenGenerator.h"

#define SQL_CONNECTION_STRING_LEN 1024

class MSSQLDatabase final : public IDatabase
{
public:
	MSSQLDatabase();
	~MSSQLDatabase();

	ISXModel::User GetUserFromDB(const std::string& user_access_token, const unsigned long& user_id) override;
	std::vector<ISXModel::User> GetUsersFromDBLike(const std::string& user_access_token, const std::string& search_string) override;
	std::vector<ISXModel::User> GetChatParticipantsFromDB(const std::string& user_access_token, const unsigned long& chat_id) override;
	std::string GenerateUserAccessToken(const std::string& user_login, const std::string& user_password) override;
	bool SaveUserToDB(const ISXModel::User& user) override;
	bool UpdateUserLoginInDB(const std::string& user_access_token, const std::string& user_login) override;
	bool UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& old_password, const std::string& new_password) override;
	bool AddUserToChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id) override;
	bool RemoveUserFromChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id) override;
	bool RemoveUserAccessToken(const std::string& user_access_token) override;
	bool RemoveUserFromDB(const std::string& user_access_token) override;

	ISXModel::Message GetMessageFromDB(const std::string& user_access_token, const unsigned long& message_id) override;
	std::vector<ISXModel::Message> GetChatMessagesFromDB(const std::string& user_access_token, const unsigned long& chat_id) override;
	bool SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message) override;
	bool RemoveMessageFromDB(const std::string& user_access_token, const unsigned long& message_id) override;

	ISXModel::Chat GetChatFromDB(const std::string& user_access_token, const unsigned long& chat_id) override;
	std::vector<ISXModel::Chat> GetUserChatsFromDB(const std::string& user_access_token) override;
	bool SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat) override;
	bool RemoveChatFromDB(const std::string& user_access_token, const unsigned long& chat_id) override;

private:
	void InitEnvironmentHandle();
	void InitConnectionHandle();
	void InitStatementHandle();
	void FreeEnvironmentHandle();
	void FreeConnectionHandle();
	void FreeStatementHandle();
	bool ExecuteQuery(const std::string& query);
	ISXModel::User GetUserFromDB() const;
	ISXModel::Message GetMessageFromDB() const;
	ISXModel::Chat GetChatFromDB() const;
	void CheckUserCredentialsInDB(const std::string& user_login, const std::string& user_password);
	void CheckUserPasswordInDB(const std::string& user_password);
	void CheckIfUserExists(const std::string& user_login);
	void CheckIfUserAccessTokenValid(const std::string& user_access_token);
	ISXModel::User GetUserByAccessToken(const std::string& user_access_token);
	bool SaveUserAccessTokenToDB(const std::string& user_login, const std::string& user_access_token);
	bool IsUserParticipantOfChat(const std::string& user_login, const std::string& chat_id_str);
	bool ChatHaveParticipants(const std::string& chat_id_str);

	SQLHANDLE m_sql_environment_handle;
	SQLHANDLE m_sql_connection_handle;
	SQLHANDLE m_sql_statement_handle;
	ConfigFile m_config_file;
	SHA256 m_sha256;
	TokenGenerator m_token_generator;
};
