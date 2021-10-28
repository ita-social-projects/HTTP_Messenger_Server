#include "ConfigFile.h"

ConfigFile::ConfigFile(const std::string& filename)
{
	char* program_data_dir = nullptr;
	_dupenv_s(&program_data_dir, nullptr, PROGRAM_DATA_DIR);

	if (program_data_dir == nullptr)
	{
		LOG_FATAL("Environment variable \"" + std::string(PROGRAM_DATA_DIR) + "\" is not set");
		throw std::runtime_error("Environment variable \"" + std::string(PROGRAM_DATA_DIR) + "\" is not set");
	}

	const std::string http_messenger_server_dir = std::string(program_data_dir) + "\\" + HTTP_MESSENGER_SERVER_DIR;
	free(program_data_dir);

	if (!IsPathExists(http_messenger_server_dir))
	{
		LOG_DEBUG("Creating directory: \"" + http_messenger_server_dir + "\"");
		CreateDirectoryA(http_messenger_server_dir.c_str(), nullptr);
	}

	m_config_filename = http_messenger_server_dir + "\\" + filename;
}

void ConfigFile::CreateIfNotExists() const
{
	LOG_DEBUG("Checking if config file exists");
	if (IsPathExists(m_config_filename))
	{
		LOG_DEBUG("Config file exists");
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

inline bool ConfigFile::IsPathExists(const std::string& path) const
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}
