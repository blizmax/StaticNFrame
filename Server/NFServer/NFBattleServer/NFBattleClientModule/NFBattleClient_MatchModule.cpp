// -------------------------------------------------------------------------
//    @FileName         :    NFBattleClient_MatchModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFBattleClientModule
//
// -------------------------------------------------------------------------

#include "NFBattleClient_MatchModule.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include <NFServer/NFServerCommon/NFServerCommon.h>

NFCBattleClient_MatchModule::NFCBattleClient_MatchModule(NFIPluginManager* p): m_pNetClientModule(nullptr)
{
	pPluginManager = p;
}

NFCBattleClient_MatchModule::~NFCBattleClient_MatchModule()
{
}

bool NFCBattleClient_MatchModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

	NFServerConfig* pServerConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_MATCH);
	NF_ASSERT_RET_VAL(pServerConfig != nullptr, false);

	mMatchServerUnlinkId = m_pNetClientModule->AddServer(NF_ST_MATCH, pServerConfig->mServerIp, pServerConfig->mServerPort);
	NF_ASSERT_RET_VAL(mMatchServerUnlinkId != 0, false);

	return true;
}

bool NFCBattleClient_MatchModule::AfterInit()
{
	return true;
}

bool NFCBattleClient_MatchModule::Shut()
{
	return true;
}

bool NFCBattleClient_MatchModule::Execute()
{
	return true;
}

void NFCBattleClient_MatchModule::OnNetSocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
}

void NFCBattleClient_MatchModule::OnHandleConnected(const uint32_t unLinkId)
{
}

void NFCBattleClient_MatchModule::OnHnadleDisConnected(const uint32_t unLinkId)
{
}
