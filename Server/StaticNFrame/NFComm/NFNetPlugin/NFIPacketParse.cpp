// -------------------------------------------------------------------------
//    @FileName         :    NFIPacketParse.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFIPacketParse.h"
#include "ArpgNetPacketParse.h"

#include "NFComm/NFPluginModule/NFINetModule.h"

NFIPacketParse* NFIPacketParse::m_pPacketParse = CreatePacketParse(0);

void NFIPacketParse::DeletePacketParse(NFIPacketParse* pPacketParse)
{
	delete pPacketParse;
}

NFIPacketParse* NFIPacketParse::CreatePacketParse(int parseType)
{
	return NF_NEW ArpgNetPacketParse();
}

int NFIPacketParse::DeCode(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t& nMsgId, uint64_t& nValue)
{
	return m_pPacketParse->DeCodeImpl(strData, unLen, outData, outLen, allLen, nMsgId, nValue);
}

int NFIPacketParse::EnCode(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer)
{
	return m_pPacketParse->EnCodeImpl(unMsgID, nValue, strData, unDataLen, buffer);
}

bool NFIPacketParse::DeCodeWeb(const char* strData, const uint32_t unLen, std::string& payload, uint32_t& outopcode, uint32_t& frameSize, bool& outfin)
{
	const auto buffer = (const unsigned char*)strData;

	if (unLen < 2)
	{
		return false;
	}

	outfin = (buffer[0] & 0x80) != 0;
	outopcode = (uint32_t)(buffer[0] & 0x0F);
	const bool isMasking = (buffer[1] & 0x80) != 0;
	uint32_t payloadlen = buffer[1] & 0x7F;

	uint32_t pos = 2;
	if (payloadlen == 126)
	{
		if (unLen < 4)
		{
			return false;
		}

		payloadlen = (buffer[2] << 8) + buffer[3];
		pos = 4;
	}
	else if (payloadlen == 127)
	{
		if (unLen < 10)
		{
			return false;
		}

		if (buffer[2] != 0 ||
			buffer[3] != 0 ||
			buffer[4] != 0 ||
			buffer[5] != 0)
		{
			return false;
		}

		if ((buffer[6] & 0x80) != 0)
		{
			return false;
		}

		payloadlen = (buffer[6] << 24) + (buffer[7] << 16) + (buffer[8] << 8) + buffer[9];
		pos = 10;
	}

	uint8_t mask[4];
	if (isMasking)
	{
		if (unLen < (pos + 4))
		{
			return false;
		}

		mask[0] = buffer[pos++];
		mask[1] = buffer[pos++];
		mask[2] = buffer[pos++];
		mask[3] = buffer[pos++];
	}

	if (unLen < (pos + payloadlen))
	{
		return false;
	}

	if (isMasking)
	{
		payload.reserve(payloadlen);
		for (size_t i = pos, j = 0; j < payloadlen; i++, j++)
			payload.push_back(buffer[i] ^ mask[j % 4]);
	}
	else
	{
		payload.append((const char*)(buffer + pos), payloadlen);
	}

	frameSize = payloadlen + pos;

	return true;
}

