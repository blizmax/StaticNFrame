// -------------------------------------------------------------------------
//    @FileName         :    NFEvppServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFEvppServer.h"
#include "NFLibEvent.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include <NFComm/NFPluginModule/NFINetModule.h>
#include "NetObject.h"

NFEvppServer::NFEvppServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag) : mFlag(flag), mServerType(serverType), mServerId(serverId)
{
	m_eventLoop = nullptr;
	m_tcpServer = nullptr;
	mWebSocket = flag.bWebSocket;
	mPacketParseType = flag.mPacketParseType;
	assert(serverType > NF_ST_NONE && serverType < NF_ST_MAX);
}

NFEvppServer::~NFEvppServer()
{
	if (m_eventLoop)
	{
		NF_SAFE_DELETE(m_eventLoop);
		NF_SAFE_DELETE(m_tcpServer);
	}
}

uint32_t NFEvppServer::GetServerId() const
{
	return mServerId;
}

uint32_t NFEvppServer::GetServerType() const
{
	return mServerType;
}

bool NFEvppServer::IsWebSocket() const
{
	return mWebSocket;
}

bool NFEvppServer::Init()
{
	m_eventLoop = NF_NEW evpp::EventLoop();
	std::string listenAddr = "0.0.0.0:";
	listenAddr += mFlag.nPort;

	m_tcpServer = NF_NEW evpp::TCPServer(m_eventLoop, listenAddr, GetServerName(mServerType), mFlag.nWorkThreadNum);
	
	m_tcpServer->SetConnectionCallback([this](const evpp::TCPConnPtr& conn) {
		if (conn->IsConnected())
		{
			
		}
	});

	m_tcpServer->SetMessageCallback([this](const evpp::TCPConnPtr& conn,
		evpp::Buffer* msg) {
		
	});

	if (m_tcpServer->Init())
	{
		return m_tcpServer->Start();
	}

	return false;
}

bool NFEvppServer::Shut()
{
	m_tcpServer->Stop();
	m_eventLoop->Stop();
	return true;
}

bool NFEvppServer::Finalize()
{
	return true;
}

bool NFEvppServer::Execute()
{
	m_eventLoop->Run();
	return true;
}