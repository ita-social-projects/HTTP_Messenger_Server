#pragma once

#include <iomanip>
#include <sstream>

#include <openssl/evp.h>

#include "ICryptHash.h"
#include "../../Logger/Logger.h"

constexpr auto HEX_WIDTH = 2;

class SHA256Crypt final : public ICryptHash
{
public:
	SHA256Crypt();
	~SHA256Crypt();
	const std::string GenerateHash(const std::string& string) override;
	const std::string GenerateSaltedHash(const std::string& string, const std::string& salt) override;
	static const std::string GetHexString(const unsigned char* char_array_ptr, const std::size_t char_array_size);

private:
	void Init();
	void Update(const std::string& string);
	void Final();

	EVP_MD_CTX* m_context;
	std::unique_ptr<unsigned char[]> m_hash;
	unsigned int m_hash_length;
};
