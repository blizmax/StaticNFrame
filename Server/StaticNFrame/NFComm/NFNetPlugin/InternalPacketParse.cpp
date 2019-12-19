// -------------------------------------------------------------------------
//    @FileName         :    InternalPacketParse.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "InternalPacketParse.h"
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#pragma pack(push)
#pragma pack(1)

struct InternalMsg
{
public:
	InternalMsg() : wSZ(0), wCmdID(0), ulValue(0), opreateId(0)
	{
	}

	uint32_t wSZ;
	uint32_t wCmdID;
	uint64_t ulValue;
	uint32_t opreateId;
};

#pragma pack(pop)

InternalPacketParse::InternalPacketParse()
{
}

int InternalPacketParse::DeCodeImpl(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t& nMsgId, uint64_t& nValue, uint32_t& operateId)
{
	if (strData == nullptr || unLen == 0) return 1;

	InternalMsg* packHead = nullptr;

	if (unLen < static_cast<uint32_t>(sizeof(InternalMsg)))
	{
		return 1;
	}

	int32_t dwMsgSz = *((int32_t*)strData);
	if (dwMsgSz < static_cast<int32_t>(sizeof(InternalMsg)))
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

	packHead = (InternalMsg*)strData; //-V519

	outData = const_cast<char*>(strData + sizeof(InternalMsg));
	outLen = static_cast<uint32_t>(dwMsgSz) - sizeof(InternalMsg);
	nMsgId = packHead->wCmdID;
	nValue = packHead->ulValue;
	operateId = packHead->opreateId;
	allLen = static_cast<uint32_t>(dwMsgSz);
	return 0;
}

int InternalPacketParse::EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const uint32_t operateId, const char* strData, const uint32_t unDataLen, NFBuffer& buffer)
{
	InternalMsg packHead;
	packHead.wCmdID = unMsgID;
	packHead.ulValue = nValue;
	packHead.opreateId = operateId;
	packHead.wSZ = unDataLen + sizeof(InternalMsg);

	buffer.PushData(&packHead, sizeof(InternalMsg));
	buffer.PushData(strData, unDataLen);

	return packHead.wSZ;
}

