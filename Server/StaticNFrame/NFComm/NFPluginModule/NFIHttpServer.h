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
#include "NFComm/NFCore/NFSlice.hpp"

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

class NFCHttpServer;

class NFHttpHandle
{
public:
	NFHttpHandle()
	{
		Reset();
	}

	void Reset()
	{
		requestId = 0;
		url.clear();
		path.clear();
		remoteHost.clear();
		httpServer = nullptr;
		timeOut = 0;
	}

	std::string get_url() { return url; }
	std::string get_path() { return path; }
	std::string get_remoteHost() { return remoteHost; }
	int get_type() { return type; }
	std::string get_body() { return bodySlice.ToString(); }

	void* req;
	std::string url;
	std::string path;
	std::string remoteHost;
	NFHttpType type;
	NFSlice bodySlice;
	uint64_t requestId;
	NFCHttpServer* httpServer;
	uint64_t timeOut;
};

//it should be
typedef std::function<bool(uint32_t linkId, const NFHttpHandle& req)> HTTP_RECEIVE_FUNCTOR;
typedef std::function<NFWebStatus(uint32_t linkId, const NFHttpHandle& req)> HTTP_FILTER_FUNCTOR;

class NFIHttpServer
{
public:
	virtual ~NFIHttpServer()
	{

	}

	virtual bool Execute() = 0;

	virtual uint32_t GetLinkId() const = 0;

	virtual int InitServer(uint32_t nPort) = 0;
	virtual int InitServer(std::vector<uint32_t> nPorts) = 0;
	virtual int InitServer(const std::string& portStr) = 0;

	virtual bool ResponseMsg(const NFHttpHandle& req, const std::string& strMsg, NFWebStatus code,
		const std::string& strReason = "OK") = 0;

	virtual bool ResponseMsg(uint64_t reqeustId, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") = 0;
};

