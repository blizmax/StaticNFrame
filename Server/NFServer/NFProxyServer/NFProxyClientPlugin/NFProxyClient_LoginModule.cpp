// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_LoginModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClient_LoginModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCProxyClient_LoginModule::NFCProxyClient_LoginModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCProxyClient_LoginModule::~NFCProxyClient_LoginModule()
{
}

bool NFCProxyClient_LoginModule::Init()
{
	return true;
}

bool NFCProxyClient_LoginModule::AfterInit()
{
	FindModule<NFINetClientModule>()->AddEventCallBack(NF_ST_LOGIN, this, &NFCProxyClient_LoginModule::OnProxySocketEvent);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, this, &NFCProxyClient_LoginModule::OnHandleOtherMessage);

	return true;
}

bool NFCProxyClient_LoginModule::Execute()
{
	return true;
}

bool NFCProxyClient_LoginModule::BeforeShut()
{
	return true;
}

bool NFCProxyClient_LoginModule::Shut()
{
	mUnlinkLoginMap.ClearAll();
	mLoginMap.ClearAll();
	return true;
}

void NFCProxyClient_LoginModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Connect Login Server Success!");

		//连接成功，发送网关服务器IP以及数据给Login服务器
		RegisterServer(unLinkId);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server DisConnect Login Server!");

		OnClientDisconnect(unLinkId);
	}
}

void NFCProxyClient_LoginModule::OnClientDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkLoginMap.GetElement(unLinkId);
	if (pServerData)
	{
		//proxy server 与 world server 一旦断开，不需要主动重新连接, 清理后将关闭这个unLinkId
		pServerData->mUnlinkId = 0;
		pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);

		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the login server disconnect, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
			, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_PROXY, NF_ST_LOGIN, unLinkId, pServerData);

		mUnlinkLoginMap.RemoveElement(unLinkId);

		FindModule<NFINetClientModule>()->CloseServer(unLinkId);
	}
}

void NFCProxyClient_LoginModule::OnHandleServerReport(const NFMsg::ServerInfoReport& xData)
{
	if (xData.server_type() != NF_ST_LOGIN) return;

	NF_SHARE_PTR<NFServerData> pServerData = GetServerByServerId(xData.server_id());
	if (pServerData == nullptr)
	{
		pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
		mLoginMap.AddElement(xData.server_id(), pServerData);

		pServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_LOGIN, xData.server_ip(), xData.server_port());
		mUnlinkLoginMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	if (pServerData->mUnlinkId <= 0)
	{
		pServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_LOGIN, xData.server_ip(), xData.server_port());
		mUnlinkLoginMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	pServerData->mServerInfo = xData;
}

void NFCProxyClient_LoginModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, playerId, "msg:{} not handled!", nMsgId);
}

void NFCProxyClient_LoginModule::RegisterServer(uint32_t linkId)
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		FindModule<NFINetClientModule>()->SendToServerByPB(linkId, EGMI_NET_PROXY_TO_LOGIN_REGISTER, xMsg, 0);
	}

	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkLoginMap.GetElement(linkId);
	if (pServerData)
	{
		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_PROXY, NF_ST_LOGIN, linkId, pServerData);
	}
}