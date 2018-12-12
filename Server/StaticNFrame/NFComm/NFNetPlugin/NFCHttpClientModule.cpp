// -------------------------------------------------------------------------
//    @FileName         :    NFCHttpClientModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-11-07
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFCHttpClientModule.h"
#include "NFCHttpClient.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#define DEFAULT_USER_AGENT "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Mobile Safari/537.36"

NFCHttpClientModule::NFCHttpClientModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pHttpClient = new NFCHttpClient();
	m_xDefaultHttpHeaders =
	{
		{ "Connection",    "close" },
		{ "Content-Type",  "text/plain;text/html;application/x-www-form-urlencoded;charset=utf-8" },
		{ "User-Agent", DEFAULT_USER_AGENT },
		{ "Cache-Control", "no-cache" }
	};
	m_pLuaScriptModule = nullptr;
}

NFCHttpClientModule::~NFCHttpClientModule()
{
	delete m_pHttpClient;
	m_pHttpClient = NULL;
}

bool NFCHttpClientModule::Awake()
{
	//可以允许Lua Module不存在
	m_pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pPluginManager->FindModule(typeid(NFILuaScriptModule).name()));
	m_pHttpClient->Init();
	m_pHttpClient->SetLuaScriptModule(m_pLuaScriptModule);
	return true;
}

bool NFCHttpClientModule::Init()
{
	return true;
}

bool NFCHttpClientModule::Execute()
{
	m_pHttpClient->Execute();
	return true;
}

bool NFCHttpClientModule::Shut()
{
	m_pHttpClient->Final();

	return true;
}

bool NFCHttpClientModule::PerformGet(const std::string& strUri,
	const std::map<std::string, std::string>& xHeaders,
	const HTTP_RESP_FUNCTOR& pCB,
	const std::string& strUserData)
{
	return m_pHttpClient->PerformGet(strUri, pCB, strUserData, xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}

bool NFCHttpClientModule::PerformPost(const std::string& strUri,
	const std::map<std::string, std::string>& xHeaders,
	const std::string& strPostData,
	const HTTP_RESP_FUNCTOR& pCB,
	const std::string& strUserData)
{
	return m_pHttpClient->PerformPost(strUri, strPostData, pCB, strUserData,
		xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}

bool NFCHttpClientModule::LuaPerformGet(const std::string& strUri,
	const std::map<std::string, std::string>& xHeaders,
	const std::string& pCB,
	const std::string& strUserData)
{
	return m_pHttpClient->LuaPerformGet(strUri, pCB, strUserData, xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}

bool NFCHttpClientModule::LuaPerformPost(const std::string& strUri,
	const std::map<std::string, std::string>& xHeaders,
	const std::string& strPostData,
	const std::string& pCB,
	const std::string& strUserData)
{
	return m_pHttpClient->LuaPerformPost(strUri, strPostData, pCB, strUserData,
		xHeaders.size() == 0 ? m_xDefaultHttpHeaders : xHeaders);
}