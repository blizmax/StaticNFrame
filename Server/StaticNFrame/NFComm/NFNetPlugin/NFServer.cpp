// -------------------------------------------------------------------------
//    @FileName         :    NFServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFServer.h"
#include "NFLibEvent.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include <NFComm/NFPluginModule/NFINetModule.h>
#include "NetObject.h"

void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data)
{
	NFServer* pServer = static_cast<NFServer*>(user_data);

	bool ret = pServer->AddNetObject(fd, sa);
	if (ret == false)
	{
		NFLogError("pServer AddNetObject Failed!");
		return;
	}
}

NFServer::NFServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag): mBase(nullptr), mListener(nullptr), mFlag(flag), mServerType(serverType), mServerId(serverId), mNetObjectCount(0)
{
	assert(serverType > NF_ST_NONE && serverType < NF_ST_MAX);
}

NFServer::~NFServer()
{
}

bool NFServer::AddNetObject(SOCKET fd, sockaddr* sa)
{
	if (GetNetObjectCount() >= GetMaxConnectNum())
	{
		NFLogError("connected count >= mnMaxConnect:%d! Can't add connect", GetMaxConnectNum());
		return false;
	}

	uint32_t usLinkId = GetFreeUnLinkId();
	if (usLinkId == 0)
	{
		NFLogError("connected count >= mnMaxConnect:%d! Can't add connect", GetMaxConnectNum());
		return false;
	}

	uint32_t index = GetServerIndexFromUnlinkId(usLinkId);
	if (index >= mNetObjectArray.size() || mNetObjectArray[index] == nullptr)
	{
		NFLogError("GetServerIndexFromUnLinkId Failed!");
		return false;
	}

	struct bufferevent* bev = bufferevent_socket_new(mBase, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		NFLogError("Error constructing bufferevent!");
		return false;
	}

	struct sockaddr_in* pSin = reinterpret_cast<sockaddr_in*>(sa);
	std::string ip = inet_ntoa(pSin->sin_addr);
	uint32_t	port = pSin->sin_port;

	NetObject* pObject = NF_NEW NetObject();
	mNetObjectArray[index] = pObject;
	mNetObjectCount++;

	pObject->SetLinkId(usLinkId);
	pObject->SetBev(bev);
	pObject->SetStrIp(ip);
	pObject->SetPort(port);

	pObject->SetRecvCB(this, &NFServer::OnReceiveNetPack);
	pObject->SetEventCB(this, &NFServer::OnSocketNetEvent);

	bufferevent_setcb(bev, NetObject::conn_recvcb, NetObject::conn_writecb, NetObject::conn_eventcb, static_cast<void*>(pObject));
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	NetObject::conn_eventcb(bev, BEV_EVENT_CONNECTED, static_cast<void*>(pObject));

	return true;
}

uint32_t NFServer::GetServerId() const
{
	return mServerId;
}

uint32_t NFServer::GetServerType() const
{
	return mServerType;
}

bool NFServer::Init()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	struct event_config* cfg = event_config_new();
#if NF_PLATFORM == NF_PLATFORM_WIN
	mBase = event_base_new_with_config(cfg);
#else
	//event_config_avoid_method(cfg, "epoll");
	if (event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST) < 0)
	{
		return -1;
	}

	if (event_config_set_num_cpus_hint(cfg, 2) < 0)
	{
		return -1;
	}

	mBase = event_base_new_with_config(cfg);//event_base_new()
#endif
	event_config_free(cfg);

	if (!mBase)
	{
		NFLogError("Could not initialize libevent!\n");
		return false;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET ;
	sin.sin_port = htons(mFlag.nPort);

	NFLogInfo("serverId:%d serverType:%s started with port:", mServerId, GetServerName(mServerType).c_str(), mFlag.nPort);

	mListener = evconnlistener_new_bind(mBase, listener_cb, static_cast<void*>(this),
	                                    LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
	                                    reinterpret_cast<struct sockaddr*>(&sin),
	                                    sizeof(sin));

	if (mListener)
	{
		NFLogError("Can not create a listener for port:%d", mFlag.nPort);
		return false;
	}

	return true;
}

bool NFServer::Shut()
{
	return true;
}

bool NFServer::Execute()
{
	ExecuteClose();

    if (mBase)
    {
        event_base_loop(mBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
    }
	return true;
}

uint32_t NFServer::GetFreeUnLinkId()
{
	if (mNetObjectArray.empty())
	{
		return GetUnLinkId(mServerType, 0);
	}

	size_t sz = mNetObjectArray.size();

	for (size_t index = 0; index < sz; index++)
	{
		if (mNetObjectArray[index] == nullptr)
		{
			return GetUnLinkId(mServerType, index);
		}
	}

	if (sz >= mFlag.nMaxConnectNum)
	{
		return 0;
	}

	mNetObjectArray.push_back(nullptr);

	return GetUnLinkId(mServerType, sz);
}

uint32_t NFServer::GetNetObjectCount() const
{
	return mNetObjectCount;
}

uint32_t NFServer::GetMaxConnectNum() const
{
	return mFlag.nMaxConnectNum;
}

event_base* NFServer::GetEventBase() const
{
	return mBase;
}

void NFServer::ExecuteClose()
{
	for (size_t i = 0; i < mvRemoveObject.size(); i++)
	{
		uint32_t unLinkId = mvRemoveObject[i];
		uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
		uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

		NF_ASSERT_MSG(serverType == mServerType, "the unlinkId is not of the server");
		if (serverIndex < mNetObjectArray.size())
		{
			auto pObject = mNetObjectArray[serverIndex];
			if (pObject && pObject->GetNeedRemove())
			{
				mNetObjectArray[serverIndex] = nullptr;
				NFSafeDelete(pObject);
				return;
			}
		}			
		NF_ASSERT_MSG(false, "the unlinkId is not right!");
	}

	mvRemoveObject.clear();
}

void NFServer::OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}

void NFServer::OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_DISCONNECTED)
	{
		uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
		uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

		NF_ASSERT_MSG(serverType == mServerType, "the unlinkId is not of the server");
		if (serverIndex < mNetObjectArray.size())
		{
			auto pObject = mNetObjectArray[serverIndex];
			if (pObject && pObject->GetNeedRemove())
			{
				mvRemoveObject.push_back(unLinkId);
				return;
			}
		}

		NF_ASSERT_MSG(false, "the unlinkId is not right!");
	}
}
