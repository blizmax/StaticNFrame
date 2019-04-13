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

#pragma pack(push)
#pragma pack(1)

struct stMsg
{
public:
	stMsg() : mSZ(0), mCmdID(0), mValue(0)
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

	stMsg* packHead = nullptr;

	if (unLen < static_cast<uint32_t>(sizeof(stMsg)))
	{
		return 1;
	}

	packHead = (stMsg*)strData; //-V519
	uint32_t msgSize = 0;
	uint16_t tmpMsgId = 0;
	uint16_t tmpValue = 0;
	NFDataStream stream(packHead, sizeof(stMsg), false);
	stream.SeekWriteIndex(sizeof(stMsg));
	stream.SeekReadIndex(4);
	stream.Read(&msgSize, sizeof(msgSize));
	stream.Read(&tmpMsgId, sizeof(uint16_t));
	stream.Read(&tmpValue, sizeof(uint16_t));

	if (sizeof(stMsg) + msgSize >= MAX_RECV_BUFFER_SIZE) //-V560
	{
		return -1;
	}

	if (sizeof(stMsg) + msgSize > unLen)
	{
		return 1;
	}

	outData = const_cast<char*>(strData + sizeof(stMsg));
	outLen = msgSize;
	nMsgId = tmpMsgId;
	nValue = tmpValue;
	allLen = sizeof(stMsg) + msgSize;
	return 0;
}

int QiPaiNetPacketParse::EnCodeImpl(const uint32_t unMsgID, const uint64_t nValue, const char* strData, const uint32_t unDataLen, NFBuffer& buffer)
{
	stMsg packHead;
	NFDataStream stream;
	uint16_t tmpMsgId = unMsgID;
	uint16_t tmpValue = nValue;
	stream.SeekWriteIndex(4);
	stream.Write(&unDataLen, sizeof(unDataLen));
	stream.Write(&tmpMsgId, sizeof(tmpMsgId));
	stream.Write(&tmpValue, sizeof(tmpValue));

	buffer.PushData(&packHead, sizeof(stMsg));
	buffer.PushData(strData, unDataLen);

	return unDataLen + sizeof(stMsg);
}

