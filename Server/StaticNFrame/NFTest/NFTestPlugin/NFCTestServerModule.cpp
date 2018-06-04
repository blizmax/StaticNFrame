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

NFCTestServerModule::NFCTestServerModule(NFIPluginManager* p): m_pNetServerModule(nullptr)
{
	pPluginManager = p;
}

NFCTestServerModule::~NFCTestServerModule()
{

}

bool NFCTestServerModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	//m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCTestServerModule::OnProxySocketEvent);
	//m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCTestServerModule::OnHandleOtherMessage);
	//m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, proto::message::ELogin_CTS_GAMESERVER_QueryPlayerData, &NFCTestServerModule::OnHandleQueryPlayerData);
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
}

void NFCTestServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}

void NFCTestServerModule::OnHandleQueryPlayerData(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}
