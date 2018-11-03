// -------------------------------------------------------------------------
//    @FileName         :    NFIHttpServer.h
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

enum NFWebStatus
{
	WEB_OK = 200,
	WEB_AUTH = 401,
	WEB_ERROR = 404,
	WEB_INTER_ERROR = 500,
	WEB_TIMEOUT = 503,
};

enum NFHttpType
{
	NF_HTTP_REQ_GET = 1 << 0,
	NF_HTTP_REQ_POST = 1 << 1,
	NF_HTTP_REQ_HEAD = 1 << 2,
	NF_HTTP_REQ_PUT = 1 << 3,
	NF_HTTP_REQ_DELETE = 1 << 4,
	NF_HTTP_REQ_OPTIONS = 1 << 5,
	NF_HTTP_REQ_TRACE = 1 << 6,
	NF_HTTP_REQ_CONNECT = 1 << 7,
	NF_HTTP_REQ_PATCH = 1 << 8
};

class NFHttpRequest
{
public:
	NFHttpRequest()
	{
		Reset();
	}

	void Reset()
	{
		id = 0;
		url.clear();
		path.clear();
		remoteHost.clear();
		//type
		body.clear();
		params.clear();
		headers.clear();
	}

	int64_t id;

	void* req;
	std::string url;
	std::string path;
	std::string remoteHost;
	NFHttpType type;
	std::string body;//when using post
	std::map<std::string, std::string> params;//when using get
	std::map<std::string, std::string> headers;
};

//it should be
typedef std::function<bool(const NFHttpRequest& req)> HTTP_RECEIVE_FUNCTOR;

typedef std::function<NFWebStatus(const NFHttpRequest& req)> HTTP_FILTER_FUNCTOR;

class NFIHttpServer
{
public:
	virtual bool Execute() = 0;

	virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") = 0;
};

