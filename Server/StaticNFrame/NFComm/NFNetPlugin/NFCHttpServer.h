// -------------------------------------------------------------------------
//    @FileName         :    NFHttpServer.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIHttpServer.h"
#include "NFLibEvent.h"

class NFCHttpServer : public NFIHttpServer
{
public:
	NFCHttpServer()
	{
	}

	template<typename BaseType>
	NFCHttpServer(BaseType* pBaseType, bool (BaseType::*handleRecieve)(const NFHttpRequest& req), NFWebStatus(BaseType::*handleFilter)(const NFHttpRequest& req))
	{
		base = NULL;
		mReceiveCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1);
		mFilter = std::bind(handleFilter, pBaseType, std::placeholders::_1);
		mPort = 0;
	}

	virtual ~NFCHttpServer();

	virtual bool Execute();

	virtual int InitServer(const unsigned short nPort);

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");
private:
	static void listener_cb(struct evhttp_request* req, void* arg);
private:
	int mPort;
	struct event_base* base;
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;
};