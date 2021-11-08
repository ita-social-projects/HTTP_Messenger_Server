#include "ConfigFile.h"

ConfigFile::ConfigFile(const std::string& filename)
		: m_config_filename(filename)
		, m_connection_settings(nullptr)
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
		DatabaseConnectionSettings default_settings{ "{SQL Server}", "tcp:localhost,1433", "HTTP_Messenger", "sa", "" };

		LOG_DEBUG("Saving default configuration settings to file");
		config_file << "DRIVER" << SETTING_VALUE_DELIMITER << default_settings.driver << "\n";
		config_file << "SERVER" << SETTING_VALUE_DELIMITER << default_settings.server << "\n";
		config_file << "DATABASE" << SETTING_VALUE_DELIMITER << default_settings.database << "\n";
		config_file << "UID" << SETTING_VALUE_DELIMITER << default_settings.uid << "\n";
		config_file << "PWD" << SETTING_VALUE_DELIMITER << default_settings.pwd;

		config_file.close();
	}
	else
	{
		LOG_FATAL("Cannot open file \"" + m_config_filename + "\" for writing");
		throw std::runtime_error("Cannot open file \"" + m_config_filename + "\" for writing");
	}
}

DatabaseConnectionSettings ConfigFile::get_connection_settings() const
{
	if (m_connection_settings == nullptr)
	{
		const_cast<ConfigFile*>(this)->ReadSettingsFromFile();
	}

	LOG_DEBUG("Receiving saved configuration settings");
	return *m_connection_settings;
}

void ConfigFile::ReadSettingsFromFile()
{
	std::string line;
	std::ifstream config_file(m_config_filename, std::ios::in);
	m_connection_settings = std::make_unique<DatabaseConnectionSettings>();

	if (!config_file.is_open())
	{
		LOG_FATAL("Cannot open file: \"" + m_config_filename + "\"");
		throw std::runtime_error("Cannot open file: \"" + m_config_filename + "\"");
	}

	LOG_DEBUG("Receiving configuration settings from file");
	while (std::getline(config_file, line))
	{
		GetSettingFromString(line);
	}

	config_file.close();
}

void ConfigFile::GetSettingFromString(const std::string& setting)
{
	const std::size_t delimiter_pos = setting.find(SETTING_VALUE_DELIMITER);

	if (delimiter_pos == std::string::npos)
	{
		LOG_FATAL("Incorrect format of configuration settings in file");
		throw std::runtime_error("Incorrect format of configuration settings in file");
	}

	const std::string setting_name = setting.substr(0, delimiter_pos);
	const std::string setting_value = setting.substr(delimiter_pos+1);

	if (setting_name == "DRIVER")
	{
		m_connection_settings->driver = setting_value;
	}
	else if (setting_name == "SERVER")
	{
		m_connection_settings->server = setting_value;
	}
	else if (setting_name == "DATABASE")
	{
		m_connection_settings->database = setting_value;
	}
	else if (setting_name == "UID")
	{
		m_connection_settings->uid = setting_value;
	}
	else if (setting_name == "PWD")
	{
		m_connection_settings->pwd = setting_value;
	}
}

inline bool ConfigFile::IsFileExist(const std::string& filename) const
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}
