#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "../Logger/Logger.h"

#define PROGRAM_DATA_DIR "ProgramData"
#define HTTP_MESSENGER_SERVER_DIR "HTTP_Messenger_Server"

class ConfigFile
{
public:
	ConfigFile(const std::string& filename);
	void CreateIfNotExists() const;
	std::string GetStringWithDelimeter(const char delimeter) const;

private:
	bool IsPathExists(const std::string& path) const;

	std::string m_config_filename;

	struct SQLInfo
	{
		std::string driver = "{SQL Server}";
		std::string server = "tcp:localhost,1433";
		std::string database = "HTTP_Messenger";
		std::string uid = "sa";
		std::string pwd = "";
	};
};

