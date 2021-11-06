#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "../Logger/Logger.h"

class ConfigFile
{
private:
	struct SQLSettings
	{
		std::string driver;
		std::string server;
		std::string database;
		std::string uid;
		std::string pwd;
	};

public:
	ConfigFile(const std::string& filename);
	void CreateIfNotExists() const;
	std::string GetStringWithDelimeter(const char delimeter = '\n') const;

private:
	bool IsFileExist(const std::string& filename) const;

	std::string m_config_filename;
};
