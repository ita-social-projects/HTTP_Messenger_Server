#include "stringtowstring.h"
#include <string>

std::string to_string(std::wstring value) {
	std::string val = std::string(value.begin(), value.end());
	return val;
}

std::wstring to_wstring(std::string value) {
	std::wstring val = std::wstring(value.begin(), value.end());
	return val;
}