// -------------------------------------------------------------------------
//    @FileName         :    NFIPacketParse.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <string>
#include "NFComm/NFCore/NFBuffer.h"
#include "NFNetDefine.h"
#include <vector>
#include "NFComm/NFPluginModule/NFServerDefine.h"

class NFIPacketParse
{
public:
	virtual ~NFIPacketParse()
	{
	}

	//////////////////////////////////////////////////////////////
	virtual int DeCodeImpl(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t& nMsgId, uint64_t& nValue) = 0;
	virtual int EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer) = 0;
	/////////////////////////////////////////////////////////////
	static int DeCode(uint32_t packetType, const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t& nMsgId, uint64_t& nValue);
	static int EnCode(uint32_t packetType, const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer);
	static bool EnCodeWeb(const char* strData, const uint32_t unDataLen, std::string& frame, uint32_t frame_type = eWebSocketFrameType::TEXT_FRAME, bool isFin = true, bool masking = false);
	static bool DeCodeWeb(const char* strData, const uint32_t unLen, std::string& payload, uint32_t& outopcode, uint32_t& frameSize, bool& outfin);
	////////////////////////////////////////////////////////////
	static NFIPacketParse* CreatePacketParse(int parseType = 0);
	static void DeletePacketParse(NFIPacketParse* pPacketParse);
private:
	static std::vector<NFIPacketParse*> m_pPacketParse;
};

