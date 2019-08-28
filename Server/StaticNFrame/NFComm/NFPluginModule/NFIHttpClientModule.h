// -------------------------------------------------------------------------
//    @FileName         :    NFIHttpClientModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/11/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIModule.h"
#include "common/rapidjson/rapidjson.h"
#include "common/rapidjson/document.h"
#include <functional>
#include <map>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIHttpClient.h"
#include "NFComm/NFCore/NFPlatform.h"

class NFIHttpClientModule
	: public NFIModule
{
public:
	virtual ~NFIHttpClientModule() {};

	template<typename BaseType>
	bool PerformGet(const std::string& strUri,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR pd = HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return PerformGet(strUri, std::map<std::string, std::string>(), pd, std::string());
	}

	template<typename BaseType>
	bool PerformGet(const std::string& strUri, const std::string& strUserData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR pd = HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return PerformGet(strUri, std::map<std::string, std::string>(), pd, strUserData);
	}

	template<typename BaseType>
	bool PerformGet(const std::string& strUri, const std::string& strUserData, const std::map<std::string, std::string>& xHeaders,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR pd = HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return PerformGet(strUri, xHeaders, pd, strUserData);
	}

	template<typename BaseType>
	bool PerformPost(const std::string& strUri, const std::string& strPostData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR pd = HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return PerformPost(strUri, std::map<std::string, std::string>(), strPostData, pd, std::string());
	}


	template<typename BaseType>
	bool PerformPost(const std::string& strUri, const std::string& strPostData, const std::string& strUserData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR pd = HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return PerformPost(strUri, std::map<std::string, std::string>(), strPostData, pd, strUserData);
	}

	template<typename BaseType>
	bool PerformPost(const std::string& strUri, const std::string& strPostData, const std::string& strUserData, const std::map<std::string, std::string>& xHeaders,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR pd = HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return PerformPost(strUri, xHeaders, strPostData, pd, strUserData);
	}
protected:
	virtual bool PerformGet(const std::string& strUrl,
		const std::map<std::string, std::string>& xHeaders,
		const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData) = 0;

	virtual bool PerformPost(const std::string& strUrl,
		const std::map<std::string, std::string>& xHeaders,
		const std::string& strPostData,
		const HTTP_RESP_FUNCTOR& pCB,
		const std::string& strUserData) = 0;
};
