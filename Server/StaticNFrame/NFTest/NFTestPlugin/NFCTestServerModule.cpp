// -------------------------------------------------------------------------
//    @FileName         :    NFCTestServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestServerModule.h"
#include "NFComm/NFPluginModule/NFINetServerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFMessageDefine/NFMsgDefine.h"

NFCTestServerModule::NFCTestServerModule(NFIPluginManager* p) : m_pNetServerModule(nullptr)
{
	pPluginManager = p;
}

NFCTestServerModule::~NFCTestServerModule()
{
}

bool NFCTestServerModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCTestServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCTestServerModule::OnHandleOtherMessage);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, proto::message::ELogin_CTS_GAMESERVER_QueryPlayerData, this, &NFCTestServerModule::OnHandleQueryPlayerData);
	m_pNetServerModule->AddServer(NF_ST_PROXY, 1, 1, 7002);
	return true;
}

bool NFCTestServerModule::AfterInit()
{
	return true;
}

bool NFCTestServerModule::Execute()
{
	return true;
}

bool NFCTestServerModule::BeforeShut()
{
	return true;
}

bool NFCTestServerModule::Shut()
{
	return true;
}

void NFCTestServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	switch (nEvent)
	{
	case eMsgType_CONNECTED:
	{
	}
	break;
	case eMsgType_DISCONNECTED:
	{
		auto iter = m_unLinkIdToAccount.find(unLinkId);
		if (iter != m_unLinkIdToAccount.end())
		{
			std::string account = iter->second;
			auto it_data = m_playerDataMap.find(account);
			if (it_data != m_playerDataMap.end())
			{
				auto pData = it_data->second;
				if (pData)
				{
					pData->m_usLinkId = 0;
				}
			}
			m_unLinkIdToAccount.erase(iter);
		}
	}
	break;
	default:
		break;
	}
}

void NFCTestServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogNormalWarning(playerId, "Message Not Registed", lexical_cast<std::string>(nMsgId) + " | msg Len:" + lexical_cast<std::string>(nLen));
}

void NFCTestServerModule::OnHandleQueryPlayerData(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
    proto::message::Login_CTS_QueryPlayerData xMsg;
	if (!xMsg.ParseFromArray(msg, nLen))
    {
		NFLogNormalError(playerId, "Parse Message Failed:", lexical_cast<std::string>(nMsgId));
        return ;									
    }

	std::string account = xMsg.account();

	TestPlayerData* pData = nullptr; 
	auto iter = m_playerDataMap.find(account);
	if (iter != m_playerDataMap.end())
	{
		pData = iter->second;
	}

	if (pData == nullptr)
	{
		pData = new TestPlayerData();
		m_playerDataMap[account] = pData;
	}

	m_unLinkIdToAccount[unLinkId] = account;

	pData->m_account = account;
	pData->m_usLinkId = unLinkId;

	proto::message::Login_STC_QueryPlayerData respone;
	respone.set_rt(1);
	m_pNetServerModule->SendToServerByPB(unLinkId, proto::message::ELogin_STC_QueryPlayerData, respone, 0);
}