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
#include <vector>

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

class NFIHttpHandle
{
public:
	NFIHttpHandle() = default;

	virtual ~NFIHttpHandle() = default;

	virtual void Reset() = 0;
	virtual const std::string& GetUrl() const = 0;
	virtual const std::string& GetPath() const = 0;
	virtual const std::string& GetRemoteHost() const = 0;
	virtual int GetType() const = 0;
	virtual std::string GetBody() const = 0;
	virtual uint64_t GetRequestId() const = 0;
	virtual uint64_t GetTimeOut() const = 0;

	virtual void AddResponseHeader(const std::string& key, const std::string& value) const = 0;
	virtual bool ResponseMsg(const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") const = 0;
};

//it should be
typedef std::function<bool(uint32_t serverType, const NFIHttpHandle& req)> HTTP_RECEIVE_FUNCTOR;
typedef std::function<NFWebStatus(uint32_t serverType, const NFIHttpHandle& req)> HTTP_FILTER_FUNCTOR;

class NFIHttpServer
{
public:
	virtual ~NFIHttpServer()
	{

	}

	virtual bool Execute() = 0;

	virtual uint32_t GetServerType() const = 0;

	virtual bool InitServer(uint32_t nPort) = 0;
	virtual bool InitServer(const std::vector<uint32_t>& listen_ports) = 0;
	virtual bool InitServer(const std::string& portStr) = 0;

	virtual bool ResponseMsg(const NFIHttpHandle& req, const std::string& strMsg, NFWebStatus code,
		const std::string& strReason = "OK") = 0;

	virtual bool ResponseMsg(uint64_t reqeustId, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") = 0;

	/**
	 *@brief  设置接收回调.
	 */
	template <typename BaseType>
	void SetRecvCB(BaseType* pBaseType, bool (BaseType::*handleRecieve)(uint32_t, const NFIHttpHandle& req))
	{
		mReceiveCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	 *@brief  设置连接事件回调.
	 */
	template <typename BaseType>
	void SetFilterCB(BaseType* pBaseType, NFWebStatus(BaseType::*handleFilter)(uint32_t, const NFIHttpHandle& req))
	{
		mFilter = std::bind(handleFilter, pBaseType, std::placeholders::_1, std::placeholders::_2);
	}

protected:
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;
};

