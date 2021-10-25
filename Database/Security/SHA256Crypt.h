#pragma once

#include <iomanip>
#include <sstream>

#include <openssl/evp.h>

#include "ICryptHash.h"
#include "../../Logger/Logger.h"

#define HEX_WIDTH 2

class SHA256Crypt final : public ICryptHash
{
public:
	SHA256Crypt();
	~SHA256Crypt();
	const std::string GenerateHash(const std::string& string) override;
	const std::string GenerateSaltedHash(const std::string& string, const std::string& salt) override;

private:
	void Init();
	void Update(const std::string& string);
	void Final();
	std::string GetHashString() const;

	EVP_MD_CTX* m_context;
	std::unique_ptr<unsigned char[]> m_hash;
	unsigned int m_hash_length;
};
