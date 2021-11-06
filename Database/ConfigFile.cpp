#include "ConfigFile.h"

ConfigFile::ConfigFile(const std::string& filename)
		: m_config_filename(filename)
{}

void ConfigFile::CreateIfNotExists() const
{
	LOG_DEBUG("Checking if config file exists");
	if (IsFileExist(m_config_filename))
	{
		LOG_DEBUG("Config file exists");
		return;
	}

	std::ofstream config_file(m_config_filename, std::ios::out | std::ios::trunc);

	if (config_file.is_open())
	{
		SQLSettings default_sql_settings{ "{SQL Server}", "tcp:localhost,1433", "HTTP_Messenger", "sa", "" };

		LOG_DEBUG("Saving default configuration settings to file");
		config_file << "DRIVER=" << default_sql_settings.driver << "\n";
		config_file << "SERVER=" << default_sql_settings.server << "\n";
		config_file << "DATABASE=" << default_sql_settings.database << "\n";
		config_file << "UID=" << default_sql_settings.uid << "\n";
		config_file << "PWD=" << default_sql_settings.pwd;

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

inline bool ConfigFile::IsFileExist(const std::string& filename) const
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}
