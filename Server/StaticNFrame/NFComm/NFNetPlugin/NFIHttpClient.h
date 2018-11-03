// -------------------------------------------------------------------------
//    @FileName         :    NFIHttpClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFLibEvent.h"

#include <map>
#include <functional>

typedef std::function<void(const uint64_t id, const int state_code, const std::string& strRespData)> HTTP_RESP_FUNCTOR;

class NFIHttpClient
{
public:
	virtual ~NFIHttpClient()
	{}

	virtual bool Execute() = 0;

	virtual bool Init() = 0;

	virtual bool Final() = 0;

	virtual bool DoGet(const std::string& strUri, const HTTP_RESP_FUNCTOR& pCB,
		const std::map<std::string, std::string>& xHeaders, const uint64_t id = 0) = 0;

	virtual bool DoPost(const std::string& strUri, const std::string& strPostData, const HTTP_RESP_FUNCTOR& pCB,
		const std::map<std::string, std::string>& xHeaders, const uint64_t id = 0) = 0;
};
