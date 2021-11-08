#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "IDatabase.h"
#include "../Logger/Logger.h"

constexpr auto SETTING_VALUE_DELIMITER = "=";

class ConfigFile
{
public:
	ConfigFile(const std::string& filename);
	ConfigFile(const ConfigFile& config_file) = delete;
	ConfigFile& operator=(const ConfigFile& config_file) = delete;

	void CreateIfNotExists() const;

	DatabaseConnectionSettings get_connection_settings() const;

private:
	void ReadSettingsFromFile(std::ifstream& config_file);
	void GetSettingFromString(const std::string& setting);
	void WriteSettingsToFile(std::ofstream& config_file) const;
	bool IsFileExist(const std::string& filename) const;

	std::string m_config_filename;
	std::unique_ptr<DatabaseConnectionSettings> m_connection_settings;
	static std::mutex s_mutex;
};
