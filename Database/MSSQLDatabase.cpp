#include "MSSQLDatabase.h"

MSSQLDatabase::MSSQLDatabase()
		: m_sql_environment_handle(SQL_NULL_HENV)
		, m_sql_connection_handle(SQL_NULL_HDBC)
		, m_sql_statement_handle(SQL_NULL_HSTMT)
		, m_config_file("Database.conf")
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

ISXModel::User MSSQLDatabase::GetUserFromDB(const std::string& user_login)
{
	ExecuteQuery("select * from [User] as u where u.login=\'" + user_login + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("No such user login: \"" + user_login + "\"");
	}

	return GetUserFromDB();
}

std::vector<ISXModel::User> MSSQLDatabase::GetChatParticipantsFromDB(const std::string& chat_title)
{
	ExecuteQuery("select u.* from [User] as u"
				" inner join ChatParticipant as cp"
				" on cp.participant_id = u.user_id"
				" where cp.chat_id = (select c.chat_id from Chat as c where c.title=\'" + chat_title + "\')");

	std::vector<ISXModel::User> participants;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		participants.push_back(GetUserFromDB());
	}

	return participants;
}

bool MSSQLDatabase::SaveUserToDB(const ISXModel::User& user)
{
	std::string login = user.get_login();
	std::string password = user.get_password();

	ExecuteQuery("select * from [User] as u where u.login=\'" + login + "\'"); // check if user login exist

	if (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		throw QueryException("User login \"" + login + "\" already exist");
	}

	return ExecuteQuery("insert into [User](login, password) values(\'" + login + "\', \'" + password + "\')");
}

bool MSSQLDatabase::AddUserToChat(const std::string& user_login, const std::string& chat_title)
{
	// check if user is already participant of the chat
	ExecuteQuery("select * from ChatParticipant as cp"
				" where cp.chat_id = (select c.chat_id from Chat as c where c.title=\'" + chat_title + "\')"
				" AND cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + user_login + "\')");

	if (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		throw QueryException("User \"" + user_login + "\" is already participant of the chat \"" + chat_title + "\"");
	}

	return ExecuteQuery("insert into ChatParticipant values"
						" ((select c.chat_id from Chat as c where c.title=\'" + chat_title + "\'),"
						" (select u.user_id from [User] as u where u.login=\'" + user_login + "\'))");
}

bool MSSQLDatabase::RemoveUserFromChat(const std::string& user_login, const std::string& chat_title)
{
	return ExecuteQuery("delete cp from ChatParticipant as cp"
						" where cp.chat_id = (select c.chat_id from Chat as c where c.title=\'" + chat_title + "\')"
						" AND cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + user_login + "\')");
}

bool MSSQLDatabase::RemoveUserFromDB(const std::string& user_login)
{
	return ExecuteQuery("delete u from [User] as u where u.login=\'" + user_login + "\'");
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB(const unsigned long& message_id)
{
	ExecuteQuery("select * from Message as m where m.message_id=\'" + std::to_string(message_id) + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("No such message id: " + std::to_string(message_id));
	}

	return GetMessageFromDB();
}

std::vector<ISXModel::Message> MSSQLDatabase::GetChatMessagesFromDB(const std::string& chat_title)
{
	ExecuteQuery("select m.* from Message as m"
				" inner join Chat as c"
				" on c.chat_id = m.chat_id"
				" where m.chat_id = (select c.chat_id from Chat as c where c.title=\'" + chat_title + "\')");

	std::vector<ISXModel::Message> messages;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		messages.push_back(GetMessageFromDB());
	}

	return messages;
}

bool MSSQLDatabase::SaveMessageToDB(const ISXModel::Message& message)
{
	std::string content = message.get_content();
	std::string sender_id = std::to_string(message.get_sender_id());
	std::string chat_id = std::to_string(message.get_chat_id());

	// check if user is participant of the chat
	ExecuteQuery("select * from ChatParticipant as cp where cp.chat_id=\'" + chat_id + "\' AND cp.participant_id=\'" + sender_id + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("User with id: " + sender_id + " is not participant of the chat with id: " + chat_id);
	}

	return ExecuteQuery("insert into Message([content], sender_id, chat_id) values(\'" + content + "\', \'" + sender_id + "\', \'" + chat_id + "\')");
}

bool MSSQLDatabase::RemoveMessageFromDB(const unsigned long& message_id)
{
	return ExecuteQuery("delete m from Message as m where m.message_id=\'" + std::to_string(message_id) + "\'");
}

ISXModel::Chat MSSQLDatabase::GetChatFromDB(const std::string& chat_title)
{
	ExecuteQuery("select * from Chat as c where c.title=\'" + chat_title + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw QueryException("No such chat title: \"" + chat_title + "\"");
	}

	return GetChatFromDB();
}

std::vector<ISXModel::Chat> MSSQLDatabase::GetUserChatsFromDB(const std::string& user_login)
{
	ExecuteQuery("select c.* from Chat as c"
				" inner join ChatParticipant as cp"
				" on cp.chat_id = c.chat_id"
				" where cp.participant_id = (select u.user_id from [User] as u where u.login=\'" + user_login + "\')");

	std::vector<ISXModel::Chat> chats;

	while (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		chats.push_back(GetChatFromDB());
	}

	return chats;
}

bool MSSQLDatabase::SaveChatToDB(const ISXModel::Chat& chat)
{
	std::string title = chat.get_title();

	ExecuteQuery("select * from Chat as c where c.title=\'" + title + "\'");

	if (SQLFetch(m_sql_statement_handle) == SQL_SUCCESS)
	{
		throw QueryException("Chat title \"" + title + "\" already exist");
	}

	return ExecuteQuery("insert into Chat(title) values(\'" + title + "\')");
}

bool MSSQLDatabase::RemoveChatFromDB(const std::string& chat_title)
{
	return ExecuteQuery("delete c from Chat as c where c.title=\'" + chat_title + "\'");
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
	char login[USER_LOGIN_LEN];
	char password[USER_PASSWORD_LEN];

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, login, USER_LOGIN_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 3, SQL_C_CHAR, password, USER_PASSWORD_LEN, nullptr);

	return ISXModel::User(id, login, password);
}

ISXModel::Message MSSQLDatabase::GetMessageFromDB() const
{
	unsigned long id = 0;
	char content[MESSAGE_CONTENT_LEN];
	unsigned long sender_id = 0;
	unsigned long chat_id = 0;
	char timestamp[MESSAGE_TIMESTAMP_LEN];

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
	char title[CHAT_TITLE_LEN];

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, title, CHAT_TITLE_LEN, nullptr);

	return ISXModel::Chat(id, title);
}
