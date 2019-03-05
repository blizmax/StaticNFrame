// -------------------------------------------------------------------------
//    @FileName         :    NFMD5.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPHPMD5Impl.h"

#include <fstream>
#include <iostream>

// A wrapper of PHP md5 implementation
class _NFExport NFPHPMD5
{
public:
	enum
	{
		kBinDigestLength = 16, // the length of digest as a 16-byte binary array
		kHexDigestLength = 32, // the length of digest as a 32-byte ASCII-hex string, not include the terminating null-character
	};

public:
	NFPHPMD5()
	{
		PHP_MD5Init(&md5_ctx);
	}

	void Update(const void* data, size_t data_len)
	{
		PHP_MD5Update(&md5_ctx, static_cast<const unsigned char*>(data), data_len);
	}

	template <typename string_t>
	void Update(const string_t& data)
	{
		Update(data.data(), data.size());
	}

	void Finalize(unsigned char binary16_digest[/*16*/])
	{
		PHP_MD5Final(binary16_digest, &md5_ctx);
	}

	void Finalizeh(char hex33[33]);

	string Finalize()
	{
		string m(kBinDigestLength, '\0');
		Finalize(reinterpret_cast<unsigned char*>(&m[0]));
		return m;
	}

	string Finalizeh()
	{
		string m(kHexDigestLength, '\0');
		Finalize(reinterpret_cast<unsigned char*>(&m[0]));
		//m.resize(kHexDigestLength);
		return m;
	}

	// static helpful utility methods
public:
	// Calculate the MD5 checksum of the data.
	static void Sum(const void* data, size_t data_len, unsigned char binary16_digest[16]);
	static string Sum(const void* data, size_t data_len);
	static string Sum(const string& d);

	// Calculate the MD5 checksum of the data. The checksum is human readable with 32 bytes hex code.
	static void Sumh(const void* data, size_t data_len, char hex33[/*33*/]);
	static string Sumh(const void* data, size_t data_len);
	static string Sumh(const string& d);

	// Generate a random md5
	// @return a random hex md5, 32 bytes
	static string GenHexMD5(const size_t random_data_len = 8);

	// @return a random binary md5, 16 bytes
	static string GenBinMD5(const size_t random_data_len = 8);

	// @brief Convert a binary buffer to a hex format
	// @param[in] binary -
	// @param[in] binary_len -
	// @param[in] hex - At least with length of binary_len*2
	// @return void -
	static void Bin2Hex(const void* binary, size_t binary_len, char* hex);
	static string Bin2Hex(const void* binary, size_t binary_len);

	// @brief Convert a hex format buffer to binary
	// @param[in] hex -
	// @param[in] hex_len -
	// @param[in] binary - At least with length of hex_len/2
	// @return bool -
	static bool Hex2Bin(const char* hex, const size_t hex_len, void* binary);
	static string Hex2Bin(const char* hex, const size_t hex_len);

	static int DehexChar(char c);
	static bool IsValid(const char* hexmd5, size_t hexmd5_length);
	static bool IsValid(const string& hexmd5);
private:
	PHP_MD5_CTX md5_ctx;
};

inline void NFPHPMD5::Finalizeh(char hex33[33])
{
	unsigned char binary16_digest[kBinDigestLength];
	PHP_MD5Final(binary16_digest, &md5_ctx);
	Bin2Hex(binary16_digest, kBinDigestLength, hex33);
}

inline void NFPHPMD5::Sum(const void* data, size_t data_len, unsigned char binary16_digest[16])
{
	PHP_MD5_CTX ctx;
	PHP_MD5Init(&ctx);
	PHP_MD5Update(&ctx, static_cast<const unsigned char*>(data), data_len);
	PHP_MD5Final(binary16_digest, &ctx);
}

inline string NFPHPMD5::Sum(const string& d)
{
	return Sum(d.data(), d.size());
}

inline void NFPHPMD5::Sumh(const void* data, size_t data_len, char hex33[/*33*/])
{
	PHP_MD5_CTX ctx;
	PHP_MD5Init(&ctx);
	PHP_MD5Update(&ctx, static_cast<const unsigned char*>(data), data_len);
	unsigned char* binary = reinterpret_cast<unsigned char*>(hex33 + kBinDigestLength);
	PHP_MD5Final(binary, &ctx);
	Bin2Hex(binary, kBinDigestLength, hex33);
}

inline string NFPHPMD5::Sumh(const string& d)
{
	return Sumh(d.data(), d.size());
}

inline string NFPHPMD5::Sum(const void* data, size_t data_len)
{
	string binary16_digest;
	binary16_digest.resize(16);
	Sum(data, data_len, reinterpret_cast<unsigned char*>(&binary16_digest[0]));
	return binary16_digest;
}

inline string NFPHPMD5::Sumh(const void* data, size_t data_len)
{
	string hex33;
	hex33.resize(kHexDigestLength + 1);
	Sumh(data, data_len, &hex33[0]);
	hex33.resize(kHexDigestLength);
	return hex33;
}

inline void NFPHPMD5::Bin2Hex(const void* binary, size_t binary_len, char* hex)
{
	int m = 0;
	const uint8_t* ubinary = reinterpret_cast<const uint8_t*>(binary);

	for (size_t i = 0; i < binary_len; ++i)
	{
		m = (ubinary[i] & 0xF0) >> 4;

		if (m <= 9)
		{
			hex[i << 1] = m + '0';
		}
		else
		{
			hex[i << 1] = m + 'a' - 10;
		}

		m = (ubinary[i]) & 0x0F;

		if (m <= 9)
		{
			hex[(i << 1) + 1] = m + '0';
		}
		else
		{
			hex[(i << 1) + 1] = m + 'a' - 10;
		}
	}

	hex[binary_len << 1] = '\0';
}

inline string NFPHPMD5::Bin2Hex(const void* binary, size_t binary_len)
{
	string r;
	r.resize(binary_len << 1);
	Bin2Hex(binary, binary_len, &r[0]);
	return r;
}

inline bool NFPHPMD5::Hex2Bin(const char* hex, const size_t hex_len, void* binary)
{
	int m = 0;
	uint8_t* ubinary = reinterpret_cast<uint8_t*>(binary);
	size_t i = 0;
	size_t hex_len_1 = hex_len - 1;

	while (i < hex_len_1)
	{
		m = DehexChar(hex[i]);

		if (m < 0)
		{
			return false;
		}

		ubinary[i >> 1] = m << 4;

		m = DehexChar(hex[i + 1]);

		if (m < 0)
		{
			return false;
		}

		ubinary[i >> 1] |= m;

		i += 2;
	}

	return true;
}

inline string NFPHPMD5::Hex2Bin(const char* hex, const size_t hex_len)
{
	string r;
	r.resize(hex_len >> 1);
	Hex2Bin(hex, hex_len, &r[0]);
	return r;
}

inline int NFPHPMD5::DehexChar(char c)
{
	static int hex2dec_table[256] = {
		/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
		/* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 3 */ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1, -1,-1,-1,-1,

		/* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

		/* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

		/* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		/* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
	};

	return hex2dec_table[static_cast<int>(c)];
}

inline bool NFPHPMD5::IsValid(const char* hexmd5, size_t hexmd5_length)
{
	if (hexmd5_length > 32 || hexmd5_length == 0)
	{
		return false;
	}

	static const char valid_chars[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (size_t i = 0; i < hexmd5_length; ++i)
	{
		if (valid_chars[static_cast<int>(hexmd5[i])] == 0)
		{
			return false;
		}
	}

	return true;
}

inline bool NFPHPMD5::IsValid(const string& hexmd5)
{
	return IsValid(hexmd5.c_str(), hexmd5.length());
}

