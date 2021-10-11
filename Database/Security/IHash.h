#pragma once

#include <string>

class IHash
{
public:
	virtual std::string GenerateHash(const std::string& string) = 0;
};
