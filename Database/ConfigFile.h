#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "../Logger/Logger.h"

constexpr auto PROGRAM_DATA_DIR = "ProgramData";
constexpr auto HTTP_MESSENGER_SERVER_DIR = "HTTP_Messenger_Server";

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
	std::string GetStringWithDelimeter(const char delimeter) const;

private:
	bool IsPathExists(const std::string& path) const;

	std::string m_config_filename;
};
