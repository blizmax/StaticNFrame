// -------------------------------------------------------------------------
//    @FileName         :    NFHttpClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIHttpClient.h"
#include "NFIHttpServer.h"

#include <list>

//it would be a pool
class HttpObject
{
public:
	HttpObject(NFIHttpClient* pNet, struct bufferevent* pBev, const HTTP_RESP_FUNCTOR& pCB, uint64_t id)
	{
		m_pBev = pBev;
		m_pHttpClient = pNet;
		m_pCB = pCB;

		mID = id;
	}

	virtual ~HttpObject()
	{
	}

	uint64_t mID;
	bufferevent* m_pBev;
	NFIHttpClient* m_pHttpClient;
	HTTP_RESP_FUNCTOR m_pCB;
};

class NFCHttpClient : public NFIHttpClient
{
public:
	NFCHttpClient(int nRetry = 2, int nTimeoutSec = 30)
		: m_nRetry(nRetry), m_nTimeOut(nTimeoutSec)
	{
	}

	virtual ~NFCHttpClient()
	{};

public:
	virtual bool Execute();

	virtual bool Init();

	virtual bool Final();

	virtual bool DoGet(const std::string& strUri, const HTTP_RESP_FUNCTOR& pCB,
		const std::map<std::string, std::string>& xHeaders, const uint64_t id = 0);

	virtual bool DoPost(const std::string& strUri, const std::string& strPostData, const HTTP_RESP_FUNCTOR& pCB,
		const std::map<std::string, std::string>& xHeaders, const uint64_t id = 0);

private:
	static void OnHttpReqDone(struct evhttp_request* req, void* ctx);

	bool MakeRequest(const std::string& strUri,
		const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strPostData,
		const std::map<std::string, std::string>& xHeaders,
		const NFHttpType eHttpType,
		const uint64_t id =0);

private:
	std::string m_strUserAgent;
	struct event_base* m_pBase = nullptr;

	int m_nRetry = 2;
	int m_nTimeOut = 30;

	std::list<HttpObject*> mlHttpObject;

#if NF_ENABLE_SSL
	SSL_CTX *			m_pSslCtx = nullptr;
#endif
};
