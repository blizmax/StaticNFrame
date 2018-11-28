// -------------------------------------------------------------------------
//    @FileName         :    NFMD5.cpp
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFPHPMD5.h"
#include "NFRandom.hpp"

string NFPHPMD5::GenBinMD5(const size_t random_data_len)
{
	thread_local static NFRandom rand(uint32_t(NFGetTime() / 1000));
	string data;
	data.resize(random_data_len);

	for (size_t i = 0; i < random_data_len; ++i)
	{
		data[i] = rand.Next() % 255;
	}

	return NFPHPMD5::Sum(reinterpret_cast<const void*>(data.data()), data.length());
}

string NFPHPMD5::GenHexMD5(const size_t random_data_len)
{
	string bin = GenBinMD5(random_data_len);
	return Bin2Hex(bin.data(), bin.size());
}

