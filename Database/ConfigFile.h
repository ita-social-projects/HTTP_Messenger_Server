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
	void CreateIfNotExists() const;

	DatabaseConnectionSettings get_connection_settings() const;

private:
	void GetSettingsFromFile();
	void GetSettingFromString(const std::string& setting);
	bool IsFileExist(const std::string& filename) const;

	std::string m_config_filename;
	std::unique_ptr<DatabaseConnectionSettings> m_connection_settings;
};
