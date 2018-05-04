#include "NetObject.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"

NetObject::NetObject()
{
	m_nSocket = INVALID_SOCKET;
	m_pBev = NULL;
	m_usConn = 0;
	m_usLinkId = 0;
}


NetObject::~NetObject()
{
}

bool NetObject::Init()
{
	m_buffer.AssureSpace(MAX_RECV_BUFFER_SIZE);
	return true;
}

int NetObject::Dismantle()
{
	char* outData = nullptr;
	uint32_t outLen = 0;
	uint32_t allLen = 0;
	uint32_t nMsgId = 0;
	uint64_t nValue = 0;
	int ret = NFIPacketParse::DeCode(m_buffer.ReadAddr(),m_buffer.ReadableSize(), outData, outLen, allLen, nMsgId, nValue);
	if (ret < 0)
	{
		bufferevent_disable(m_pBev, EV_READ | EV_WRITE);
		evutil_closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET;
		return -1;
	}
	else if (ret > 0)
	{
		return 1;
	}
	else
	{
		OnHandleMsgPeer(eMsgType_RECIVEDATA, m_usLinkId, outData, outLen, nMsgId, nValue);
		m_buffer.Consume(allLen);
		return 0;
	}
}

bool  NetObject::OnRecvData(bufferevent* pBufEv)
{
	if (NULL == pBufEv) {
		return false;
	}
	struct evbuffer * pEvbuff = bufferevent_get_input(pBufEv);
	if (NULL == pEvbuff){
		return false;
	}

	int32_t  ilen = (int32_t)evbuffer_get_length(pEvbuff);
	if (ilen <= 0){
		return true;
	}

	m_buffer.AssureSpace(ilen);

	if (evbuffer_remove(pEvbuff, m_buffer.WriteAddr(), ilen) > 0)
	{
		m_buffer.Produce(ilen);
	}
	else
	{
		return false;
	}

	while (true)
	{
		int ret = Dismantle();
		if (ret < 0)
		{
			return false;
		}
		else if (ret > 0)
		{
			break;
		}
	}

	return true;
}

void NetObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
{
	NFThreadNetMsg* pNetMsg = new NFThreadNetMsg();
	pNetMsg->usLink = usLink;
	pNetMsg->eType = type;
	pNetMsg->nMsgId = nMsgId;
	pNetMsg->nValue = nValue;
	pNetMsg->nBuffer.Init(pBuf, sz);

	m_threadNetMsgs.Push(pNetMsg);
}

void NetObject::FlushData()
{
	if (NULL == m_pBev) {
		return;
	}
	int size = evbuffer_get_length(bufferevent_get_output(m_pBev));
	if (size > 0)
	{
		evbuffer_unfreeze(bufferevent_get_output(m_pBev), 1);
		evbuffer_write(bufferevent_get_output(m_pBev), m_nSocket);
	}
}

void NetObject::Reset()
{
	m_buffer.Clear();
}