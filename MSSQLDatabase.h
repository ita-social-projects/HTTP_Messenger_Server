#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

#define SQL_BUFFER_LEN 240
#define SQL_CONNECTION_STRING_LEN 1024

class MSSQLDatabase
{
public:
	MSSQLDatabase();
	~MSSQLDatabase();

	void ExecuteQuery(const std::string& query);

private:
	void InitEnvironmentHandle();
	void InitConnectionHandle();
	void InitStatementHandle();
	void GetConnectionStringFromFile(const std::string& filename, SQLCHAR** output_ptr) const;

	SQLHANDLE m_sql_environment_handle;
	SQLHANDLE m_sql_connection_handle;
	SQLHANDLE m_sql_statement_handle;
};

