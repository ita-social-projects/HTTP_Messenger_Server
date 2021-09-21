#pragma once

#include <string>

class QueryException final : public std::exception
{
public:
	QueryException(const std::string& message);
	const char* what() const noexcept override;

private:
	std::string m_message;
};
