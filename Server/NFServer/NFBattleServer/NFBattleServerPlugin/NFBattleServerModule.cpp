// -------------------------------------------------------------------------
//    @FileName         :    NFBattleServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    
//
// -------------------------------------------------------------------------

#include "NFBattleServerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include <NFComm/NFPluginModule/NFLogMgr.h>
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFServer/NFServerCommon/NFServerCommon.h>

NFCBattleServerModule::NFCBattleServerModule(NFIPluginManager* p): m_pNetServerModule(nullptr)
{
	pPluginManager = p;
}

NFCBattleServerModule::~NFCBattleServerModule()
{
}

bool NFCBattleServerModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();

	NFServerConfig* pServerConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_BATTLE);

	NF_ASSERT_MSG(pServerConfig != nullptr, "Can't find the server config for the battle server");

	uint32_t ret = m_pNetServerModule->AddServer(NF_ST_BATTLE, pServerConfig->mServerId, pServerConfig->mMaxConnectNum, pServerConfig->mServerPort);
	NF_ASSERT_RET_VAL(ret != 0, false);
	return true;
}

bool NFCBattleServerModule::AfterInit()
{
	m_pNetServerModule->AddEventCallBack(NF_ST_BATTLE, this, &NFCBattleServerModule::OnNetSocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_BATTLE, this, &NFCBattleServerModule::OnHandleOtherMessage);
	return true;
}

bool NFCBattleServerModule::Shut()
{
	return true;
}

bool NFCBattleServerModule::Execute()
{
	return true;
}

void NFCBattleServerModule::OnNetSocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	switch(nEvent)
	{
	case eMsgType_CONNECTED:
		OnHandleConnected(unLinkId);
		break;
	case eMsgType_DISCONNECTED:
		OnHnadleDisConnected(unLinkId);
		break;
	default:
		assert(0);
	}
}

void NFCBattleServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogNormalWarning(playerId, "Battle Msg Warnning:"+lexical_cast<std::string>(nMsgId), "--No Handled!");
}

void NFCBattleServerModule::OnHandleConnected(const uint32_t unLinkId)
{
}

void NFCBattleServerModule::OnHnadleDisConnected(const uint32_t unLinkId)
{
}
