#pragma once

#include "NFIPacketParse.h"

#include <set>

class ArpgNetPacketParse : public NFIPacketParse
{
public:
	ArpgNetPacketParse();
	////////////////////////////////////////////////////////////////////
	virtual int DeCodeImpl(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t &nMsgId, uint64_t &value);
	virtual int EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer);
};
