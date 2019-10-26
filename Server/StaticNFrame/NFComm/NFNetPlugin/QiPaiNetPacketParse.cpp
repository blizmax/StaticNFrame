// -------------------------------------------------------------------------
//    @FileName         :    QiPaiNetPacketParse.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "QiPaiNetPacketParse.h"
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#pragma pack(push)
#pragma pack(1)

struct qipaiMsg
{
public:
	qipaiMsg() : mSZ(0), mCmdID(0), mValue(0)
	{
		mHead[0] = 'h';
		mHead[1] = 'e';
		mHead[2] = 'a';
		mHead[3] = 'd';
	}

	char mHead[4];
	uint32_t mSZ;
	uint16_t mCmdID;
	uint16_t mValue;
};

#pragma pack(pop)

QiPaiNetPacketParse::QiPaiNetPacketParse()
{
}

int QiPaiNetPacketParse::DeCodeImpl(const char* strData, const uint32_t unLen, char*& outData, uint32_t& outLen, uint32_t& allLen, uint32_t& nMsgId, uint64_t& nValue)
{
	if (strData == nullptr || unLen == 0) return 1;

	qipaiMsg* packHead = nullptr;

	if (unLen < static_cast<uint32_t>(sizeof(qipaiMsg)))
	{
		return 1;
	}

	packHead = (qipaiMsg*)strData; //-V519
	uint32_t msgSize = packHead->mSZ;
	uint16_t tmpMsgId = packHead->mCmdID;
	uint16_t tmpValue = packHead->mValue;

	if (sizeof(qipaiMsg) + msgSize >= MAX_RECV_BUFFER_SIZE) //-V560
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "net server parse data failed, msgSize:{}, msgId:{}, value:{}", msgSize, tmpMsgId, tmpValue);
		return -1;
	}

	if (sizeof(qipaiMsg) + msgSize > unLen)
	{
		return 1;
	}

	outData = const_cast<char*>(strData + sizeof(qipaiMsg));
	outLen = msgSize;
	nMsgId = tmpMsgId;
	nValue = tmpValue;
	allLen = sizeof(qipaiMsg) + msgSize;
	return 0;
}

int QiPaiNetPacketParse::EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer)
{
	qipaiMsg packHead;
	packHead.mSZ = unDataLen;
	packHead.mCmdID = (uint16_t)unMsgID;
	packHead.mValue = (uint16_t)nValue;

	buffer.PushData(&packHead, sizeof(qipaiMsg));
	buffer.PushData(strData, unDataLen);

	return unDataLen + sizeof(qipaiMsg);
}

