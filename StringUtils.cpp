#include "StringUtils.h"
#include <string>

std::string to_string(std::wstring value) {
	std::string val = std::string(value.begin(), value.end());
	return val;
}

std::wstring to_wstring(std::string value) {
	std::wstring val = std::wstring(value.begin(), value.end());
	return val;
}

std::wstring ReplaceSingleQuotes(const std::wstring& value)
{
	std::size_t pos = 0;
	std::wstring val = value;

	while ((pos = val.find(L"\'", pos)) != std::wstring::npos)
	{
		val.replace(pos, 1, L"\'\'", 2);
		pos += 2;
	}

	return val;
}
