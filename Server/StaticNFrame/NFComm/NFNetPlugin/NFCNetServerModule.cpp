// -------------------------------------------------------------------------
//    @FileName         :    NFCNetServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2017-10-16
//    @Module           :    NFCNetServerModule
//
// -------------------------------------------------------------------------


#include "NFCNetServerModule.h"

#include "NFComm/NFPluginModule/NFIPlugin.h"

NFCNetServerModule::NFCNetServerModule(NFIPluginManager* p)
{
    pPluginManager = p;
	mServerArray.resize(NF_ST_MAX);
    for (int i = 0; i < NF_SERVER_TYPES::NF_ST_MAX; ++i)
    {
		mServerArray[i] = nullptr;
    }
}

NFCNetServerModule::~NFCNetServerModule()
{
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
			NFSafeDelete(mServerArray[i]);
		}
	}
    return true;
}

bool NFCNetServerModule::Execute()
{
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
		NFSafeDelete(pServer);
	}
	return 0;
}

void NFCNetServerModule::CloseServer(const uint32_t nServerID)
{
}

void NFCNetServerModule::CloseServerByServerType(NF_SERVER_TYPES eServerType)
{
}

void NFCNetServerModule::CloseAllServer()
{
}

void NFCNetServerModule::SendByServerID(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendByServerID(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToAllServer(NF_SERVER_TYPES eServerType, const std::string& strData, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToAllServer(NF_SERVER_TYPES eServerType, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToAllServerByPB(NF_SERVER_TYPES eServerType, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
}

void NFCNetServerModule::SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
}
