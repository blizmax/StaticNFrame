// -------------------------------------------------------------------------
//    @FileName         :    NFMyClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/28
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------


#include "NFMyClient.h"
#include "NFEventData.h"

NFMyClient::NFMyClient(uint32_t linkId): m_nSocketId(INVALID_SOCKET), m_usLinkId(0), m_pEventData(nullptr)
{
	m_pEventData = new EventData();
}

NFMyClient::~NFMyClient()
{
}

bool NFMyClient::Connect(const std::string& ip, int port)
{
	SOCKET sock = NFSocket::Create();
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	bool retCode = false;

	retCode = NFSocket::Connect(sock, ip.c_str(), port);
	if (!retCode)
	{
		NFSocket::Close(sock);
		return false;
	}

	m_pEventData->sock_flag = SOCKET_CONNECT;
	m_pEventData->sock = sock;

	retCode = m_pEventData->poll->AddEvent(sock, EVENT_WRITE, m_pEventData);
	if (!retCode)
	{
		NFSocket::Close(sock);
		return false;
	}

	return true;
}
