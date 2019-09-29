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

class NFCHttpClient : public NFIHttpClient
{
public:
	NFCHttpClient(int nRetry = 2, int nTimeoutSec = 1)
		: m_nRetry(nRetry), m_nTimeOut(nTimeoutSec)
	{
	}

	virtual ~NFCHttpClient()
	{};

public:
	virtual bool Execute();

	virtual bool Init();

	virtual bool Final();

	virtual bool PerformGet(const std::string& strUri,
		const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp);

	virtual bool PerformPost(const std::string& strUri, const std::string& strPostData, 
		const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp);
private:
	static void OnHttpReqDone(struct evhttp_request* req, void* ctx);

	bool MakeRequest(const std::string& strUri,
		const std::string& strPostData,
		const std::map<std::string, std::string>& xHeaders,
		const NFHttpType eHttpType, int& respCode, std::string& strResp);
private:
	std::string m_strResp;
	int m_respCode;
	struct event_base* m_pBase = nullptr;

	int m_nRetry = 2;
	int m_nTimeOut = 1; //

#if NF_ENABLE_SSL
	SSL_CTX*            m_pSslCtx = nullptr;
#endif
};
