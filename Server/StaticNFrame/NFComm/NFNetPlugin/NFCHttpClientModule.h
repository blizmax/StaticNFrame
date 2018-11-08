// -------------------------------------------------------------------------
//    @FileName         :    NFCHttpClientModule.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-07
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIHttpClientModule.h"

class NFIKernelModule;

class NFCHttpClientModule
	: public NFIHttpClientModule
{

public:
	NFCHttpClientModule(NFIPluginManager* p);

	virtual ~NFCHttpClientModule();

	virtual bool Awake();

	virtual bool Init();

	virtual bool Execute();

	virtual bool Shut();
protected:
	virtual bool PerformGet(const std::string& strUri,
		const std::map<std::string, std::string>& xHeaders,
		const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData);

	virtual bool PerformPost(const std::string& strUri,
		const std::map<std::string, std::string>& xHeaders,
		const std::string& strPostData,
		const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData);

	virtual bool LuaPerformGet(const std::string& strUri,
		const std::map<std::string, std::string>& xHeaders,
		const std::string& luaFunc,
		const std::string& strUserData);

	virtual bool LuaPerformPost(const std::string& strUri,
		const std::map<std::string, std::string>& xHeaders,
		const std::string& strPostData,
		const std::string& luaFunc,
		const std::string& strUserData);
private:
	NFIHttpClient* m_pHttpClient;
	std::map<std::string, std::string> m_xDefaultHttpHeaders;
	NFILuaScriptModule* m_pLuaScriptModule;
};
