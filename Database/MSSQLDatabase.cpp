#include "MSSQLDatabase.h"

MSSQLDatabase::MSSQLDatabase()
		: m_sql_environment_handle(SQL_NULL_HENV)
		, m_sql_connection_handle(SQL_NULL_HDBC)
		, m_sql_statement_handle(SQL_NULL_HSTMT)
		, m_config_file("Database.conf")
		, m_token_generator(USER_ACCESS_TOKEN_LEN - 1)
{
	InitEnvironmentHandle();
	m_config_file.CreateIfNotExists();
	InitConnectionHandle();
}

MSSQLDatabase::~MSSQLDatabase()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_sql_statement_handle);
	SQLDisconnect(m_sql_connection_handle);
	SQLFreeHandle(SQL_HANDLE_DBC, m_sql_connection_handle);
	SQLFreeHandle(SQL_HANDLE_ENV, m_sql_environment_handle);
}

ISXModel::User MSSQLDatabase::GetUserFromDB(const std::string& user_access_token, const unsigned long& user_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select * from [User] as u where u.user_id=" + std::to_string(user_id));

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("No such user");
	}

	return GetUserFromDB();
}

std::vector<ISXModel::User> MSSQLDatabase::GetUsersFromDBLike(const std::string& user_access_token, const std::string& search_string)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select * from [User] as u where u.login like \'%" + search_string + "%\'");

	std::vector<ISXModel::User> users;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		users.push_back(GetUserFromDB());
	}

	return users;
}

std::vector<ISXModel::User> MSSQLDatabase::GetChatParticipantsFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select u.* from [User] as u"
				" inner join ChatParticipant as cp"
				" on cp.participant_id = u.user_id"
				" where cp.chat_id = " + std::to_string(chat_id));

	std::vector<ISXModel::User> participants;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		participants.push_back(GetUserFromDB());
	}

	return participants;
}

std::string MSSQLDatabase::GenerateUserAccessToken(const std::string& user_login, const std::string& user_password)
{
	CheckUserCredentialsInDB(user_login, user_password);

	char access_token[USER_ACCESS_TOKEN_LEN] = { 0 };
	char *access_token_ptr = access_token;

	m_token_generator.GetNextToken(access_token_ptr);

	if (!SaveUserAccessTokenToDB(user_login, access_token))
	{
		throw QueryException("Cannot save access token");
	}

	return access_token;
}

bool MSSQLDatabase::SaveUserToDB(const ISXModel::User& user)
{
	std::string login = user.get_login();

	CheckIfUserExists(login);

	std::string password = m_sha256.GenerateHash(user.get_password());

	return ExecuteQuery("insert into [User](login, password) values(\'" + login + "\', \'" + password + "\')");
}

bool MSSQLDatabase::UpdateUserLoginInDB(const std::string& user_access_token, const std::string& user_login)
{
	CheckIfUserAccessTokenValid(user_access_token);
	CheckIfUserExists(user_login);
	return ExecuteQuery("update u set u.login=\'" + user_login + "\' from [User] u where u.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& user_password)
{
	CheckIfUserAccessTokenValid(user_access_token);
	return ExecuteQuery("update u set u.password=\'" + m_sha256.GenerateHash(user_password) + "\' from [User] u"
					   " where u.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::AddUserToChat(const std::string& user_access_token, const unsigned long& user_id, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	std::string user_id_str = std::to_string(user_id);
	std::string chat_id_str = std::to_string(chat_id);

	if (IsUserParticipantOfChat(user_id_str, chat_id_str))
	{
		throw QueryException("User is already participant of the chat");
	}

	return ExecuteQuery("insert into ChatParticipant values(" + chat_id_str + ", " + user_id_str + ")");
}

bool MSSQLDatabase::RemoveUserFromChat(const std::string& user_access_token, const unsigned long& user_id, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	return ExecuteQuery("delete cp from ChatParticipant as cp"
						" where cp.chat_id=" + std::to_string(chat_id) +
						" AND cp.participant_id=" + std::to_string(user_id));
}

bool MSSQLDatabase::RemoveUserAccessToken(const std::string& user_access_token)
{
	return ExecuteQuery("update u set u.access_token=NULL from [User] u where u.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::RemoveUserFromDB(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);
	return ExecuteQuery("delete u from [User] as u where u.access_token=\'" + user_access_token + "\'");
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB(const std::string& user_access_token, const unsigned long& message_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select * from Message as m where m.message_id=" + std::to_string(message_id));

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("No such message");
	}

	return GetMessageFromDB();
}

std::vector<ISXModel::Message> MSSQLDatabase::GetChatMessagesFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select m.* from Message as m"
				" inner join Chat as c"
				" on c.chat_id = m.chat_id"
				" where m.chat_id = " + std::to_string(chat_id));

	std::vector<ISXModel::Message> messages;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		messages.push_back(GetMessageFromDB());
	}

	return messages;
}

bool MSSQLDatabase::SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message)
{
	CheckIfUserAccessTokenValid(user_access_token);

	std::string sender_id = std::to_string(GetUserIdByAccessToken(user_access_token));
	std::string chat_id = std::to_string(message.get_chat_id());

	if (!IsUserParticipantOfChat(sender_id, chat_id))
	{
		throw QueryException("User is not participant of the chat");
	}

	std::string content = message.get_content();

	return ExecuteQuery("insert into Message([content], sender_id, chat_id)"
					   " values(\'" + content + "\', " + sender_id + ", " + chat_id + ")");
}

bool MSSQLDatabase::RemoveMessageFromDB(const std::string& user_access_token, const unsigned long& message_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	return ExecuteQuery("delete m from Message as m where m.message_id=" + std::to_string(message_id));
}

ISXModel::Chat MSSQLDatabase::GetChatFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select * from Chat as c where c.chat_id=" + std::to_string(chat_id));

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("No such chat");
	}

	return GetChatFromDB();
}

std::vector<ISXModel::Chat> MSSQLDatabase::GetUserChatsFromDB(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("select c.* from Chat as c"
				" inner join ChatParticipant as cp"
				" on cp.chat_id = c.chat_id"
				" where cp.participant_id = " + std::to_string(GetUserIdByAccessToken(user_access_token)));

	std::vector<ISXModel::Chat> chats;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		chats.push_back(GetChatFromDB());
	}

	return chats;
}

bool MSSQLDatabase::SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat)
{
	CheckIfUserAccessTokenValid(user_access_token);
	ExecuteQuery("insert into Chat(title) output inserted.chat_id values(\'" + chat.get_title() + "\')");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("New chat has not been saved");
	}

	const unsigned long saved_chat_id = GetChatFromDB().get_id();

	return AddUserToChat(user_access_token, GetUserIdByAccessToken(user_access_token), saved_chat_id);
}

bool MSSQLDatabase::RemoveChatFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	return ExecuteQuery("delete c from Chat as c where c.chat_id=" + std::to_string(chat_id));
}

void MSSQLDatabase::InitEnvironmentHandle()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_sql_environment_handle) != SQL_SUCCESS)
	{
		throw std::runtime_error("Error allocating environment handle");
	}

	if (SQLSetEnvAttr(m_sql_environment_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0) != SQL_SUCCESS)
	{
		throw std::runtime_error("Error setting environment attribute");
	}
}

void MSSQLDatabase::InitConnectionHandle()
{
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_sql_environment_handle, &m_sql_connection_handle) != SQL_SUCCESS)
	{
		throw std::runtime_error("Error allocating connection handle");
	}

	std::string connection_string = m_config_file.GetStringWithDelimeter(';');

	if (connection_string.empty())
	{
		throw std::runtime_error("The contents of the configuration file are empty");
	}

	SQLCHAR sql_connection_string[SQL_CONNECTION_STRING_LEN] = { 0 };
	strncpy((char*)sql_connection_string, connection_string.c_str(), SQL_CONNECTION_STRING_LEN);

	SQLRETURN return_code = SQLDriverConnect(m_sql_connection_handle,
							 nullptr,
							 sql_connection_string,
							 SQL_CONNECTION_STRING_LEN,
							 nullptr,
							 0,
							 nullptr,
							 SQL_DRIVER_NOPROMPT);

	switch (return_code)
	{
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		break;
	case SQL_INVALID_HANDLE:
	case SQL_ERROR:
		throw std::runtime_error("Could not connect to SQL Server");
	}
}

void MSSQLDatabase::InitStatementHandle()
{
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_sql_connection_handle, &m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw std::runtime_error("Error allocating statement handle");
	}
}

void MSSQLDatabase::FreeStatementHandle()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_sql_statement_handle);
	m_sql_statement_handle = SQL_NULL_HSTMT;
}

bool MSSQLDatabase::ExecuteQuery(const std::string& query)
{
	FreeStatementHandle(); // needed to do always
	InitStatementHandle(); // before executing each query
	return SQLExecDirect(m_sql_statement_handle, (SQLCHAR*) query.c_str(), SQL_NTS) == SQL_SUCCESS;
}

ISXModel::User MSSQLDatabase::GetUserFromDB() const
{
	unsigned long id = 0;
	char login[USER_LOGIN_LEN] = { 0 };
	char password[USER_PASSWORD_LEN] = { 0 };
	char access_token[USER_ACCESS_TOKEN_LEN] = { 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, login, USER_LOGIN_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 3, SQL_C_CHAR, password, USER_PASSWORD_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 4, SQL_C_CHAR, access_token, USER_ACCESS_TOKEN_LEN, nullptr);

	return ISXModel::User(id, login, password, access_token);
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB() const
{
	unsigned long id = 0;
	char content[MESSAGE_CONTENT_LEN] = { 0 };
	unsigned long sender_id = 0;
	unsigned long chat_id = 0;
	char timestamp[MESSAGE_TIMESTAMP_LEN] = { 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, content, MESSAGE_CONTENT_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 3, SQL_C_ULONG, &sender_id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 4, SQL_C_ULONG, &chat_id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 5, SQL_C_CHAR, timestamp, MESSAGE_TIMESTAMP_LEN, nullptr);

	return ISXModel::Message(id, content, sender_id, chat_id, timestamp);
}

ISXModel::Chat MSSQLDatabase::GetChatFromDB() const
{
	unsigned long id = 0;
	char title[CHAT_TITLE_LEN] = { 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, title, CHAT_TITLE_LEN, nullptr);

	return ISXModel::Chat(id, title);
}

void MSSQLDatabase::CheckUserCredentialsInDB(const std::string& user_login, const std::string& user_password)
{
	ExecuteQuery("select * from [User] as u"
				" where u.login=\'" + user_login + "\'"
				" and u.password=\'" + m_sha256.GenerateHash(user_password) + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("Invalid login or password");
	}
}

void MSSQLDatabase::CheckIfUserExists(const std::string& user_login)
{
	ExecuteQuery("select * from [User] as u where u.login=\'" + user_login + "\'");

	if (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		throw QueryException("User login already exists");
	}
}

void MSSQLDatabase::CheckIfUserAccessTokenValid(const std::string& user_access_token)
{
	ExecuteQuery("select u.user_id from [User] as u where u.access_token=\'" + user_access_token + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("Invalid access token");
	}
}

unsigned long MSSQLDatabase::GetUserIdByAccessToken(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);
	return GetUserFromDB().get_id();
}

bool MSSQLDatabase::SaveUserAccessTokenToDB(const std::string& user_login, const std::string& user_access_token)
{
	return ExecuteQuery("update u set u.access_token=\'" + user_access_token + "\' from [User] u where u.login=\'" + user_login + "\'");
}

bool MSSQLDatabase::IsUserParticipantOfChat(const std::string& user_id_str, const std::string& chat_id_str)
{
	ExecuteQuery("select * from ChatParticipant as cp where cp.chat_id=" + chat_id_str + " AND cp.participant_id=" + user_id_str);
	return SQLFetch(m_sql_statement_handle) == SQL_SUCCESS;
}
