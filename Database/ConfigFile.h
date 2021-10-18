#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "../Logger/Logger.h"

#ifdef _WIN32
#define USER_HOME_DIR "USERPROFILE"
#else
#define USER_HOME_DIR "HOME"
#endif

class ConfigFile
{
public:
	ConfigFile(const std::string& filename);
	void CreateIfNotExists() const;
	std::string GetStringWithDelimeter(const char delimeter) const;

private:
	bool FileExists(const std::string& filename) const;

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

