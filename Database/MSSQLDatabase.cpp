#include "MSSQLDatabase.h"

MSSQLDatabase::MSSQLDatabase(const std::string& config_filename)
		: m_sql_environment_handle(SQL_NULL_HENV)
		, m_sql_connection_handle(SQL_NULL_HDBC)
		, m_sql_statement_handle(SQL_NULL_HSTMT)
		, m_config_file(config_filename)
		, m_token_generator(USER_MAX_ACCESS_TOKEN_LEN - 1)
{
	m_config_file.CreateIfNotExists();
	Connect();
}

MSSQLDatabase::~MSSQLDatabase()
{
	Disconnect();
}

void MSSQLDatabase::Connect()
{
	if (m_sql_connection_handle == SQL_NULL_HDBC)
	{
		InitEnvironmentHandle();
		InitConnectionHandle();
	}
}

void MSSQLDatabase::Disconnect()
{
	FreeStatementHandle();
	FreeConnectionHandle();
	FreeEnvironmentHandle();
}

ISXModel::User MSSQLDatabase::GetUserFromDB(const std::string& user_access_token)
{
	return GetUserByAccessToken(user_access_token);
}

std::vector<ISXModel::User> MSSQLDatabase::GetUsersFromDBLike(const std::string& user_access_token, const std::string& search_string)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Receiving users like: \"" + search_string + "\"");
	ExecuteQuery("select u.user_id, u.login, u.image from [User] as u where u.login like \'%" + search_string + "%\'");

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
	ExecuteQuery("select u.user_id, u.login, u.image from [User] as u"
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

std::string MSSQLDatabase::GetUserAccessToken(const std::string& user_login, const std::string& user_password)
{
	CheckUserCredentialsInDB(user_login, user_password);

	if (UserHasToken(user_login))
	{
		return GetUserAccessTokenFromDB();
	}
	else
	{
		char user_access_token[USER_MAX_ACCESS_TOKEN_LEN]{ 0 };
		char* user_access_token_ptr = user_access_token;

		m_token_generator.GetNextToken(user_access_token_ptr);

		if (!SaveUserAccessTokenToDB(user_access_token, user_login))
		{
			LOG_ERROR("Cannot save user access token to database");
			throw std::runtime_error("Cannot save access token");
		}

		return user_access_token;
	}
}

unsigned long MSSQLDatabase::SaveUserToDB(const ISXModel::User& user)
{
	const std::string login = user.get_login();

	if (login.length() >= USER_MAX_LOGIN_LEN)
	{
		LOG_ERROR("User login exceeds the character limit");
		throw QueryException("User login exceeds length of " + std::to_string(USER_MAX_LOGIN_LEN - 1) + " characters");
	}

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

bool MSSQLDatabase::UpdateUserAccessTokenUsedDateInDB(const std::string& user_access_token)
{
	LOG_DEBUG("Updating user access token usage date");
	const bool success = ExecuteQuery("update t set t.last_used_date = CURRENT_TIMESTAMP from Token as t"
									 " where t.access_token = \'" + user_access_token + "\'");

	CheckIfUserAccessTokenValid(user_access_token);
	return success;
}

bool MSSQLDatabase::UpdateUserLoginInDB(const std::string& user_access_token, const std::string& new_login)
{
	if (new_login.length() >= USER_MAX_LOGIN_LEN)
	{
		LOG_ERROR("User login exceeds the character limit");
		throw QueryException("User login exceeds length of " + std::to_string(USER_MAX_LOGIN_LEN - 1) + " characters");
	}

	CheckIfUserAccessTokenValid(user_access_token);
	CheckIfUserExists(new_login);

	LOG_DEBUG("Updating user login");
	return ExecuteQuery("update u set u.login = \'" + new_login + "\' from [User] as u"
					   " inner join Token as t"
					   " on t.user_id = u.user_id"
					   " where t.access_token = \'" + user_access_token + "\'");
}

bool MSSQLDatabase::UpdateUserPasswordInDB(const std::string& user_access_token, const std::string& old_password, const std::string& new_password)
{
	CheckIfUserAccessTokenValid(user_access_token);
	CheckUserPasswordInDB(old_password);

	LOG_DEBUG("Updating user password");
	return ExecuteQuery("update u set u.password = \'" + m_sha256_crypt.GenerateHash(new_password) + "\' from [User] as u"
					   " inner join Token as t"
					   " on t.user_id = u.user_id"
					   " where t.access_token = \'" + user_access_token + "\'");
}

bool MSSQLDatabase::UpdateUserImageInDB(const std::string& user_access_token, const std::string& new_image_str)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Updating user image");
	return ExecuteQuery("update u set u.image = 0x" +
						 m_sha256_crypt.GetHexString((const unsigned char*) new_image_str.c_str(), new_image_str.length()) +
					   " from [User] as u" +
					   " inner join Token as t"
					   " on t.user_id = u.user_id"
					   " where t.access_token = \'" + user_access_token + "\'");
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
					   " (select u.user_id from [User] as u where u.login=\'" + user_login + "\'))");
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
	const bool success = ExecuteQuery("delete cp from ChatParticipant as cp"
									 " where cp.chat_id = " + chat_id_str +
									 " AND cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + user_login + "\')");

	if (!ChatHaveParticipants(chat_id_str))
	{
		RemoveChatFromDB(user_access_token, chat_id);
	}

	return success;
}

bool MSSQLDatabase::RemoveUserAccessToken(const std::string& user_access_token)
{
	LOG_DEBUG("Removing user access token");
	return ExecuteQuery("delete t from Token as t where t.access_token=\'" + user_access_token + "\'");
}

bool MSSQLDatabase::RemoveUserFromDB(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Deleting user");
	return ExecuteQuery("delete u from [User] as u"
						" inner join Token as t"
						" on t.user_id = u.user_id"
						" where t.access_token = \'" + user_access_token + "\'");
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB(const std::string& user_access_token, const unsigned long& message_id)
{
	if (!user_access_token.empty())
	{
		CheckIfUserAccessTokenValid(user_access_token);
	}

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
	const std::string chat_id_str = std::to_string(message.get_chat_id());
	const std::wstring content = ReplaceSingleQuotes(message.get_content());

	if (content.length() >= MESSAGE_MAX_CONTENT_LEN)
	{
		LOG_ERROR("Message content exceeds the character limit");
		throw QueryException("Message content exceeds length of " + std::to_string(MESSAGE_MAX_CONTENT_LEN - 1) + " characters");
	}

	if (user_access_token.empty())
	{
		LOG_DEBUG("Saving system message");
		ExecuteQuery(L"insert into Message([content], sender_id, chat_id) output inserted.message_id"
					" values(N\'" + content + L"\', NULL, " + to_wstring(chat_id_str) + L")");
	}
	else
	{
		const ISXModel::User sender = GetUserByAccessToken(user_access_token);

		if (!IsUserParticipantOfChat(sender.get_login(), chat_id_str))
		{
			LOG_ERROR("User with id: " + std::to_string(sender.get_id()) + " is not participant of chat with id: " + chat_id_str);
			throw QueryException("You are not participant of the chat");
		}

		LOG_DEBUG("Saving new message");
		ExecuteQuery(L"insert into Message([content], sender_id, chat_id) output inserted.message_id"
					" values(N\'" + content + L"\', " + std::to_wstring(sender.get_id()) + L", " + to_wstring(chat_id_str) + L")");
	}

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Unable to get id of new message");
		throw QueryException("Cannot save new message");
	}

	return GetMessageFromDB().get_id();
}

bool MSSQLDatabase::UpdateMessageContentInDB(const std::string& user_access_token, const unsigned long& message_id, const std::wstring& new_content)
{
	if (new_content.length() >= MESSAGE_MAX_CONTENT_LEN)
	{
		LOG_ERROR("Message content exceeds the character limit");
		throw QueryException("Message content exceeds length of " + std::to_string(MESSAGE_MAX_CONTENT_LEN - 1) + " characters");
	}

	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Updating message content");
	return ExecuteQuery(L"update m set m.[content]=N\'" + ReplaceSingleQuotes(new_content) + L"\' from Message as m"
					   " where m.message_id=" + std::to_wstring(message_id));
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
	const std::wstring title = ReplaceSingleQuotes(chat.get_title());

	if (title.length() >= CHAT_MAX_TITLE_LEN)
	{
		LOG_ERROR("Chat title exceeds the character limit");
		throw QueryException("Chat title exceeds length of " + std::to_string(CHAT_MAX_TITLE_LEN - 1) + " characters");
	}

	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Saving new chat");
	ExecuteQuery(L"insert into Chat(title) output inserted.chat_id values(N\'" + title + L"\')");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		LOG_ERROR("Unable to get id of new chat");
		throw QueryException("Cannot save new chat");
	}

	const unsigned long saved_chat_id = GetChatFromDB().get_id();

	AddUserToChat(user_access_token, GetUserByAccessToken(user_access_token).get_login(), saved_chat_id);

	return saved_chat_id;
}

bool MSSQLDatabase::UpdateChatTitleInDB(const std::string& user_access_token, const unsigned long& chat_id, const std::wstring& new_title)
{
	if (new_title.length() >= CHAT_MAX_TITLE_LEN)
	{
		LOG_ERROR("Chat title exceeds the character limit");
		throw QueryException("Chat title exceeds length of " + std::to_string(CHAT_MAX_TITLE_LEN - 1) + " characters");
	}

	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Updating chat title");
	return ExecuteQuery(L"update c set c.title=N\'" + ReplaceSingleQuotes(new_title) + L"\' from Chat c"
					   " where c.chat_id=" + std::to_wstring(chat_id));
}

bool MSSQLDatabase::UpdateChatImageInDB(const std::string& user_access_token, const unsigned long& chat_id, const std::string& new_image_str)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Updating chat image");
	return ExecuteQuery("update c set c.image=0x" +
						 m_sha256_crypt.GetHexString((const unsigned char*) new_image_str.c_str(), new_image_str.length()) +
					   " from Chat c"
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

	const DatabaseConnectionSettings connection_settings = m_config_file.get_connection_settings();

	if (connection_settings == DatabaseConnectionSettings{})
	{
		LOG_FATAL("The contents of the configuration file are empty");
		throw std::runtime_error("The contents of the configuration file are empty");
	}

	const std::string connection_string = "DRIVER=" + connection_settings.driver + ";"
										  "SERVER=" + connection_settings.server + ";"
										  "DATABASE=" + connection_settings.database + ";"
										  "UID=" + connection_settings.uid + ";"
										  "PWD=" + connection_settings.pwd;

	SQLCHAR sql_connection_string[SQL_CONNECTION_STRING_LEN]{ 0 };
	strcpy_s((char*)sql_connection_string, SQL_CONNECTION_STRING_LEN, connection_string.c_str());

	LOG_DEBUG("Connecting to SQL Server");
	const SQLRETURN return_code = SQLDriverConnect(m_sql_connection_handle,
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

bool MSSQLDatabase::ExecuteQuery(const std::wstring& query)
{
	FreeStatementHandle();
	InitStatementHandle();
	return SQLExecDirectW(m_sql_statement_handle, (SQLWCHAR*) query.c_str(), SQL_NTS) == SQL_SUCCESS;
}

bool MSSQLDatabase::IsFieldNull(const SQLUSMALLINT field_number) const
{
	char check;
	return SQLGetData(m_sql_statement_handle, field_number, SQL_C_CHAR, &check, sizeof(char), nullptr) == SQL_NULL_DATA;
}

ISXModel::User MSSQLDatabase::GetUserFromDB() const
{
	unsigned long id = 0;
	char login[USER_MAX_LOGIN_LEN]{ 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, login, USER_MAX_LOGIN_LEN, nullptr);

	if (IsFieldNull(3))
	{
		return ISXModel::User(id, login);
	}
	else
	{
		std::unique_ptr<char[]> image{ std::make_unique<char[]>(USER_MAX_IMAGE_LEN) };

		SQLGetData(m_sql_statement_handle, 3, SQL_C_BINARY, image.get(), USER_MAX_IMAGE_LEN, nullptr);

		return ISXModel::User(id, login, image.get());
	}
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB() const
{
	unsigned long id = 0;
	wchar_t content[MESSAGE_MAX_CONTENT_LEN]{ 0 };
	char sender[USER_MAX_LOGIN_LEN]{ 0 };
	unsigned long chat_id = 0;
	char timestamp[MESSAGE_MAX_TIMESTAMP_LEN]{ 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_WCHAR, content, MESSAGE_MAX_CONTENT_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 3, SQL_C_CHAR, sender, USER_MAX_LOGIN_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 4, SQL_C_ULONG, &chat_id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 5, SQL_C_CHAR, timestamp, MESSAGE_MAX_TIMESTAMP_LEN, nullptr);

	return ISXModel::Message(id, content, sender, chat_id, timestamp);
}

ISXModel::Chat MSSQLDatabase::GetChatFromDB() const
{
	unsigned long id = 0;
	wchar_t title[CHAT_MAX_TITLE_LEN]{ 0 };

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_WCHAR, title, CHAT_MAX_TITLE_LEN, nullptr);

	if (IsFieldNull(3))
	{
		return ISXModel::Chat(id, title);
	}
	else
	{
		std::unique_ptr<char[]> image{ std::make_unique<char[]>(CHAT_MAX_IMAGE_LEN) };

		SQLGetData(m_sql_statement_handle, 3, SQL_C_BINARY, image.get(), CHAT_MAX_IMAGE_LEN, nullptr);

		return ISXModel::Chat(id, title, image.get());
	}
}

void MSSQLDatabase::CheckUserCredentialsInDB(const std::string& user_login, const std::string& user_password)
{
	LOG_DEBUG("Checking user credentials");
	ExecuteQuery("select u.user_id from [User] as u"
				" where u.login=\'" + user_login + "\'"
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
	ExecuteQuery("select u.user_id from [User] as u where u.login=\'" + user_login + "\'");

	if (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		LOG_ERROR("User login already exists");
		throw QueryException("Such user login already exists");
	}
}

void MSSQLDatabase::CheckIfUserAccessTokenValid(const std::string& user_access_token)
{
	LOG_DEBUG("Checking if user access token is valid");
	ExecuteQuery("select u.user_id, u.login, u.image from [User] as u"
				" inner join Token as t"
				" on t.user_id = u.user_id"
				" where t.access_token = \'" + user_access_token + "\'"
				" and t.last_used_date < t.expiration_date");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		RemoveUserAccessToken(user_access_token);

		LOG_ERROR("Invalid user access token");
		throw QueryException("Time session expired. Please login again.");
	}
}

ISXModel::User MSSQLDatabase::GetUserByAccessToken(const std::string& user_access_token)
{
	CheckIfUserAccessTokenValid(user_access_token);

	LOG_DEBUG("Receiving user by access token");
	return GetUserFromDB();
}

std::string MSSQLDatabase::GetUserAccessTokenFromDB()
{
	char user_access_token[USER_MAX_ACCESS_TOKEN_LEN]{ 0 };

	LOG_DEBUG("Receiving existing user access token");
	SQLGetData(m_sql_statement_handle, 1, SQL_C_CHAR, user_access_token, USER_MAX_ACCESS_TOKEN_LEN, nullptr);

	UpdateUserAccessTokenUsedDateInDB(user_access_token);
	return user_access_token;
}

bool MSSQLDatabase::SaveUserAccessTokenToDB(const std::string& user_access_token, const std::string& user_login)
{
	LOG_DEBUG("Saving user access token");
	return ExecuteQuery("insert into Token(access_token, user_id) values(\'" + user_access_token + "\',"
					   " (select u.user_id from [User] as u where u.login=\'" + user_login + "\'))");
}

bool MSSQLDatabase::IsUserParticipantOfChat(const std::string& user_login, const std::string& chat_id_str)
{
	LOG_DEBUG("Checking if user is participant of chat with id: " + chat_id_str);
	ExecuteQuery("select * from ChatParticipant as cp"
				" where cp.chat_id = " + chat_id_str +
				" AND cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + user_login + "\')");

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

bool MSSQLDatabase::UserHasToken(const std::string& user_login)
{
	LOG_DEBUG("Checking if user has token");
	ExecuteQuery("select t.access_token, count(*) from Token as t"
				" where t.user_id = (select u.user_id from [User] as u where u.login=\'" + user_login + "\')" +
				" group by t.access_token"
				" having count(*) = 1");

	return SQLFetch(m_sql_statement_handle) == SQL_SUCCESS;
}
