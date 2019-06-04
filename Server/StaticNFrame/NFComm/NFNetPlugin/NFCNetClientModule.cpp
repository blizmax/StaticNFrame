// -------------------------------------------------------------------------
//    @FileName         :    NFCNetClientModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "NFCNetClientModule.h"
#include <iostream>
#include "NFIPacketParse.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFEvppClient.h"
#include "NFClient.h"

NFCNetClientModule::NFCNetClientModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mxServerMap.resize(NF_ST_MAX);
	for (int serverType = NF_ST_NONE + 1; serverType < NF_ST_MAX; serverType++)
	{
		AddEventCallBack((NF_SERVER_TYPES)serverType, this, &NFCNetClientModule::OnHandleNetEvent);
	}
	mxSendBuffer.AssureSpace(MAX_SEND_BUFFER_SIZE);
	m_pLuaScriptModule = nullptr;
	m_eventLoop = NF_NEW evpp::EventLoopThread();
	m_eventLoop->set_name("NFEvppClientThread");
	m_eventLoop->Start();
}

NFCNetClientModule::~NFCNetClientModule()
{
}

bool NFCNetClientModule::Awake()
{
	//可以允许Lua Module不存在
	m_pLuaScriptModule = FindModule<NFILuaScriptModule>();
	return true;
}


bool NFCNetClientModule::Init()
{
	return true;
}

bool NFCNetClientModule::AfterInit()
{
	return true;
}

bool NFCNetClientModule::BeforeShut()
{
	mxCallBack.clear();
	return true;
}

bool NFCNetClientModule::Shut()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->Shut();
			}
		}
	}
	m_eventLoop->Stop(true);
	return true;
}

bool NFCNetClientModule::Finalize()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->Finalize();
				NF_SAFE_DELETE(mxServerMap[i][j]);
			}
		}
	}
	mxServerMap.clear();
	
	if (m_eventLoop)
	{
		NF_SAFE_DELETE(m_eventLoop);
	}
	return true;
}

bool NFCNetClientModule::Execute()
{
	ExecuteClose();
	ProcessExecute();
	return true;
}

uint32_t NFCNetClientModule::GetFreeUnLinkId(NF_SERVER_TYPES eServerType)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		size_t sz = mxServerMap[eServerType].size();

		for (size_t index = 0; index < sz; index++)
		{
			if (mxServerMap[eServerType][index] == nullptr)
			{
				return GetUnLinkId(eServerType, index);
			}
		}

		if (sz >= MAX_SERVER_TYPE_CLIENT_COUNT)
		{
			return 0;
		}

		mxServerMap[eServerType].push_back(nullptr);

		return GetUnLinkId(eServerType, sz);
	}
	return 0;
}

uint32_t NFCNetClientModule::AddWebServer(NF_SERVER_TYPES eServerType, const std::string& url)
{
	struct evhttp_uri* http_uri = evhttp_uri_parse(url.c_str());
	if (http_uri == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_uri_parse err. url:{}", url);
		return 0;
	}

	const char*  scheme = evhttp_uri_get_scheme(http_uri);
	if (scheme == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "http_uri:{},scheme == NULL err. ", url);
		return 0;
	}

	std::string schemeStr = scheme;

	if (schemeStr != "ws" && schemeStr != "wss")
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "http_uri:{}, is not ws or wss err. ", url);
		return 0;
	}

	const char*  host = evhttp_uri_get_host(http_uri);
	if (host == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "http_uri:{},host == NULL err. ", url);
		return 0;
	}

	std::string hostStr = host;
	std::string pathStr;

	int port = evhttp_uri_get_port(http_uri);

	const char* path = evhttp_uri_get_path(http_uri);
	if (path != NULL)
	{
		pathStr = path;
	}

	evhttp_uri_free(http_uri);

	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		uint32_t usId = GetFreeUnLinkId(eServerType);
		if (usId == 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Add Connecting Server Failed! Ip:{}, Port:{}", hostStr, port);
			return 0;
		}

		uint32_t index = GetServerIndexFromUnlinkId(usId);

		NFClientFlag flag;
		flag.strIP = hostStr;
		flag.nPort = port;
		flag.bWebSocket = true;
		flag.origin = url;
		NFClient* pClient = NF_NEW NFClient(usId, flag);
		pClient->SetRecvCB((NFINetModule*)this, &NFINetModule::OnReceiveNetPack);
		pClient->SetEventCB((NFINetModule*)this, &NFINetModule::OnSocketNetEvent);
		if (index < mxServerMap[eServerType].size() && mxServerMap[eServerType][index] == nullptr)
		{
			mxServerMap[eServerType][index] = pClient;
			return pClient->GetLinkId();
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Add Connecting Server Failed! Ip:{}, Port:{}", hostStr, port);
		}
		NF_SAFE_DELETE(pClient);
	}
	return 0;
}

uint32_t NFCNetClientModule::AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, uint32_t nPort, bool bWebSocket)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		uint32_t usId = GetFreeUnLinkId(eServerType);
		if (usId == 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Add Connecting Server Failed! Ip:%s, Port:%d, than max connection:65535", strIp.c_str(), nPort);
			return 0;
		}

		uint32_t index = GetServerIndexFromUnlinkId(usId);

		NFClientFlag flag;
		flag.strIP = strIp;
		flag.nPort = nPort;
		flag.bWebSocket = bWebSocket;
		//NFIClient* pClient = NF_NEW NFClient(usId, flag);

		NFEvppClient* pClient = NF_NEW NFEvppClient(m_eventLoop, usId, flag);
		pClient->SetRecvCB((NFINetModule*)this, &NFINetModule::OnReceiveNetPack);
		pClient->SetEventCB((NFINetModule*)this, &NFINetModule::OnSocketNetEvent);
		if (index < mxServerMap[eServerType].size() && mxServerMap[eServerType][index] == nullptr)
		{
			mxServerMap[eServerType][index] = pClient;
			return pClient->GetLinkId();
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Add Connecting Server Failed! Ip:%s, Port:%d, than max connection:65535", strIp.c_str(), nPort);
		}
		NF_SAFE_DELETE(pClient);
	}
	return 0;
}

void NFCNetClientModule::CloseServer(const uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			pClient->CloseServer();
			pClient->SetStatus(eConnectStatus_REMOVE);
		}
	}
}

void NFCNetClientModule::CloseServerByServerType(NF_SERVER_TYPES eServerType)
{
	if (eServerType >= 0 && eServerType < NF_ST_MAX)
	{
		for (size_t j = 0; j < mxServerMap[eServerType].size(); j++)
		{
			if (mxServerMap[eServerType][j])
			{
				mxServerMap[eServerType][j]->Shut();
				mxServerMap[eServerType][j]->SetStatus(eConnectStatus_REMOVE);
			}
		}
	}
}

void NFCNetClientModule::CloseAllServer()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->Shut();
				mxServerMap[i][j]->SetStatus(eConnectStatus_REMOVE);
			}
		}
	}
}

void NFCNetClientModule::SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendByServerID(unLinkId, nMsgID, strData, nPlayerID);
}

void NFCNetClientModule::SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
	SendByServerID(unLinkId, nMsgID, strData.c_str(), strData.length(), nPlayerID);
}

void NFCNetClientModule::SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			SendMsg(pClient, nMsgID, msg, nLen, nPlayerID);
		}
		else
		{
			assert(0);
		}
	}
}

void NFCNetClientModule::SendMsg(NFIClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	if (pClient == nullptr) return;

	mxSendBuffer.Clear();
	if (pClient->IsWebSocket())
	{
		std::string frame;
		NFIPacketParse::EnCodeWeb(msg, nLen, frame, BINARY_FRAME, true, true);
		mxSendBuffer.PushData((const void*)frame.data(), frame.length());
	}
	else
	{
		NFIPacketParse::EnCode(0, nMsgID, nPlayerID, msg, nLen, mxSendBuffer);
	}
	
	if (pClient)
	{
		pClient->Send(mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
	}
	mxSendBuffer.Clear();
}

void NFCNetClientModule::SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
	SendToAllServer(nMsgID, strData.c_str(), strData.length(), nPlayerID);
}

void NFCNetClientModule::SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				SendMsg(mxServerMap[i][j], nMsgID, msg, nLen, nPlayerID);
			}
		}
	}
}

void NFCNetClientModule::SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendToAllServer(nMsgID, strData, nPlayerID);
}

void NFCNetClientModule::SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
	SendToAllServer(eServerType, nMsgID, strData.c_str(), strData.length(), nPlayerID);
}

void NFCNetClientModule::SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	if (eServerType >= 0 && eServerType < NF_ST_MAX)
	{
		for (size_t j = 0; j < mxServerMap[eServerType].size(); j++)
		{
			if (mxServerMap[eServerType][j])
			{
				SendMsg(mxServerMap[eServerType][j], nMsgID, msg, nLen, nPlayerID);
			}
		}
	}
}

void NFCNetClientModule::SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendToAllServer(nMsgID, strData, nPlayerID);
}

void NFCNetClientModule::ProcessExecute()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			auto pClient = mxServerMap[i][j];
			if (pClient)
			{
				switch (pClient->GetStatus())
				{
				case eConnectStatus_Disconnect:
					{
						pClient->SetStatus(eConnectStatus_RECONNECT);
					}
					break;
				case eConnectStatus_Connecting:
					{
						pClient->Execute();
					}
					break;
				case eConnectStatus_ConnectOk:
					{
						pClient->Execute();
						KeepState(pClient);
					}
					break;
				case eConnectStatus_RECONNECT:
					{
						if (pClient->GetLastActionTime() + 10000 >= (uint64_t)NFGetTime())
						{
							break;
						}

						pClient->ExecuteClose();
						pClient->SetStatus(eConnectStatus_Connecting);
						pClient->Init();
					}
					break;
				case eConnectStatus_REMOVE:
					{
						NF_SAFE_DELETE(pClient);
						mxServerMap[i][j] = nullptr;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void NFCNetClientModule::ExecuteClose()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j] && mxServerMap[i][j]->IsNeedRemve())
			{
				NF_SAFE_DELETE(mxServerMap[i][j]);
				mxServerMap[i][j] = nullptr;
			}
		}
	}
}

void NFCNetClientModule::KeepState(NFIClient* pClient)
{
	if (pClient)
	{
		if (pClient->GetLastActionTime() + 10000 >= (uint64_t)NFGetTime()) return;

		pClient->SetLastActionTime(NFGetTime());
	}
}

void NFCNetClientModule::OnHandleNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			switch (nEvent)
			{
			case eMsgType_CONNECTED:
				{
					OnConnected(pClient);
				}
				break;
			case eMsgType_DISCONNECTED:
				{
					OnDisConnected(pClient);
				}
				break;
			default:
				break;
			}
		}
	}
}

void NFCNetClientModule::OnConnected(NFIClient* pClient)
{
	if (pClient)
	{
		pClient->SetStatus(eConnectStatus_ConnectOk);
	}
}

void NFCNetClientModule::OnDisConnected(NFIClient* pClient)
{
	if (pClient)
	{
		if (pClient->GetStatus() != eConnectStatus_REMOVE)
		{
			pClient->SetStatus(eConnectStatus_Disconnect);
			pClient->SetLastActionTime(NFGetTime());
		}
	}
}

void NFCNetClientModule::RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg)
{
	if (m_pLuaScriptModule)
	{
		m_pLuaScriptModule->RunNetRecvLuaFunc(luaFunc, unLinkId, valueId, nMsgId, strMsg);
	}
}

void NFCNetClientModule::RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId)
{
	if (m_pLuaScriptModule)
	{
		m_pLuaScriptModule->RunNetEventLuaFunc(luaFunc, nEvent, unLinkId);
	}
}

