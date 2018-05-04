// -------------------------------------------------------------------------
//    @FileName			:    {Filename}
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Module           :   {Filename}
//
// -------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string>
#include "NFComm/NFCore/NFBuffer.h"

class NFIPacketParse
{
public:
	virtual ~NFIPacketParse() { }
public:
	virtual int DeCodeImpl(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t &nMsgId, uint64_t &nValue) = 0;
	virtual int EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer) = 0;
public:
	static int DeCode(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t &nMsgId, uint64_t &nValue);
	static int EnCode(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer);
public:
	static NFIPacketParse* CreatePacketParse(int parseType = 0);
	static void DeletePacketParse(NFIPacketParse* pPacketParse);
private:
	static NFIPacketParse* m_pPacketParse;
};
