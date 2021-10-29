#pragma once
#include <string>

std::wstring to_wstring(std::string value);
std::string to_string(std::wstring value);

// replaces apostrophes (') to '' required for SQL query
std::wstring ReplaceSingleQuotes(const std::wstring& value);
