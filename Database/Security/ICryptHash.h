#pragma once

#include <string>

class ICryptHash
{
public:
	virtual const std::string GenerateHash(const std::string& string) = 0;
	virtual const std::string GenerateSaltedHash(const std::string& string, const std::string& salt) = 0;
};
