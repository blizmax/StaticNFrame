// -------------------------------------------------------------------------
//    @FileName         :    NFServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//
//                    _ooOoo_
//                   o8888888o
//                   88" . "88
//                   (| -_- |)
//                    O\ = /O
//                ____/`---'\____
//              .   ' \\| |// `.
//               / \\||| : |||// \
//             / _||||| -:- |||||- \
//               | | \\\ - /// | |
//            | \_| ''\---/'' | |
//              \ .-\__ `-` ___/-. /
//           ___`. .' /--.--\ `. . __
//        ."" '< `.___\_<|>_/___.' >'"".
//       | | : `- \`.;`\ _ /`;.`/ - ` : | |
//         \ \ `-. \_ __\ /__ _/ .-` / /
// ======`-.____`-.___\_____/___.-`____.-'======
//                    `=---='
//
// .............................................
//          ·ð×æ±£ÓÓ             ÓÀÎÞBUG
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
		evutil_closesocket(fd);
		NFLogError(NF_LOG_NET_PLUGIN, 0, "pServer AddNetObject Failed!");
		return;
	}
}

NFServer::NFServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag) : NFIServer(serverType, serverId, flag), mBase(nullptr), mListener(nullptr)
{

}

NFServer::~NFServer()
{
	for (auto iter = mNetObjectArray.begin(); iter != mNetObjectArray.end(); ++iter)
	{
		auto pObject = iter->second;
		if (pObject)
		{
			NF_SAFE_DELETE(pObject);
		}
	}
	mNetObjectArray.clear();
	if (mListener)
	{
		evconnlistener_free(mListener);
	}
	mListener = nullptr;

	if (mBase)
	{
		event_base_free(mBase);
	}
	mBase = nullptr;
}

bool NFServer::AddNetObject(SOCKET fd, sockaddr* sa)
{
	if (GetNetObjectCount() >= GetMaxConnectNum())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "connected count >= mnMaxConnect:{}! Can't add connect", GetMaxConnectNum());
		return false;
	}

	uint32_t usLinkId = GetFreeUnLinkId();
	if (usLinkId == 0)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "connected count >= mnMaxConnect:{}! Can't add connect", GetMaxConnectNum());
		return false;
	}

	if (mNetObjectArray.find(usLinkId) != mNetObjectArray.end())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "GetServerIndexFromUnLinkId Failed!");
		return nullptr;
	}

	struct bufferevent* bev = bufferevent_socket_new(mBase, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Error constructing bufferevent!");
		return false;
	}

	struct sockaddr_in* pSin = reinterpret_cast<sockaddr_in*>(sa);
	std::string ip = inet_ntoa(pSin->sin_addr);
	uint32_t port = pSin->sin_port;

	NetObject* pObject = NF_NEW NetObject();
	mNetObjectArray.emplace(usLinkId, pObject);
	mNetObjectCount++;

	pObject->SetWebSocket(mWebSocket);
	pObject->SetLinkId(usLinkId);
	pObject->SetBev(bev);
	pObject->SetStrIp(ip);
	pObject->SetPort(port);
	pObject->SetPacketParseType(mPacketParseType);

	pObject->SetRecvCB(this, &NFServer::OnReceiveNetPack);
	pObject->SetEventCB(this, &NFServer::OnSocketNetEvent);

	bufferevent_setcb(bev, NetObject::conn_recvcb, NetObject::conn_writecb, NetObject::conn_eventcb, static_cast<void*>(pObject));
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	NetObject::conn_eventcb(bev, BEV_EVENT_CONNECTED, static_cast<void*>(pObject));

	return true;
}

void NFServer::CloseLinkId(uint32_t usLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(usLinkId);

	if (serverType != mServerType)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "serverType != mServerType, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return;
	}

	uint32_t isServer = GetIsServerFromUnlinkId(usLinkId);
	if (isServer != NF_IS_SERVER)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a server link, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return;
	}

	auto iter = mNetObjectArray.find(usLinkId);
	if (iter != mNetObjectArray.end())
	{
		auto pObject = iter->second;
		if (pObject)
		{
			pObject->SetNeedRemove(true);
			pObject->CloseObject();
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the usLinkId:{} is nullptr", usLinkId);
		}
	}
	return;
}

bool NFServer::Send(uint32_t usLinkId, const void* pData, uint32_t unSize)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(usLinkId);

	if (serverType != mServerType)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "serverType != mServerType, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return false;
	}

	uint32_t isServer = GetIsServerFromUnlinkId(usLinkId);
	if (isServer != NF_IS_SERVER)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a server link, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return false;
	}

	auto iter = mNetObjectArray.find(usLinkId);
	if (iter != mNetObjectArray.end())
	{
		auto pObject = iter->second;
		if (pObject)
		{
			return pObject->Send(pData, unSize);
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the usLinkId:{} is nullptr", usLinkId);
		}
	}
	return false;
}

std::string NFServer::GetLinkIp(uint32_t usLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(usLinkId);

	if (serverType != mServerType)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "serverType != mServerType, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return std::string();
	}

	uint32_t isServer = GetIsServerFromUnlinkId(usLinkId);
	if (isServer != NF_IS_SERVER)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a server link, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return std::string();
	}

	auto iter = mNetObjectArray.find(usLinkId);
	if (iter != mNetObjectArray.end())
	{
		auto pObject = iter->second;
		if (pObject)
		{
			return pObject->GetStrIp();
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the usLinkId:%d is nullptr", usLinkId);
		}
	}
	return std::string();
}

bool NFServer::SendAll(const void* pData, uint32_t unSize)
{
	for (auto iter = mNetObjectArray.begin(); iter != mNetObjectArray.end(); ++iter)
	{
		auto pObject = iter->second;
		if (pObject)
		{
			return pObject->Send(pData, unSize);
		}
	}
	return true;
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
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Could not initialize libevent!\n");
		return false;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET ;
	sin.sin_port = htons(mFlag.nPort);

	//NFLogInfo("serverId:{} serverType:{} started with port:{}", mServerId, GetServerName(mServerType).c_str(), mFlag.nPort);

	mListener = evconnlistener_new_bind(mBase, listener_cb, static_cast<void*>(this),
	                                    LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
	                                    reinterpret_cast<struct sockaddr*>(&sin),
	                                    sizeof(sin));

	if (!mListener)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Can not create a listener for port:{}", mFlag.nPort);
		return false;
	}

	return true;
}

bool NFServer::Shut()
{
	for (auto iter = mNetObjectArray.begin(); iter != mNetObjectArray.end(); ++iter)
	{
		auto pObject = iter->second;
		if (pObject)
		{
			pObject->SetNeedRemove(true);
			pObject->CloseObject();
		}
	}
	return true;
}

bool NFServer::Finalize()
{
	for (auto iter = mNetObjectArray.begin(); iter != mNetObjectArray.end(); ++iter)
	{
		auto pObject = iter->second;
		if (pObject)
		{
			mNetObjectCount--;
			NF_SAFE_DELETE(pObject);
		}
	}

	mNetObjectArray.clear();
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
	uint32_t loopCount = 0;
	while (loopCount < MAX_CLIENT_MASK)
	{
		loopCount++;
		mNetObjectMaxIndex++;
		if (mNetObjectMaxIndex >= MAX_CLIENT_MASK)
		{
			mNetObjectMaxIndex = 0;
		}

		uint32_t unlinkId = GetUnLinkId(NF_IS_SERVER, mServerType, mNetObjectMaxIndex);

		if (mNetObjectArray.find(unlinkId) == mNetObjectArray.end())
		{
			return unlinkId;
		}
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "GetFreeUnLinkId failed!");
	return 0;
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
		uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);

		NF_ASSERT_MSG(isServer == NF_IS_SERVER, "the unlinkId is not a server");
		NF_ASSERT_MSG(serverType == mServerType, "the unlinkId is not of the server");
		auto iter = mNetObjectArray.find(unLinkId);
		if (iter != mNetObjectArray.end())
		{
			auto pObject = iter->second;
			if (pObject && pObject->GetNeedRemove())
			{
				mNetObjectArray.erase(iter);
				mNetObjectCount--;
				NF_SAFE_DELETE(pObject);
			}
			else
			{
				NF_ASSERT_MSG(false, "the NetObject of the unlinkId is not remove!");
			}
		}
		else
		{
			NF_ASSERT_MSG(false, "the unlinkId's index is not right!");
		}
	}

	mvRemoveObject.clear();
}

void NFServer::OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (mRecvCB)
	{
		mRecvCB(unLinkId, playerId, nMsgId, msg, nLen);
	}
}

void NFServer::OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_DISCONNECTED)
	{
		uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
		uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);

		NF_ASSERT_MSG(isServer == NF_IS_SERVER, "the unlinkId is not a server");
		NF_ASSERT_MSG(serverType == mServerType, "the unlinkId is not of the server");
		auto iter = mNetObjectArray.find(unLinkId);
		if (iter != mNetObjectArray.end())
		{
			auto pObject = iter->second;
			if (pObject && pObject->GetNeedRemove())
			{
				mvRemoveObject.push_back(unLinkId);
			}
			else
			{
				NF_ASSERT_MSG(pObject && pObject->GetNeedRemove(), "the pObject is nullptr or is not remove!");
			}
		}
		else
		{
			NF_ASSERT_MSG(iter != mNetObjectArray.end(), "the unlinkId is not right!");
		}
	}

	if (mEventCB)
	{
		mEventCB(nEvent, unLinkId);
	}
}

