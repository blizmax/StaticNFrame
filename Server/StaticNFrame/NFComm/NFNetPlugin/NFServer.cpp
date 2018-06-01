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
	NFServer* pServer = (NFServer*)user_data;

	if (pServer->GetNetObjectCount() >= (uint32_t)pServer->GetMaxConnectNum())
	{
		NFLogError("connected count >= mnMaxConnect:%d! Can't add connect", pServer->GetMaxConnectNum());
		return;
	}

	struct event_base* base = pServer->GetEventBase();

	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		NFLogError("Error constructing bufferevent!");
		return;
	}


	struct sockaddr_in* pSin = (sockaddr_in*)sa;

	NetObject* pObject = new NetObject();


	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, (void*)pObject);


	bufferevent_enable(bev, EV_READ | EV_WRITE);


	conn_eventcb(bev, BEV_EVENT_CONNECTED, (void*)pObject);
}

NFServer::NFServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag): mBase(nullptr), mListener(nullptr), mFlag(flag), mServerType(serverType), mServerId(serverId), mNetObjectCount(0)
{
	assert(serverType > NF_ST_NONE && serverType < NF_ST_MAX);
}

NFServer::~NFServer()
{
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

	mListener = evconnlistener_new_bind(mBase, listener_cb, (void*)this,
	                                    LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
	                                    (struct sockaddr*)&sin,
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
	return true;
}

uint32_t NFServer::GetFreeUnLinkId()
{
	if (mNetObjectArray.empty())
	{
		return GetUnLinkId(mServerType, 0);
	}
	else
	{
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
	return 0;
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
