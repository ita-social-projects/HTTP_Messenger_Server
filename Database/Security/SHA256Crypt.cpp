#include "SHA256Crypt.h"

SHA256Crypt::SHA256Crypt()
		: m_context(EVP_MD_CTX_new())
		, m_hash(std::make_unique<unsigned char[]>(EVP_MAX_MD_SIZE))
		, m_hash_length(0)
{
	if (m_context == nullptr)
	{
		LOG_FATAL("Error allocating digest context");
		throw std::runtime_error("Error allocating digest context");
	}
}

SHA256Crypt::~SHA256Crypt()
{
	m_hash.reset();
	EVP_MD_CTX_free(m_context);
}

const std::string SHA256Crypt::GenerateHash(const std::string& string)
{
	Init();
	Update(string);
	Final();

	LOG_DEBUG("Receiving hash string");
	return GetHexString(m_hash.get(), m_hash_length);
}

const std::string SHA256Crypt::GenerateSaltedHash(const std::string& string, const std::string& salt)
{
	Init();
	Update(salt);
	Update(string);
	Final();

	LOG_DEBUG("Receiving hash string");
	return GetHexString(m_hash.get(), m_hash_length);
}

void SHA256Crypt::Init()
{
	if (!EVP_DigestInit_ex(m_context, EVP_sha256(), nullptr))
	{
		LOG_FATAL("SHA256 init failed");
		throw std::runtime_error("SHA256 init failed");
	}
}

void SHA256Crypt::Update(const std::string& string)
{
	if (!EVP_DigestUpdate(m_context, string.c_str(), string.length()))
	{
		LOG_FATAL("SHA256 update failed");
		throw std::runtime_error("SHA256 update failed");
	}
}

void SHA256Crypt::Final()
{
	if (!EVP_DigestFinal_ex(m_context, m_hash.get(), &m_hash_length))
	{
		LOG_FATAL("SHA256 final failed");
		throw std::runtime_error("SHA256 final failed");
	}
}

const std::string SHA256Crypt::GetHexString(const unsigned char* char_array_ptr, const std::size_t char_array_size)
{
	if (char_array_ptr == nullptr)
	{
		return "";
	}

	std::ostringstream oss;

	oss << std::setfill('0') << std::hex;
	for (std::size_t i = 0; i < char_array_size; i++)
	{
		oss << std::setw(HEX_WIDTH) << (unsigned int) char_array_ptr[i];
	}

	return oss.str();
}
