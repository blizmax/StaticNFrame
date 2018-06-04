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
#include "NFMyClient.h"

NFCNetClientModule::NFCNetClientModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mxServerMap.resize(NF_ST_MAX);
	for(int serverType = NF_ST_NONE+1; serverType < NF_ST_MAX; serverType++)
	{
		AddEventCallBack((NF_SERVER_TYPES)serverType, this, &NFCNetClientModule::OnHandleNetEvent);
	}
	mxSendBuffer.AssureSpace(MAX_SEND_BUFFER_SIZE);
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
				mxServerMap[i][j]->Finalize();
				NFSafeDelete(mxServerMap[i][j]);
			}
		}
	}
	mxServerMap.clear();
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

uint32_t NFCNetClientModule::AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, const int nPort)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		uint32_t usId = GetFreeUnLinkId(eServerType);
		if (usId == 0)
		{
			NFLogError("Add Connecting Server Failed! Ip:%s, Port:%d, than max connection:65535", strIp.c_str(), nPort);
			return 0;
		}

		uint32_t index = GetServerIndexFromUnlinkId(usId);

		NFClientFlag flag;
		flag.strIP = strIp;
		flag.nPort = nPort;
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
			NFLogError("Add Connecting Server Failed! Ip:%s, Port:%d, than max connection:65535", strIp.c_str(), nPort);
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
						NFSafeDelete(pClient);
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
				NFSafeDelete(mxServerMap[i][j]);
				mxServerMap[i][j] = nullptr;
			}
		}
	}
}

void NFCNetClientModule::KeepState(NFClient* pClient)
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
		NFClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			switch(nEvent)
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

void NFCNetClientModule::OnConnected(NFClient* pClient)
{
	if (pClient)
	{
		pClient->SetStatus(eConnectStatus_ConnectOk);
	}
}

void NFCNetClientModule::OnDisConnected(NFClient* pClient)
{
	if (pClient)
	{
		pClient->SetStatus(eConnectStatus_Disconnect);
		pClient->SetLastActionTime(NFGetTime());
	}
}
