// -------------------------------------------------------------------------
//    @FileName         :    NFIHttpClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <map>
#include <functional>

class NFILuaScriptModule;

typedef std::function <void(const int state_code, const std::string& strRespData,
	const std::string& strUserData) > HTTP_RESP_FUNCTOR;

class NFIHttpClient
{
public:
	virtual ~NFIHttpClient()
	{}

	virtual bool Execute() = 0;

	virtual bool Init() = 0;

	virtual bool Final() = 0;

	virtual bool PerformGet(const std::string& strUri, const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders) = 0;

	virtual bool PerformPost(const std::string& strUri, const std::string& strPostData, const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders) = 0;
};
