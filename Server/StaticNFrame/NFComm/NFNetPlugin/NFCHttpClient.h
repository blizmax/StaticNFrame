// -------------------------------------------------------------------------
//    @FileName         :    NFHttpClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIHttpClient.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIHttpServer.h"
#include "NFLibEvent.h"


#include <list>

class HttpObject
{
public:
	HttpObject(NFIHttpClient* pNet, struct bufferevent* pBev, const std::string& strUserData, const HTTP_RESP_FUNCTOR& pCB)
	{
		m_pBev = pBev;
		m_pHttpClient = pNet;
		m_pCB = pCB;
		m_strUserData = strUserData;
	}

	virtual ~HttpObject()
	{
	}

	bufferevent* m_pBev;
	NFIHttpClient* m_pHttpClient;
	HTTP_RESP_FUNCTOR m_pCB;
	std::string m_strUserData;
};

class LuaHttpObject
{
public:
	LuaHttpObject(NFIHttpClient* pNet, struct bufferevent* pBev, const std::string& strUserData, const std::string& pCB, NFILuaScriptModule* pLuaScriptModule = nullptr)
	{
		m_pBev = pBev;
		m_pHttpClient = pNet;
		m_luaFunc = pCB;
		m_strUserData = strUserData;
		m_pLuaScriptModule = pLuaScriptModule;
	}

	virtual ~LuaHttpObject()
	{
	}

	virtual void RunLuaFunc(const int state_code, const std::string& strRespData, const std::string& strUserData)
	{
		if (m_pLuaScriptModule)
		{
			m_pLuaScriptModule->RunHtttpClientLuaFunc(m_luaFunc, state_code, strRespData, strUserData);
		}
	}

	bufferevent* m_pBev;
	NFIHttpClient* m_pHttpClient;
	std::string m_luaFunc;
	std::string m_strUserData;
	NFILuaScriptModule* m_pLuaScriptModule;
};

class NFCHttpClient : public NFIHttpClient
{
public:
	NFCHttpClient(int nRetry = 2, int nTimeoutSec = 30)
		: m_nRetry(nRetry), m_nTimeOut(nTimeoutSec), m_pLuaScriptModule(nullptr)
	{
	}

	virtual ~NFCHttpClient()
	{};

public:
	virtual bool Execute();

	virtual bool Init();

	virtual bool Final();

	virtual bool PerformGet(const std::string& strUri, const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders);

	virtual bool PerformPost(const std::string& strUri, const std::string& strPostData, const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders);

	virtual bool LuaPerformGet(const std::string& strUri, const std::string& luaFunc,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders);

	virtual bool LuaPerformPost(const std::string& strUri, const std::string& strPostData, const std::string& luaFunc,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders);

	virtual void SetLuaScriptModule(NFILuaScriptModule* pLuaScriptModule) { m_pLuaScriptModule = pLuaScriptModule; }
private:
	static void OnHttpReqDone(struct evhttp_request* req, void* ctx);
	static void LuaOnHttpReqDone(struct evhttp_request* req, void* ctx);

	bool MakeRequest(const std::string& strUri,
		const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData,
		const std::string& strPostData,
		const std::map<std::string, std::string>& xHeaders,
		const NFHttpType eHttpType);

	bool LuaMakeRequest(const std::string& strUri,
		const std::string& luaFunc,
		const std::string& strUserData,
		const std::string& strPostData,
		const std::map<std::string, std::string>& xHeaders,
		const NFHttpType eHttpType);
private:
	std::string m_strUserAgent;
	struct event_base* m_pBase = nullptr;

	int m_nRetry = 2;
	int m_nTimeOut = 30;

	NFILuaScriptModule* m_pLuaScriptModule;

#if NF_ENABLE_SSL
	SSL_CTX*            m_pSslCtx = nullptr;
#endif
};
