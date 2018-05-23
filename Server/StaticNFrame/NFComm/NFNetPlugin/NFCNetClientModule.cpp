#include "NFCNetClientModule.h"
#include <iostream>
#include "NFIPacketParse.h"

#include "event2/thread.h"

NFCNetClientModule::NFCNetClientModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mxServerMap.resize(NF_ST_MAX);
	mxSendBuffer.AssureSpace(MAX_SEND_BUFFER_SIZE);
#ifdef WIN32
	WSADATA wsaData;
	int nResult = WSAStartup(0x0201, &wsaData);
	if (nResult)
	{
		std::cout << "WSAStartup failed with error code:" << nResult << std::endl;
		return;
	}
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif
}

NFCNetClientModule::~NFCNetClientModule()
{
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

	NFSLEEP(2000);
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
				NFSafeDelete(mxServerMap[i][j]);
			}
		}
	}
	mxServerMap.clear();
	return true;
}

bool NFCNetClientModule::Execute()
{
	ProcessExecute();
	return true;
}

uint32_t NFCNetClientModule::GetFreeUnLinkId(NF_SERVER_TYPES eServerType)
{
	if (eServerType >= NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		if (mxServerMap[eServerType].empty())
		{
			return GetUnLinkId(eServerType, 0);
		}
		else
		{
			size_t sz = mxServerMap[eServerType].size();
			if (sz >= MAX_SERVER_TYPE_CLIENT_COUNT)
			{
				return 0;
			}

			for (size_t index = 0; index < sz; index++)
			{
				if (mxServerMap[eServerType][index] == nullptr)
				{
					return GetUnLinkId(eServerType, index);
				}
			}
			return GetUnLinkId(eServerType, sz);
		}
	}
	return 0;
}

uint32_t NFCNetClientModule::AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, const int nPort, bool useThread)
{
	if (eServerType >= NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		uint32_t usId = GetFreeUnLinkId(eServerType);
		uint32_t index = GetServerIndexFromUnlinkId(usId);

		stClientFlag flag;
		flag.strIP = strIp;
		flag.nPort = nPort;

		NFClient* pClient = NF_NEW NFClient(usId, flag);
		pClient->SetRecvCB(this, &NFCNetClientModule::OnReceiveNetPack);
		pClient->SetEventCB(this, &NFCNetClientModule::OnSocketNetEvent);
		if (pClient->Init())
		{
			if (index == mxServerMap[eServerType].size())
			{
				mxServerMap[eServerType].push_back(pClient);
				return pClient->GetLinkId();
			}
			else if (index < mxServerMap[eServerType].size())
			{
				mxServerMap[eServerType][index] = pClient;
				return pClient->GetLinkId();
			}
		}
		NFSafeDelete(pClient);
	}
	return 0;
}

void NFCNetClientModule::CloseServer(const uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			pClient->Shut();
			NFSafeDelete(pClient);
			mxServerMap[serverType][serverIndex] = nullptr;
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
				NFSafeDelete(mxServerMap[eServerType][j]);
				mxServerMap[eServerType][j] = nullptr;
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
				NFSafeDelete(mxServerMap[i][j]);
				mxServerMap[i][j] = nullptr;
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
		NFClient* pClient = mxServerMap[serverType][serverIndex];
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

void NFCNetClientModule::SendMsg(NFClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	mxSendBuffer.Clear();
	NFIPacketParse::EnCode(nMsgID, nPlayerID, msg, nLen, mxSendBuffer);
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
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->Execute();
			}
		}
	}
}

void NFCNetClientModule::OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	OnReceiveBaseNetPack(unLinkId, playerId, nMsgId, msg, nLen);
}

void NFCNetClientModule::OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	int nRet(0);
	if (nEvent == eMsgType_CONNECTED)
	{
		nRet = OnConnected(nEvent, unLinkId);
	}
	else
	{
		nRet = OnDisConnected(nEvent, unLinkId);
	}

	OnSocketBaseNetEvent(nEvent, unLinkId);
}

int NFCNetClientModule::OnConnected(const eMsgType nEvent, const uint32_t unLinkId)
{
	return 0;
}

int NFCNetClientModule::OnDisConnected(const eMsgType nEvent, const uint32_t unLinkId)
{
	return 0;
}