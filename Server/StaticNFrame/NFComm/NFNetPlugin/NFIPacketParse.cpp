// -------------------------------------------------------------------------
//    @FileName			:    {Filename}
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Module           :   {Filename}
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

