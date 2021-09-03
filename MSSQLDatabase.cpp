#include "MSSQLDatabase.h"

MSSQLDatabase::MSSQLDatabase()
		: m_sql_environment_handle(SQL_NULL_HENV)
		, m_sql_connection_handle(SQL_NULL_HDBC)
		, m_sql_statement_handle(SQL_NULL_HSTMT)
{
	InitEnvironmentHandle();
	InitConnectionHandle();
}

MSSQLDatabase::~MSSQLDatabase()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_sql_statement_handle);
	SQLDisconnect(m_sql_connection_handle);
	SQLFreeHandle(SQL_HANDLE_DBC, m_sql_connection_handle);
	SQLFreeHandle(SQL_HANDLE_ENV, m_sql_environment_handle);
}

User MSSQLDatabase::GetUserFromDB(const std::string& user_login)
{
	ExecuteQuery("select * from [User] as u where u.login=\'" + user_login + "\'");

	if (SQLFetch(m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw std::runtime_error("No such user login: \"" + user_login + "\"");
	}

	return GetUserFromDB();
}

bool MSSQLDatabase::SaveUserToDB(const User& user)
{
	std::string login = user.get_login();
	std::string password = user.get_password();

	try
	{
		GetUserFromDB(login); // check if user login exist
		return false;
	}
	catch (const std::exception&)
	{
		return ExecuteQuery("insert into [User](login, password) values(\'" + login + "\', \'" + password + "\')");
	}
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

	SQLCHAR connection_string[SQL_CONNECTION_STRING_LEN] = { 0 };
	SQLCHAR *connection_string_ptr = connection_string;
	GetConnectionStringFromFile("Database.conf", &connection_string_ptr);

	if (strcmp((char*) connection_string, "") == 0)
	{
		throw std::runtime_error("The contents of the configuration file are empty");
	}

	SQLRETURN return_code = SQLDriverConnect(m_sql_connection_handle,
							 nullptr,
							 connection_string,
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
	SQLFreeHandle(SQL_HANDLE_STMT, m_sql_statement_handle);
	m_sql_statement_handle = SQL_NULL_HSTMT;

	if (SQLAllocHandle(SQL_HANDLE_STMT, m_sql_connection_handle, &m_sql_statement_handle) != SQL_SUCCESS)
	{
		throw std::runtime_error("Error allocating statement handle");
	}
}

void MSSQLDatabase::GetConnectionStringFromFile(const std::string& filename, SQLCHAR** output_ptr) const
{
	std::string connection_string;
	std::string line;
	std::ifstream config_file(filename, std::ios::in);

	if (!config_file.is_open())
	{
		throw std::runtime_error("Cannot open file: \"" + filename + "\"");
	}

	while (std::getline(config_file, line))
	{
		connection_string += line + ';';
	}

	if (output_ptr != nullptr)
	{
		strcpy_s((char*) *output_ptr, SQL_CONNECTION_STRING_LEN, connection_string.c_str());
	}
}

bool MSSQLDatabase::ExecuteQuery(const std::string& query)
{
	InitStatementHandle(); // needed to do always before executing each query

	if (SQLExecDirect(m_sql_statement_handle, (SQLCHAR*) query.c_str(), SQL_NTS) != SQL_SUCCESS)
	{
		return false;
	}

	return true;
}

User MSSQLDatabase::GetUserFromDB() const
{
	unsigned long id = 0;
	char login[USER_LOGIN_LEN];
	char password[USER_PASSWORD_LEN];

	SQLGetData(m_sql_statement_handle, 1, SQL_C_ULONG, &id, sizeof(unsigned long), nullptr);
	SQLGetData(m_sql_statement_handle, 2, SQL_C_CHAR, login, USER_LOGIN_LEN, nullptr);
	SQLGetData(m_sql_statement_handle, 3, SQL_C_CHAR, password, USER_PASSWORD_LEN, nullptr);

	return User(id, login, password);
}
