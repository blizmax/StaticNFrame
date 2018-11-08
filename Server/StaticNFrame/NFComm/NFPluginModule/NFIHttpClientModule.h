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
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
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

	bool LuaHttpGet(const std::string& strUri, const std::string& luaFunc, const std::string& jsonHeaders = std::string(), const std::string& strUserData = std::string())
	{
		std::map<std::string, std::string> headers;

		if (jsonHeaders.empty() == false)
		{
			NFLogDebug("[HttpGet | {}]", jsonHeaders);
			rapidjson::Document jsonDoc;
			jsonDoc.Parse(jsonHeaders.c_str());
			if (jsonDoc.IsObject() == false)
			{
				NFLogError("[HttpGet Error");
				return false;
			}

			for (auto it = jsonDoc.MemberBegin(); it != jsonDoc.MemberEnd(); it++)
			{
				if (it->name.IsString() && it->value.IsString())
				{
					std::string name = it->name.GetString();
					std::string value = it->value.GetString();
					headers.emplace(name, value);
				}
			}
		}

		return LuaPerformGet(strUri, headers, luaFunc, strUserData);
	}

	bool LuaHttpPost(const std::string& strUri, const std::string& strPostData, const std::string& luaFunc, const std::string& jsonHeaders = std::string(), const std::string& strUserData = std::string())
	{
		std::map<std::string, std::string> headers;

		if (jsonHeaders.empty() == false)
		{
			NFLogDebug("[HttpPos | {}]", jsonHeaders);
			rapidjson::Document jsonDoc;
			jsonDoc.Parse(jsonHeaders.c_str());
			if (jsonDoc.IsObject() == false)
			{
				NFLogError("[HttpPos Error");
				return false;
			}

			for (auto it = jsonDoc.MemberBegin(); it != jsonDoc.MemberEnd(); it++)
			{
				if (it->value.IsString())
				{
					std::string name = it->name.GetString();
					std::string value = it->value.GetString();
					headers.emplace(name, value);
				}
			}
		}

		return LuaPerformPost(strUri, headers, strPostData, luaFunc, strUserData);
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

	virtual bool LuaPerformGet(const std::string& strUrl,
		const std::map<std::string, std::string>& xHeaders,
		const std::string& luaFunc,
		const std::string& strUserData) = 0;

	virtual bool LuaPerformPost(const std::string& strUrl,
		const std::map<std::string, std::string>& xHeaders,
		const std::string& strPostData,
		const std::string& luaFunc,
		const std::string& strUserData) = 0;
};
