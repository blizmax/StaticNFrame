// -------------------------------------------------------------------------
//    @FileName         :    NFCNetServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2017-10-16
//    @Module           :    NFCNetServerModule
//
// -------------------------------------------------------------------------

#include "NFCNetServerModule.h"

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFIPacketParse.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCNetServerModule::NFCNetServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mServerArray.resize(NF_ST_MAX);
	for (int i = 0; i < NF_SERVER_TYPES::NF_ST_MAX; ++i)
	{
		mServerArray[i] = nullptr;
	}
	mxSendBuffer.AssureSpace(MAX_SEND_BUFFER_SIZE);
	m_pLuaScriptModule = nullptr;
}

NFCNetServerModule::~NFCNetServerModule()
{
}

bool NFCNetServerModule::Awake()
{
	//可以允许Lua Module不存在
	m_pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pPluginManager->FindModule("NFILuaScriptModule"));
	return true;
}

bool NFCNetServerModule::Init()
{
	return true;
}

bool NFCNetServerModule::AfterInit()
{
	return true;
}

bool NFCNetServerModule::BeforeShut()
{
	return true;
}

bool NFCNetServerModule::Shut()
{
	for (size_t i = 0; i < mServerArray.size(); i++)
	{
		if (mServerArray[i] != nullptr)
		{
			mServerArray[i]->Shut();
		}
	}
	return true;
}

bool NFCNetServerModule::Finalize()
{
	for (size_t i = 0; i < mServerArray.size(); i++)
	{
		if (mServerArray[i] != nullptr)
		{
			mServerArray[i]->Finalize();
			NF_SAFE_DELETE(mServerArray[i]);
		}
	}
	return true;
}

bool NFCNetServerModule::Execute()
{
	for (size_t i = 0; i < mServerArray.size(); i++)
	{
		if (mServerArray[i] != nullptr)
		{
			mServerArray[i]->Execute();
		}
	}
	return true;
}

uint32_t NFCNetServerModule::AddServer(const NF_SERVER_TYPES eServerType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		if (mServerArray[eServerType] != nullptr)
		{
			NFLogError("the serverType:%s has existing! Add Server Failed!", GetServerName(eServerType).c_str());
			return 0;
		}

		NFServerFlag flag;
		flag.nPort = nPort;
		flag.nMaxConnectNum = nMaxClient;
		NFServer* pServer = NF_NEW NFServer(eServerType, nServerID, flag);
		pServer->SetRecvCB((NFINetModule*)this, &NFINetModule::OnReceiveNetPack);
		pServer->SetEventCB((NFINetModule*)this, &NFINetModule::OnSocketNetEvent);
		if (pServer->Init())
		{
			mServerArray[eServerType] = pServer;
			return eServerType;
		}

		NFLogError("Add Server Failed!");
		NF_SAFE_DELETE(pServer);
	}
	return 0;
}

std::string NFCNetServerModule::GetLinkIp(uint32_t usLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(usLinkId);
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		auto pServer = mServerArray[serverType];
		if (pServer)
		{
			return pServer->GetLinkIp(usLinkId);
		}
		else
		{
			return std::string();
		}
	}
	return std::string();
}

void NFCNetServerModule::SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
	SendByServerID(usLinkId, nMsgID, strData.c_str(), strData.length(), nPlayerID);
}

void NFCNetServerModule::SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(usLinkId);
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		auto pServer = mServerArray[serverType];
		if (pServer)
		{
			SendMsg(pServer, usLinkId, nMsgID, msg, nLen, nPlayerID);
			return;
		}
		else
		{
			assert(0);
		}
	}
	assert(0);
}

void NFCNetServerModule::SendToServerByPB(uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendByServerID(usLinkId, nMsgID, strData, nPlayerID);
}

void NFCNetServerModule::SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
	SendToAllServer(nMsgID, strData.c_str(), strData.length(), nPlayerID);
}

void NFCNetServerModule::SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	for (size_t serverType = 0; serverType < mServerArray.size(); serverType++)
	{
		auto pServer = mServerArray[serverType];
		if (pServer)
		{
			SendAllMsg(pServer, nMsgID, msg, nLen, nPlayerID);
		}
	}
}

void NFCNetServerModule::SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendToAllServer(nMsgID, strData, nPlayerID);
}

void NFCNetServerModule::SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
	SendToAllServer(eServerType, nMsgID, strData.c_str(), strData.length(), nPlayerID);
}

void NFCNetServerModule::SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		auto pServer = mServerArray[eServerType];
		if (pServer)
		{
			SendAllMsg(pServer, nMsgID, msg, nLen, nPlayerID);
		}
	}
}

void NFCNetServerModule::SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendToAllServer(eServerType, nMsgID, strData, nPlayerID);
}

void NFCNetServerModule::SendMsg(NFServer* pServer, uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	mxSendBuffer.Clear();
	NFIPacketParse::EnCode(nMsgID, nPlayerID, msg, nLen, mxSendBuffer);
	if (pServer)
	{
		pServer->Send(usLinkId, mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
	}
	mxSendBuffer.Clear();
}

void NFCNetServerModule::SendAllMsg(NFServer* pServer, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	mxSendBuffer.Clear();
	NFIPacketParse::EnCode(nMsgID, nPlayerID, msg, nLen, mxSendBuffer);
	if (pServer)
	{
		pServer->SendAll(mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
	}
	mxSendBuffer.Clear();
}

void NFCNetServerModule::RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg)
{
	if (m_pLuaScriptModule)
	{
		m_pLuaScriptModule->RunNetRecvLuaFunc(luaFunc, unLinkId, valueId, nMsgId, strMsg);
	}
}

void NFCNetServerModule::RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId)
{
	if (m_pLuaScriptModule)
	{
		m_pLuaScriptModule->RunNetEventLuaFunc(luaFunc, nEvent, unLinkId);
	}
}

