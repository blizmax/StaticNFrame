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
	WEB_ERROR = 404,
	WEB_TIMEOUT = 503,
};

class NFHttpRequest
{
public:
	void* req;
	int nCommand;
	std::string url;
};

class NFHttpHeader
{

};

class NFHttpBody
{

};

//it should be
//typedef std::function<void(const NFHttpRequest& req, const int nCommand, const std::string& strData, const std::string& strUrl)> HTTP_RECEIVE_FUNCTOR;
typedef std::function < void(const NFHttpRequest& req, const std::string& strCommand,
	const std::string& strUrl) > HTTP_RECEIVE_FUNCTOR;

class NFIHttpServer
{
public:
	virtual bool Execute() = 0;

	virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool Final() = 0;

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
		const std::string& strReason = "OK") = 0;

	virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName) = 0;

};

