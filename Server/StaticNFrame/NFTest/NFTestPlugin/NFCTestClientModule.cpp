// -------------------------------------------------------------------------
//    @FileName         :    NFCTestClientModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------
#include "NFCTestClientModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFCore/NFBuffer.h"
#include "NFComm/NFNetPlugin/NFEventLoop.h"
#include "NFComm/NFNetPlugin/NFEventLoopThread.h"

#include <memory>

int testPrev()
{
	std::cout << "testPrev" << std::endl;
	return 0;
}

bool NFCTestClientModule::Init()
{
#ifdef _MSC_VER
	WSADATA wsaData;
	int nResult = WSAStartup(0x0201, &wsaData);
	if (nResult)
	{
		std::cout << "WSAStartup failed with error code:" << nResult << std::endl;
		return false;
	}
#endif
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_serverId = 1001;
	return true;
}

bool NFCTestClientModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_PROXY, this, &NFCTestClientModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCTestClientModule::OnHandleOtherMessage);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_PROXY, proto::message::ELogin_STC_QueryPlayerData, this, &NFCTestClientModule::OnHandleQueryPlayerData);
	std::string strAccount = NFRandomEnglish(12);
	m_account = strAccount + "_" + lexical_cast<std::string>(0);
	m_charName = strAccount + "_n_" + lexical_cast<std::string>(0);
	m_usId = m_pNetClientModule->AddServer(NF_ST_PROXY, "127.0.0.1", 7002);
	return true;
}

void NFCTestClientModule::OnTimer(uint32_t nTimerID)
{
}

bool NFCTestClientModule::Execute()
{
	return true;
}

bool NFCTestClientModule::BeforeShut()
{
	return true;
}

bool NFCTestClientModule::Shut()
{
	return true;
}

void NFCTestClientModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		proto::message::Login_CTS_QueryPlayerData proReq;

		proReq.set_account(m_account);
		proReq.set_serverid(m_serverId);

		//SendToGateway(Login_FGW_QueryPlayerData, proReq);
		m_pNetClientModule->SendToServerByPB(unLinkId, proto::message::ELogin_CTS_GAMESERVER_QueryPlayerData, proReq, 0);
	}
	else
	{
	}
}

void NFCTestClientModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogNormalWarning(playerId, "Message Not Registed", lexical_cast<std::string>(nMsgId) + " | msg Len:" + lexical_cast<std::string>(nLen));
}

void NFCTestClientModule::OnHandleQueryPlayerData(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, proto::message::Login_STC_QueryPlayerData);

	if (xMsg.rt() == 1)
	{
		SendCreateChar();
	}
}

void NFCTestClientModule::SendCreateChar()
{
	proto::message::Login_CTS_CreateChar proReq;

	proReq.set_account(m_account);
	proReq.set_charname(m_charName);
	proReq.set_serverid(m_serverId);
	proReq.set_weapon(NFRandInt(1, 4));
	proReq.set_sex(NFRandInt(1, 3));

	m_pNetClientModule->SendToServerByPB(m_usId, proto::message::ELogin_CTS_GAMESERVER_CreateChar, proReq, 0);

	NFSLEEP(1000);

	m_pNetClientModule->CloseServer(m_usId);
}