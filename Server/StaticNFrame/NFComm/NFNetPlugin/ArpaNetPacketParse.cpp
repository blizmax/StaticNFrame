// -------------------------------------------------------------------------
//    @FileName         :    ArpaNetPacketParse.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "ArpgNetPacketParse.h"
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#pragma pack(push)
#pragma pack(1)

struct stMsg
{
public:
	stMsg() : wSZ(0), wCmdID(0), ulValue(0)
	{
	}

	uint32_t wSZ;
	uint16_t wCmdID;
	uint64_t ulValue;
};

#pragma pack(pop)

ArpgNetPacketParse::ArpgNetPacketParse()
{
}

int ArpgNetPacketParse::DeCodeImpl(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t& nMsgId, uint64_t& nValue)
{
	if (strData == nullptr || unLen == 0) return 1;

	stMsg* packHead = nullptr;

	if (unLen < static_cast<uint32_t>(sizeof(stMsg)))
	{
		return 1;
	}

	int32_t dwMsgSz = *((int32_t*)strData);
	if (dwMsgSz < static_cast<int32_t>(sizeof(stMsg)))
	{
		return 1;
	}

	if (dwMsgSz < 0 || dwMsgSz >= MAX_RECV_BUFFER_SIZE) //-V560
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "net server parse data failed, msgSize:{}", dwMsgSz);
		return -1;
	}

	if (dwMsgSz > static_cast<int32_t>(unLen))
	{
		return 1;
	}

	packHead = (stMsg*)strData; //-V519

	outData = const_cast<char*>(strData + sizeof(stMsg));
	outLen = static_cast<uint32_t>(dwMsgSz) - sizeof(stMsg);
	nMsgId = packHead->wCmdID;
	nValue = packHead->ulValue;
	allLen = static_cast<uint32_t>(dwMsgSz);
	return 0;
}

int ArpgNetPacketParse::EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer)
{
	stMsg packHead;
	packHead.wCmdID = unMsgID;
	packHead.ulValue = nValue;
	packHead.wSZ = unDataLen + sizeof(stMsg);

	buffer.PushData(&packHead, sizeof(stMsg));
	buffer.PushData(strData, unDataLen);

	return packHead.wSZ;
}

