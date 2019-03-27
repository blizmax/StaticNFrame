// -------------------------------------------------------------------------
//    @FileName         :    NetObject.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "NetObject.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NFComm/NFCore/NFSHA2.h"
#include "NFComm/NFCore/NFBase64.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFHttpFormat.h"
#include "NFComm/NFCore/NFStringUtility.h"

/**
* @brief libevent�����ݻص�
*
* @param pEv   libevent��д������
* @param pArg  ����Ĳ���
* @return
*/

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

void NetObject::conn_recvcb(struct bufferevent* pEv, void* pArg)
{
	NetObject* pObject = static_cast<NetObject*>(pArg);
	if (pObject == nullptr) return;

	if (pObject->GetNeedRemove()) return;

	if (!pObject->OnRecvData(pEv))
	{
		pObject->OnHandleDisConnect();
	}
}

/**
* @brief libevent�����¼��ص�
*
* @param pEv		libevent��д������
* @param events	�¼�
* @param pArg		����Ĳ���
* @return
*/
void NetObject::conn_eventcb(struct bufferevent* pEv, short events, void* pArg)
{
	NetObject* p = static_cast<NetObject*>(pArg);
	if (p == nullptr) return;

	if (events & BEV_EVENT_CONNECTED)
	{
		p->OnHandleConnect(static_cast<SOCKET>(bufferevent_getfd(pEv)));
	}

	if (events & BEV_EVENT_EOF)
	{
		p->OnHandleDisConnect();
		return;
	}

	if (events & BEV_EVENT_ERROR)
	{
#ifdef _WIN32
		if (ArkGetLastError() == WSAEISCONN)
		{
			p->OnHandleConnect(static_cast<SOCKET>(bufferevent_getfd(pEv)));;
			return;
		}
#endif
		p->OnHandleDisConnect();
		//NFLogError("NetError | CloseProc Error Code ",std::string(evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR())));
	}
}

/**
* @brief libeventд���ݻص�
*
* @param pEv   libevent��д������
* @param pArg  ����Ĳ���
* @return
*/
void NetObject::conn_writecb(struct bufferevent* pEv, void* pArg)
{
	// Intentionally unimplemented...
}

NetObject::NetObject() : m_pBev(nullptr), m_usLinkId(0), m_port(0), mNeedRemove(false)
{
	m_nSocketId = INVALID_SOCKET;
	m_buffer.AssureSpace(MAX_RECV_BUFFER_SIZE);
	mHandleShark = false;
	mWebSocket = false;
	mIsServer = true;
}

NetObject::~NetObject()
{
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
	}
	m_nSocketId = INVALID_SOCKET;
	m_buffer.Clear();
	m_pBev = nullptr;
}

bool NetObject::IsHandleShark() const
{
	return mHandleShark;
}

/**
* @brief �����websocket�Ļ�����Ҫ���handleshark
*
* @return bool
*/
void NetObject::SetHandleShark(bool b)
{
	mHandleShark = b;
}

void NetObject::SetWebSocket(bool b)
{
	mWebSocket = b;
}

bool NetObject::IsWebSocket() const
{
	return mWebSocket;
}

std::string NetObject::GetStrIp() const
{
	return m_strIp;
}

void NetObject::SetStrIp(std::string val)
{
	m_strIp = val;
}

uint32_t NetObject::GetPort() const
{
	return m_port;
}

void NetObject::SetPort(uint32_t val)
{
	m_port = val;
}

void NetObject::SetIsServer(bool b)
{
	mIsServer = b;
}

std::string NetObject::HandleSharkReturn()
{
	std::string request = "HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: ";

	std::string server_key = mHeaderMap["Sec-WebSocket-Key"];
	server_key += MAGIC_KEY;

	server_key = NFSHA2::encode(server_key);

	server_key = NFBase64::Encode(server_key);

	server_key += "\r\n\r\n";
	request += server_key;
	return request;
}

void NetObject::SendHandleShark()
{
	HttpRequest request;
	request.setMethod(HttpRequest::HTTP_METHOD::HTTP_METHOD_GET);
	request.setUrl(m_origin);
	request.addHeadValue("Host", m_strIp + ":" + lexical_cast<std::string>(m_port));
	request.addHeadValue("Upgrade", "websocket");
	request.addHeadValue("Pragma", "no-cache");
	request.addHeadValue("Cache-Control", "no-cache");
	request.addHeadValue("Connection", "Upgrade");
	request.addHeadValue("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ==");
	request.addHeadValue("Sec-WebSocket-Version", "13");
	request.addHeadValue("Content-Type", "text/plain;");

	std::string requestStr = request.getResult();

	Send(requestStr.data(), requestStr.length());
}

/**
* @brief ����shark info
*
* @return void
*/
int NetObject::HandleSharkInfo()
{
	std::istringstream s(std::string(m_buffer.ReadAddr(), m_buffer.ReadableSize()));
	std::string request;

	std::getline(s, request);
	if (request[request.size() - 1] == '\r') {
		request.erase(request.end() - 1);
	}
	else {
		return -1;
	}

	std::string header;
	std::string::size_type end;

	while (std::getline(s, header) && header != "\r") {
		if (header[header.size() - 1] != '\r') {
			continue; //end
		}
		else {
			header.erase(header.end() - 1);	//remove last char
		}

		end = header.find(": ", 0);
		if (end != std::string::npos) {
			std::string key = header.substr(0, end);
			std::string value = header.substr(end + 2);
			mHeaderMap[key] = value;
		}
	}

	return 0;
}

int NetObject::Dismantle()
{
	if (mIsServer && mWebSocket)
	{
		if (!mHandleShark)
		{
			if (HandleSharkInfo() < 0)
			{
				m_buffer.Consume(m_buffer.ReadableSize());
				return -1;
			}
			m_buffer.Consume(m_buffer.ReadableSize());
			std::string request = HandleSharkReturn();
			Send(request.c_str(), request.size());
			mHandleShark = true;
			return 1;
		}
		else
		{
			if (m_buffer.ReadableSize() <= 0)
			{
				return 1;
			}

			mParseString.clear();
			uint32_t allLen = 0;
			uint32_t opcode = eWebSocketFrameType::ERROR_FRAME;
			bool isFinish = false;
			if (NFIPacketParse::DeCodeWeb(m_buffer.ReadAddr(), m_buffer.ReadableSize(), mParseString, opcode, allLen, isFinish) == false)
			{
				m_buffer.Consume(m_buffer.ReadableSize());
				return 1;
			}

			// �����ǰfram��finΪfalse����opcodeΪ���������򽫵�ǰframe��payload��ӵ�cache
			if (!isFinish || opcode == eWebSocketFrameType::CONTINUATION_FRAME)
			{
				mCacheFrame += mParseString;
				mParseString.clear();
			}
			// �����ǰfram��finΪfalse������opcode��Ϊ�����������ʾ�յ��ֶ�payload�ĵ�һ����(frame)����Ҫ���浱ǰframe��opcode
			if (!isFinish && opcode != eWebSocketFrameType::CONTINUATION_FRAME)
			{
				mWSFrameType = opcode;
			}

			if (isFinish)
			{
				// ���finΪtrue������opcodeΪ�����������ʾ�ֶ�payloadȫ��������ϣ������Ҫ��ȡ֮ǰ��һ���յ��ֶ�frame��opcode��Ϊ����payload������
				if (opcode == eWebSocketFrameType::CONTINUATION_FRAME)
				{
					if (!mCacheFrame.empty())
					{
						mParseString = std::move(mCacheFrame);
						mCacheFrame.clear();
					}
					opcode = mWSFrameType;
				}
				else if (opcode == eWebSocketFrameType::CLOSE_FRAME)
				{
					NFLogError(NF_LOG_NET_PLUGIN, 0, "recv close frame, close connection!");

					m_buffer.Consume(allLen);
					return 0;
				}
				else if (opcode == eWebSocketFrameType::PONG_FRAME)
				{
					NFLogError(NF_LOG_NET_PLUGIN, 0, "recv pong frame!");
				}
				else if (opcode == eWebSocketFrameType::PING_FRAME)
				{
					NFLogError(NF_LOG_NET_PLUGIN, 0, "recv ping frame!");
				}

				if (NFStringUtility::IsUTF8String(mParseString))
				{
					OnHandleMsgPeer(eMsgType_RECIVEDATA, m_usLinkId, mParseString);
				}
			}
			
			m_buffer.Consume(allLen);
			return 0;
		}
	}
	else if (mIsServer == false && mWebSocket)
	{
		if (m_buffer.ReadableSize() <= 0)
		{
			return 1;
		}

		if (mHandleShark == false)
		{
			if (HandleSharkInfo() < 0)
			{
				m_buffer.Consume(m_buffer.ReadableSize());
				return -1;
			}

			mHandleShark = true;
			OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0);
			m_buffer.Consume(m_buffer.ReadableSize());
			return 0;
		}
		else
		{
			//OnHandleMsgPeer(eMsgType_RECIVEDATA, m_usLinkId, m_buffer.ReadAddr(), m_buffer.ReadableSize(), 0, 0);
			//m_buffer.Consume(m_buffer.ReadableSize());
			if (m_buffer.ReadableSize() <= 0)
			{
				return 1;
			}

			mParseString.clear();
			uint32_t allLen = 0;
			uint32_t opcode = eWebSocketFrameType::ERROR_FRAME;
			bool isFinish = false;
			if (NFIPacketParse::DeCodeWeb(m_buffer.ReadAddr(), m_buffer.ReadableSize(), mParseString, opcode, allLen, isFinish) == false)
			{
				m_buffer.Consume(m_buffer.ReadableSize());
				return 1;
			}

			// �����ǰfram��finΪfalse����opcodeΪ���������򽫵�ǰframe��payload��ӵ�cache
			if (!isFinish || opcode == eWebSocketFrameType::CONTINUATION_FRAME)
			{
				mCacheFrame += mParseString;
				//NFLogError("cacheFrame:{}", mCacheFrame);
				//NFLogError("parseString:{}", mParseString);
				mParseString.clear();
			}
			// �����ǰfram��finΪfalse������opcode��Ϊ�����������ʾ�յ��ֶ�payload�ĵ�һ����(frame)����Ҫ���浱ǰframe��opcode
			if (!isFinish && opcode != eWebSocketFrameType::CONTINUATION_FRAME)
			{
				mWSFrameType = opcode;
			}

			//NFLogError("parseString:{}", mParseString);
			if (isFinish)
			{
				// ���finΪtrue������opcodeΪ�����������ʾ�ֶ�payloadȫ��������ϣ������Ҫ��ȡ֮ǰ��һ���յ��ֶ�frame��opcode��Ϊ����payload������
				if (opcode == eWebSocketFrameType::CONTINUATION_FRAME)
				{
					if (!mCacheFrame.empty())
					{
						mParseString = std::move(mCacheFrame);
						mCacheFrame.clear();
					}
					opcode = mWSFrameType;
				}
				else if (opcode == eWebSocketFrameType::CLOSE_FRAME)
				{
					NFLogError(NF_LOG_NET_PLUGIN, 0, "recv close frame, close connection!");
					m_buffer.Consume(allLen);
					return 0;
				}
				else if (opcode == eWebSocketFrameType::PONG_FRAME)
				{
					NFLogError(NF_LOG_NET_PLUGIN, 0, "recv pong frame!");
				}
				else if (opcode == eWebSocketFrameType::PING_FRAME)
				{
					NFLogError(NF_LOG_NET_PLUGIN, 0, "recv ping frame!");
				}

				if (NFStringUtility::IsUTF8String(mParseString))
				{
					OnHandleMsgPeer(eMsgType_RECIVEDATA, m_usLinkId, mParseString);
				}
				else
				{
					//NFLogError("recv data not utf8:{}", mParseString);
				}
			}

			m_buffer.Consume(allLen);
			return 0;
		}
	}
	char* outData = nullptr;
	uint32_t outLen = 0;
	uint32_t allLen = 0;
	uint32_t nMsgId = 0;
	uint64_t nValue = 0;
	int ret = NFIPacketParse::DeCode(m_buffer.ReadAddr(), m_buffer.ReadableSize(), outData, outLen, allLen, nMsgId, nValue);
	if (ret < 0)
	{
		m_buffer.Consume(m_buffer.ReadableSize());
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

bufferevent* NetObject::GetBev() const
{
	return m_pBev;
}

void NetObject::SetBev(bufferevent* bev)
{
	m_pBev = bev;
}

uint32_t NetObject::GetLinkId() const
{
	return m_usLinkId;
}

void NetObject::SetLinkId(uint32_t linkId)
{
	m_usLinkId = linkId;
}

void NetObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, const std::string& strMsg)
{
	switch (type)
	{
	case eMsgType_RECIVEDATA:
	{
		if (mRecvCB)
		{
			mRecvCB(usLink, 0, 0, strMsg.data(), strMsg.length());
		}
	}
	break;
	case eMsgType_CONNECTED:
	{
		if (mIsServer == false && mWebSocket == true)
		{
			if (!mHandleShark)
			{
				SendHandleShark();
				return;
			}
		}
		if (mEventCB)
		{
			mEventCB(type, usLink);
		}
	}
	break;
	case eMsgType_DISCONNECTED:
	{
		if (mIsServer == false && mWebSocket == true)
		{
			mHandleShark = false;
		}
		if (mEventCB)
		{
			mEventCB(type, usLink);
		}
	}
	break;
	default:
		break;
	}
}

void NetObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
{
	switch (type)
	{
	case eMsgType_RECIVEDATA:
		{
			if (mRecvCB)
			{
				mRecvCB(usLink, nValue, nMsgId, pBuf, sz);
			}
		}
		break;
	case eMsgType_CONNECTED:
	{
		if (mIsServer == false && mWebSocket == true)
		{
			if (!mHandleShark)
			{
				SendHandleShark();
				return;
			}
		}
		if (mEventCB)
		{
			mEventCB(type, usLink);
		}
	}
	break;
	case eMsgType_DISCONNECTED:
		{
			if (mIsServer == false && mWebSocket == true)
			{
				mHandleShark = false;
			}
			if (mEventCB)
			{
				mEventCB(type, usLink);
			}
		}
		break;
	default:
		break;
	}
}

bool NetObject::OnRecvData(bufferevent* pBufEv)
{
	if (NULL == pBufEv)
	{
		return false;
	}
	struct evbuffer* pEvbuff = bufferevent_get_input(pBufEv);
	if (NULL == pEvbuff)
	{
		return false;
	}

	int32_t ilen = static_cast<int32_t>(evbuffer_get_length(pEvbuff));
	if (ilen <= 0)
	{
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
		if (GetNeedRemove())
		{
			return false;
		}
	}

	return true;
}

bool NetObject::GetNeedRemove() const
{
	return mNeedRemove;
}

void NetObject::SetNeedRemove(bool val)
{
	mNeedRemove = val;
}

void NetObject::OnHandleConnect(SOCKET nSocket)
{
	SetSocketId(nSocket);
	OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NetObject::OnHandleDisConnect()
{
	SetNeedRemove(true);
	CloseObject();
	OnHandleMsgPeer(eMsgType_DISCONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NetObject::CloseObject()
{
	if (m_nSocketId != INVALID_SOCKET)
	{
		bufferevent_disable(m_pBev, EV_READ | EV_WRITE);
		evutil_closesocket(m_nSocketId);
		m_nSocketId = INVALID_SOCKET;
	}
}

void NetObject::SetSocketId(SOCKET nSocket)
{
	m_nSocketId = nSocket;
	int tcp_nodelay = 1;
	int ret = 0;
	ret = setsockopt(nSocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char *>(&tcp_nodelay), sizeof(tcp_nodelay));
	if (ret < 0)
	{
		std::cout << "setsockopt TCP_NODELAY failed" << std::endl;
	}
	int bufflen = 65536;
	ret = setsockopt(nSocket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&bufflen), sizeof(bufflen));
	if (ret < 0)
	{
		std::cout << "setsockopt SO_RCVBUF failed" << std::endl;
	}
	ret = setsockopt(nSocket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&bufflen), sizeof(bufflen));
	if (ret < 0)
	{
		std::cout << "setsockopt SO_SNDBUF failed" << std::endl;
	}

	ret = evutil_make_socket_nonblocking(nSocket);
	if (ret < 0)
	{
		std::cout << "evutil_make_socket_nonblocking failed" << std::endl;
	}
}

bool NetObject::Send(const void* pData, uint32_t unSize)
{
	if (!GetNeedRemove() && bufferevent_get_enabled(m_pBev) != 0)
	{
		int nRet = bufferevent_write(m_pBev, pData, unSize);
		if (nRet < 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "NetError | send msg error !");
			return false;
		}
		return true;
	}
	return false;
}

