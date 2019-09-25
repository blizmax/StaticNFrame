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
		const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp);

	virtual bool PerformPost(const std::string& strUri, const std::string& strPostData,
		const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp);
private:
	NFIHttpClient* m_pHttpClient;
	std::map<std::string, std::string> m_xDefaultHttpHeaders;
};
