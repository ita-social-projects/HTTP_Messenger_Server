#include "ConfigFile.h"

ConfigFile::ConfigFile(const std::string& filename)
{
	const char* const home_dir = std::getenv(USER_HOME_DIR);

	if (home_dir == nullptr)
	{
		LOG_FATAL("Environment variable \"" + std::string(USER_HOME_DIR) + "\" is not set");
		throw std::runtime_error("Environment variable \"" + std::string(USER_HOME_DIR) + "\" is not set");
	}

	m_config_filename = std::string(home_dir) + "/" + filename;
}

void ConfigFile::CreateIfNotExists() const
{
	LOG_DEBUG("Checking if config file exists");
	if (FileExists(m_config_filename))
	{
		return;
	}

	std::ofstream config_file(m_config_filename, std::ios::out | std::ios::trunc);

	if (config_file.is_open())
	{
		SQLInfo default_sql_info;

		LOG_DEBUG("Saving default configuration settings to file");
		config_file << "DRIVER=" << default_sql_info.driver << "\n";
		config_file << "SERVER=" << default_sql_info.server << "\n";
		config_file << "DATABASE=" << default_sql_info.database << "\n";
		config_file << "UID=" << default_sql_info.uid << "\n";
		config_file << "PWD=" << default_sql_info.pwd;

		config_file.close();
	}
	else
	{
		LOG_FATAL("Cannot open file \"" + m_config_filename + "\" for writing");
		throw std::runtime_error("Cannot open file \"" + m_config_filename + "\" for writing");
	}
}

std::string ConfigFile::GetStringWithDelimeter(const char delimeter) const
{
	std::string string;
	std::string line;
	std::ifstream config_file(m_config_filename, std::ios::in);

	if (!config_file.is_open())
	{
		LOG_FATAL("Cannot open file: \"" + m_config_filename + "\"");
		throw std::runtime_error("Cannot open file: \"" + m_config_filename + "\"");
	}

	LOG_DEBUG("Receiving configuration settings from file");
	while (std::getline(config_file, line))
	{
		string += line + delimeter;
	}

	config_file.close();
	return string;
}

inline bool ConfigFile::FileExists(const std::string& filename) const
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}
