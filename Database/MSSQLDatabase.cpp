#include "MSSQLDatabase.h"

MSSQLDatabase::MSSQLDatabase()
		: m_sql_environment_handle(SQL_NULL_HENV)
		, m_sql_connection_handle(SQL_NULL_HDBC)
		, m_sql_statement_handle(SQL_NULL_HSTMT)
		, m_config_file("Database.conf")
		, m_token_generator(USER_MAX_ACCESS_TOKEN_LEN - 1)
{
	InitEnvironmentHandle();
	m_config_file.CreateIfNotExists();
	InitConnectionHandle();
}

MSSQLDatabase::~MSSQLDatabase()
{
	FreeStatementHandle();
	FreeConnectionHandle();
	FreeEnvironmentHandle();
}

ISXModel::User MSSQLDatabase::GetUserFromDB(const std::string& user_access_token, const unsigned long& user_id)
{
	CheckIfUserAccessTokenValid(user_access_token);
	const std::string user_id_str = std::to_string(user_id);

	LOG_DEBUG("Receiving user with id: " + user_id_str);
	ExecuteQuery("select u.user_id, u.login from [User] as u where u.user_id=" + user_id_str);

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("No user with id: " + user_id_str);
		throw QueryException("No such user");
	}

	return GetUserFromDB();
}

std::vector<ISXModel::User> MSSQLDatabase::GetUsersFromDBLike(const std::string& user_access_token, const std::string& search_string)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Receiving users like: \"" + search_string + "\"");
	ExecuteQuery("select u.user_id, u.login from [User] as u where u.login like \'%" + ReplaceSingleQuotes(search_string) + "%\'");

	std::vector<ISXModel::User> users;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		users.push_back(GetUserFromDB());
	}

	LOG_DEBUG(std::to_string(users.size()) + " matches");
	return users;
}

std::vector<ISXModel::User> MSSQLDatabase::GetChatParticipantsFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string chat_id_str = std::to_string(chat_id);

	LOG_DEBUG("Receiving participants from chat with id: " + chat_id_str);
	ExecuteQuery("select u.user_id, u.login from [User] as u"
				" inner join ChatParticipant as cp"
				" on cp.participant_id = u.user_id"
				" where cp.chat_id = " + chat_id_str);

	std::vector<ISXModel::User> participants;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		participants.push_back(GetUserFromDB());
	}

	LOG_DEBUG(std::to_string(participants.size()) + " results");
	return participants;
}

std::string MSSQLDatabase::GenerateUserAccessToken(const std::string& user_login, const std::string& user_password)
{
	CheckUserCredentialsInDB(user_login, user_password);

	char access_token[USER_MAX_ACCESS_TOKEN_LEN] = { 0 };
	char* access_token_ptr = access_token;

	m_token_generator.GetNextToken(access_token_ptr);

	if (!SaveUserAccessTokenToDB(user_login, access_token))
	{
		LOG_ERROR("Cannot save user access token in database");
		throw QueryException("Cannot save access token");
	}

	return access_token;
}

unsigned long MSSQLDatabase::SaveUserToDB(const ISXModel::User& user)
{
	const std::string login = ReplaceSingleQuotes(user.get_login());

	CheckIfUserExists(login);

	const std::string password = m_sha256_crypt.GenerateHash(user.get_password());

	LOG_DEBUG("Saving new user");
	ExecuteQuery("insert into [User](login, password) output inserted.user_id values(\'" + login + "\', \'" + password + "\')");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Unable to get id of new user");
		throw QueryException("Cannot save new user");
	}

	return GetUserFromDB().get_id();
}

bool MSSQLDatabase::UpdateUserLoginInDB(const std::string& user_access_token, const std::string& user_login)
{
	CheckIfUserAccessTokenValid(user_access_token);
	CheckIfUserExists(user_login);

	LOG_DEBUG("Updating user login");
	return ExecuteQuery("update u set u.login=\'" + ReplaceSingleQuotes(user_login) + "\' from [User] u where u.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& old_password, const std::string& new_password)
{
	CheckIfUserAccessTokenValid(user_access_token);
	CheckUserPasswordInDB(old_password);

	LOG_DEBUG("Updating user password");
	return ExecuteQuery("update u set u.password=\'" + m_sha256_crypt.GenerateHash(new_password) + "\' from [User] u"
					   " where u.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::AddUserToChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id)
{
	const ISXModel::User user = GetUserByAccessToken(user_access_token);
	const std::string chat_id_str = std::to_string(chat_id);

	if (ChatHaveParticipants(chat_id_str) && !IsUserParticipantOfChat(user.get_login(), chat_id_str))
	{
		LOG_ERROR("User with id: " + std::to_string(user.get_id()) + " is not participant of chat with id: " + chat_id_str);
		throw QueryException("You cannot perform operations on this chat");
	}

	if (IsUserParticipantOfChat(user_login, chat_id_str))
	{
		LOG_ERROR("Unable to add user to the chat with id: " + chat_id_str + ", who is already participant of this chat");
		throw QueryException("This user is already participant of the chat");
	}

	LOG_DEBUG("Adding user to chat with id: " + chat_id_str);
	return ExecuteQuery("insert into ChatParticipant values(" + chat_id_str + ","
					   " (select u.user_id from [User] as u where u.login=\'" + ReplaceSingleQuotes(user_login) + "\'))");
}

bool MSSQLDatabase::RemoveUserFromChat(const std::string& user_access_token, const std::string& user_login, const unsigned long& chat_id)
{
	const ISXModel::User user = GetUserByAccessToken(user_access_token);
	const std::string chat_id_str = std::to_string(chat_id);

	if (!IsUserParticipantOfChat(user.get_login(), chat_id_str))
	{
		LOG_ERROR("User with id: " + std::to_string(user.get_id()) + " is not participant of chat with id: " + chat_id_str);
		throw QueryException("You cannot perform operations on this chat");
	}

	LOG_DEBUG("Deleting user from chat with id: " + chat_id_str);
	bool success = ExecuteQuery("delete cp from ChatParticipant as cp"
						" where cp.chat_id = " + chat_id_str +
						" AND cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + ReplaceSingleQuotes(user_login) + "\')");

	if (!ChatHaveParticipants(chat_id_str))
	{
		RemoveChatFromDB(user_access_token, chat_id);
	}

	return success;
}

bool MSSQLDatabase::RemoveUserAccessToken(const std::string& user_access_token)
{
	LOG_DEBUG("Removing user access token");
	return ExecuteQuery("update u set u.access_token=NULL from [User] u where u.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::RemoveUserFromDB(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Deleting user");
	return ExecuteQuery("delete u from [User] as u where u.access_token=\'" + user_access_token + "\'");
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB(const std::string& user_access_token, const unsigned long& message_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string message_id_str = std::to_string(message_id);

	LOG_DEBUG("Receiving message with id: " + message_id_str);
	ExecuteQuery("select m.message_id, m.content,"
				" (select u.login from [User] as u where u.user_id = m.sender_id) as sender,"
				" m.chat_id, m.timestamp"
				" from Message as m"
				" where m.message_id=" + message_id_str);

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("No message with id: " + message_id_str);
		throw QueryException("No such message");
	}

	return GetMessageFromDB();
}

std::vector<ISXModel::Message> MSSQLDatabase::GetChatMessagesFromDB(const std::string& user_access_token,
																	const unsigned long& chat_id,
																	const unsigned long& last_message_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string chat_id_str = std::to_string(chat_id);

	LOG_DEBUG("Receiving messages from chat with id: " + chat_id_str);
	ExecuteQuery("select m.message_id, m.content,"
				" (select u.login from [User] as u where u.user_id = m.sender_id) as sender,"
				" m.chat_id, m.timestamp"
				" from Message as m"
				" inner join Chat as c"
				" on c.chat_id = m.chat_id"
				" where m.chat_id = " + chat_id_str +
				" and m.message_id > " + std::to_string(last_message_id));

	std::vector<ISXModel::Message> messages;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		messages.push_back(GetMessageFromDB());
	}

	LOG_DEBUG(std::to_string(messages.size()) + " results");
	return messages;
}

unsigned long MSSQLDatabase::SaveMessageToDB(const std::string& user_access_token, const ISXModel::Message& message)
{
	const ISXModel::User sender = GetUserByAccessToken(user_access_token);
	const std::string chat_id_str = std::to_string(message.get_chat_id());

	if (!IsUserParticipantOfChat(sender.get_login(), chat_id_str))
	{
		LOG_ERROR("User with id: " + std::to_string(sender.get_id()) + " is not participant of chat with id: " + chat_id_str);
		throw QueryException("You are not participant of the chat");
	}

	std::string content = ReplaceSingleQuotes(message.get_content());

	LOG_DEBUG("Saving new message");
	ExecuteQuery("insert into Message([content], sender_id, chat_id) output inserted.message_id"
					   " values(\'" + content + "\', " + std::to_string(sender.get_id()) + ", " + chat_id_str + ")");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Unable to get id of new message");
		throw QueryException("Cannot save new message");
	}

	return GetMessageFromDB().get_id();
}

bool MSSQLDatabase::UpdateMessageContentInDB(const std::string& user_access_token, const unsigned long& message_id, const std::string& new_content)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Updating message content");
	return ExecuteQuery("update m set m.[content]=\'" + ReplaceSingleQuotes(new_content) + "\' from Message m"
					   " where m.message_id=" + std::to_string(message_id));
}

bool MSSQLDatabase::RemoveMessageFromDB(const std::string& user_access_token, const unsigned long& message_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string message_id_str = std::to_string(message_id);

	LOG_DEBUG("Deleting message with id: " + message_id_str);
	return ExecuteQuery("delete m from Message as m where m.message_id=" + message_id_str);
}

ISXModel::Chat MSSQLDatabase::GetChatFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string chat_id_str = std::to_string(chat_id);

	LOG_DEBUG("Receiving chat with id: " + chat_id_str);
	ExecuteQuery("select * from Chat as c where c.chat_id=" + chat_id_str);

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("No chat with id: " + chat_id_str);
		throw QueryException("No such chat");
	}

	return GetChatFromDB();
}

std::vector<ISXModel::Chat> MSSQLDatabase::GetUserChatsFromDB(const std::string& user_access_token)
{
	const std::string user_id_str = std::to_string(GetUserByAccessToken(user_access_token).get_id());

	LOG_DEBUG("Receiving chats for user with id: " + user_id_str);
	ExecuteQuery("select c.* from Chat as c"
				" inner join ChatParticipant as cp"
				" on cp.chat_id = c.chat_id"
				" where cp.participant_id = " + user_id_str);

	std::vector<ISXModel::Chat> chats;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		chats.push_back(GetChatFromDB());
	}

	LOG_DEBUG(std::to_string(chats.size()) + " results");
	return chats;
}

unsigned long MSSQLDatabase::SaveChatToDB(const std::string& user_access_token, const ISXModel::Chat& chat)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string title = ReplaceSingleQuotes(chat.get_title());

	LOG_DEBUG("Saving new chat");
	ExecuteQuery("insert into Chat(title) output inserted.chat_id values(\'" + title + "\')");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Unable to get id of new chat");
		throw QueryException("Cannot save new chat");
	}

	const unsigned long saved_chat_id = GetChatFromDB().get_id();

	AddUserToChat(user_access_token, GetUserByAccessToken(user_access_token).get_login(), saved_chat_id);

	return saved_chat_id;
}

bool MSSQLDatabase::UpdateChatTitleInDB(const std::string& user_access_token, const unsigned long& chat_id, const std::string& new_title)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Updating chat title");
	return ExecuteQuery("update c set c.title=\'" + ReplaceSingleQuotes(new_title) + "\' from Chat c"
					   " where c.chat_id=" + std::to_string(chat_id));
}

bool MSSQLDatabase::RemoveChatFromDB(const std::string& user_access_token, const unsigned long& chat_id)
{
	CheckIfUserAccessTokenValid(user_access_token);

	const std::string chat_id_str = std::to_string(chat_id);

	LOG_DEBUG("Deleting chat with id: " + chat_id_str);
	return ExecuteQuery("delete c from Chat as c where c.chat_id=" + chat_id_str);
}

void MSSQLDatabase::InitEnvironmentHandle()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_sql_environment_handle) != SQL_SUCCESS)
	{
		LOG_FATAL("Error allocating environment handle");
		throw std::runtime_error("Error allocating environment handle");
	}

	if (SQLSetEnvAttr(m_sql_environment_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0) != SQL_SUCCESS)
	{
		LOG_FATAL("Error setting environment attribute");
		throw std::runtime_error("Error setting environment attribute");
	}
}

void MSSQLDatabase::InitConnectionHandle()
{
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_sql_environment_handle, &m_sql_connection_handle) != SQL_SUCCESS)
	{
		LOG_FATAL("Error allocating connection handle");
		throw std::runtime_error("Error allocating connection handle");
	}

	std::string connection_string = m_config_file.GetStringWithDelimeter(';');

	if (connection_string.empty())
	{
		LOG_FATAL("The contents of the configuration file are empty");
		throw std::runtime_error("The contents of the configuration file are empty");
	}

	SQLCHAR sql_connection_string[SQL_CONNECTION_STRING_LEN] = { 0 };
	strcpy_s((char*)sql_connection_string, SQL_CONNECTION_STRING_LEN, connection_string.c_str());

	LOG_DEBUG("Connecting to SQL Server");
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
		LOG_DEBUG("Successfully connected to SQL Server");
		break;
	case SQL_INVALID_HANDLE:
	case SQL_ERROR:
		LOG_FATAL("Could not connect to SQL Server");
		throw std::runtime_error("Could not connect to SQL Server");
	}
}

void MSSQLDatabase::InitStatementHandle()
{
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_sql_connection_handle, &m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_FATAL("Error allocating statement handle");
		throw std::runtime_error("Error allocating statement handle");
	}
}

void MSSQLDatabase::FreeEnvironmentHandle()
{
	SQLFreeHandle(SQL_HANDLE_ENV, m_sql_environment_handle);
	m_sql_environment_handle = SQL_NULL_HENV;
}

void MSSQLDatabase::FreeConnectionHandle()
{
	LOG_DEBUG("Disconnecting from SQL Server");
	SQLDisconnect(m_sql_connection_handle);
	LOG_DEBUG("Successfully disconnected from SQL Server");

	SQLFreeHandle(SQL_HANDLE_DBC, m_sql_connection_handle);
	m_sql_connection_handle = SQL_NULL_HDBC;
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
	char login[USER_MAX_LOGIN_LEN] = { 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, login, USER_MAX_LOGIN_LEN, nullptr);

	return ISXModel::User(id, login);
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB() const
{
	unsigned long id = 0;
	char content[MESSAGE_MAX_CONTENT_LEN] = { 0 };
	char sender[USER_MAX_LOGIN_LEN] = { 0 };
	unsigned long chat_id = 0;
	char timestamp[MESSAGE_MAX_TIMESTAMP_LEN] = { 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, content, MESSAGE_MAX_CONTENT_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 3, SQL_C_CHAR, sender, USER_MAX_LOGIN_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 4, SQL_C_ULONG, &chat_id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 5, SQL_C_CHAR, timestamp, MESSAGE_MAX_TIMESTAMP_LEN, nullptr);

	return ISXModel::Message(id, content, sender, chat_id, timestamp);
}

ISXModel::Chat MSSQLDatabase::GetChatFromDB() const
{
	unsigned long id = 0;
	char title[CHAT_MAX_TITLE_LEN] = { 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, title, CHAT_MAX_TITLE_LEN, nullptr);

	return ISXModel::Chat(id, title);
}

void MSSQLDatabase::CheckUserCredentialsInDB(const std::string& user_login, const std::string& user_password)
{
	LOG_DEBUG("Checking user credentials");
	ExecuteQuery("select u.user_id from [User] as u"
				" where u.login=\'" + ReplaceSingleQuotes(user_login) + "\'"
				" and u.password=\'" + m_sha256_crypt.GenerateHash(user_password) + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("User login or password does not match the value in database");
		throw QueryException("Invalid login or password");
	}
}

void MSSQLDatabase::CheckUserPasswordInDB(const std::string& user_password)
{
	LOG_DEBUG("Checking current user password");
	ExecuteQuery("select u.user_id from [User] as u where u.password=\'" + m_sha256_crypt.GenerateHash(user_password) + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Current user password does not match the value in database");
		throw QueryException("Invalid password");
	}
}

void MSSQLDatabase::CheckIfUserExists(const std::string& user_login)
{
	LOG_DEBUG("Checking if user login exists");
	ExecuteQuery("select u.user_id from [User] as u where u.login=\'" + ReplaceSingleQuotes(user_login) + "\'");

	if (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		LOG_ERROR("User login already exists");
		throw QueryException("Such user login already exists");
	}
}

void MSSQLDatabase::CheckIfUserAccessTokenValid(const std::string& user_access_token)
{
	LOG_DEBUG("Checking if user access token is valid");
	ExecuteQuery("select u.user_id, u.login from [User] as u where u.access_token=\'" + user_access_token + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Invalid user access token");
		throw QueryException("Invalid access token");
	}
}

ISXModel::User MSSQLDatabase::GetUserByAccessToken(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Receiving user by access token");
	return GetUserFromDB();
}

bool MSSQLDatabase::SaveUserAccessTokenToDB(const std::string& user_login, const std::string& user_access_token)
{
	LOG_DEBUG("Saving user access token");
	return ExecuteQuery("update u set u.access_token=\'" + user_access_token + "\' from [User] u where u.login=\'" + ReplaceSingleQuotes(user_login) + "\'");
}

bool MSSQLDatabase::IsUserParticipantOfChat(const std::string& user_login, const std::string& chat_id_str)
{
	LOG_DEBUG("Checking if user is participant of chat with id: " + chat_id_str);
	ExecuteQuery("select * from ChatParticipant as cp"
				" where cp.chat_id = " + chat_id_str +
				" AND cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + ReplaceSingleQuotes(user_login) + "\')");

	return SQLFetch(m_sql_statement_handle) == SQL_SUCCESS;
}

bool MSSQLDatabase::ChatHaveParticipants(const std::string& chat_id_str)
{
	LOG_DEBUG("Checking if chat with id: " + chat_id_str + " have participants");
	ExecuteQuery("select count(*) from ChatParticipant as cp"
				" where cp.chat_id = " + chat_id_str +
				" group by cp.chat_id"
				" having count(*) > 0");

	return SQLFetch(m_sql_statement_handle) == SQL_SUCCESS;
}
