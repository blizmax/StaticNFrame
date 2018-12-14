// -------------------------------------------------------------------------
//    @FileName         :    NFIHttpServer.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <map>
#include <functional>

#include "NFComm/NFPluginModule/NFIHttpClientModule.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif

#else

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <atomic>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif

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
		url.clear();
		path.clear();
		remoteHost.clear();
		//type
		body.clear();
		params.clear();
		headers.clear();
	}

	std::string get_url() { return url; }
	void set_url(const std::string& _url) { url = _url; }
	std::string get_path() { return path; }
	void set_path(const std::string& _path) { path = _path; }
	std::string get_remoteHost() { return remoteHost; }
	void set_remoteHost(const std::string& _host) { remoteHost = _host; }
	int get_type() { return type; }
	void set_type(int t) { type = (NFHttpType)t; }
	std::string get_body() { return body; }
	void set_body(const std::string& b) { body = b; }

	std::string get_params() { return json_params; }
	void set_params(const std::string& p) { json_params = p; }
	std::string get_headers() { return json_headers; }
	void set_headers(const std::string& h) { json_headers = h; }

	void* req;
	std::string url;
	std::string path;
	std::string remoteHost;
	NFHttpType type;
	std::string body;//when using post
	std::map<std::string, std::string> params;//when using get
	std::map<std::string, std::string> headers;
	std::string json_params;
	std::string json_headers;
};

//it should be
typedef std::function<bool(uint32_t linkId, const NFHttpRequest& req)> HTTP_RECEIVE_FUNCTOR;
typedef std::function<NFWebStatus(uint32_t linkId, const NFHttpRequest& req)> HTTP_FILTER_FUNCTOR;

class NFIHttpServer
{
public:
	virtual bool Execute() = 0;

	virtual uint32_t GetLinkId() const = 0;

	virtual int InitServer(uint32_t nPort) = 0;

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
		const std::string& strReason = "OK") = 0;
};

