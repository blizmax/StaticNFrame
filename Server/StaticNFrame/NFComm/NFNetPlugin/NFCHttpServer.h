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
	NFCHttpServer(uint32_t unLinkId, BaseType* pBaseType, bool (BaseType::*handleRecieve)(uint32_t,const NFHttpRequest& req), NFWebStatus(BaseType::*handleFilter)(uint32_t, const NFHttpRequest& req))
	{
		base = NULL;
		mReceiveCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2);
		mFilter = std::bind(handleFilter, pBaseType, std::placeholders::_1, std::placeholders::_2);
		mPort = 0;
		mUnLinkId = unLinkId;
	}

	virtual ~NFCHttpServer();

	virtual uint32_t GetLinkId() const;

	virtual bool Execute();

	virtual int InitServer(uint32_t nPort);

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");
private:
	static void listener_cb(struct evhttp_request* req, void* arg);
private:
	uint32_t mPort;
	struct event_base* base;
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;
	uint32_t mUnLinkId;
};