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

	virtual bool PerformGet(const std::string& strUri,
		const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp) = 0;

	virtual bool PerformPost(const std::string& strUri, const std::string& strPostData,
		const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp) = 0;
};
