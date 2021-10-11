#pragma once

#include <iomanip>
#include <sstream>

#include <openssl/evp.h>

#include "IHash.h"

#define HEX_WIDTH 2

class SHA256 final : public IHash
{
public:
	SHA256();
	~SHA256();
	std::string GenerateHash(const std::string& string) override;

private:
	void Init();
	void Update(const std::string& string);
	void Final();
	std::string GetHashString() const;

	EVP_MD_CTX* m_context;
	std::unique_ptr<unsigned char[]> m_hash;
	unsigned int m_hash_length;
};
