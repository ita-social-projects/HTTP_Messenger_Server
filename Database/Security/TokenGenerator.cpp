#include "TokenGenerator.h"

TokenGenerator::TokenGenerator(const std::uint32_t token_length)
		: m_alphabet(TOKEN_CHARS_ALLOWED)
		, m_alphabet_length(strlen(TOKEN_CHARS_ALLOWED))
		, m_increment(TOKEN_INCREMENT)
{
	if (token_length == 0)
	{
		LOG_FATAL("The length of the token cannot be zero");
		throw std::runtime_error("The length of the token cannot be zero");
	}

	m_token_length = token_length;
	m_modulus = BinaryPow(m_alphabet_length, m_token_length) - 1;

	ReseedGenerator();
}

void TokenGenerator::GetNextToken(char*& output_token_ptr)
{
	std::uint64_t number;

	LOG_DEBUG("Generating token");
	for (std::uint32_t i = GENERATED_TOKEN_LEN; i <= m_token_length; i += GENERATED_TOKEN_LEN)
	{
		number = GenerateNextNumber();
		GetTokenFromNumber(number, output_token_ptr, GENERATED_TOKEN_LEN);
		output_token_ptr += GENERATED_TOKEN_LEN;
	}

	const std::uint32_t remainder = m_token_length % GENERATED_TOKEN_LEN;

	if (remainder != 0)
	{
		number = GenerateNextNumber();
		GetTokenFromNumber(number, output_token_ptr, remainder);
	}
}

void TokenGenerator::ReseedGenerator()
{
	LOG_DEBUG("Setting new seed to generator");
	std::uint64_t seed = GenerateSeed();
	m_multiplier = (m_modulus / seed) + 1;
	m_init_generated_number = m_last_generated_number = seed % m_modulus;
}

void TokenGenerator::GetTokenFromNumber(const std::uint64_t token_number, char*& output_token_ptr, const std::uint32_t output_token_length) const
{
	if (output_token_ptr == nullptr)
	{
		LOG_FATAL("Output token is not initialized");
		throw std::runtime_error("Output token is not initialized");
	}

	if (token_number >= m_modulus)
	{
		LOG_FATAL("Token number exceeds the number of permutations of possible tokens");
		throw std::runtime_error("Token number exceeds the number of permutations of possible tokens");
	}

	for (std::uint32_t i = 0; i < output_token_length; ++i)
	{
		output_token_ptr[i] = m_alphabet[(std::uint64_t)(token_number / BinaryPow(m_alphabet_length, i)) % m_alphabet_length];
	}
}

std::uint64_t TokenGenerator::GenerateNextNumber()
{
	std::uint64_t number = (m_multiplier * m_last_generated_number + m_increment) % m_modulus;

	if (number != m_init_generated_number)
	{
		m_last_generated_number = number;
		return number;
	}

	ReseedGenerator();
	return GenerateNextNumber();
}

std::uint64_t TokenGenerator::BinaryPow(std::uint64_t base, std::uint64_t exp) const
{
	std::uint64_t result = 1;

	while (exp > 0)
	{
		if (exp & 1)
		{
			result *= base;
		}

		base *= base;
		exp >>= 1;
	}

	return result;
}

std::uint64_t TokenGenerator::GenerateSeed() const
{
	srand(time(NULL));
	return rand();
}
