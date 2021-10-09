#include "QueryException.h"

QueryException::QueryException(const std::string& message)
		: m_message(message)
{}

const char* QueryException::what() const noexcept
{
	return m_message.c_str();
}
