// -------------------------------------------------------------------------
//    @FileName         :    NFIHttpServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/11/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFIHttpServer.h"

class NFIHttpServerModule
	: public NFIModule
{
public:
	virtual ~NFIHttpServerModule() {};

	// register msg callback
	template<typename BaseType>
	bool AddRequestHandler(const std::string& strPath, const NFHttpType eRequestType, BaseType* pBase, bool (BaseType::*handleRecieve)(const NFHttpRequest& req))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		return AddMsgCB(strPath, eRequestType, functor);
	}

	template<typename BaseType>
	bool AddNetFilter(const std::string& strPath, BaseType* pBase, NFWebStatus(BaseType::*handleFilter)(const NFHttpRequest& req))
	{
		HTTP_FILTER_FUNCTOR functor = std::bind(handleFilter, pBase, std::placeholders::_1);

		return AddFilterCB(strPath, functor);
	}

	bool LuaAddRequestHandler(const std::string& strPath, const NFHttpType eRequestType, const std::string& luaFunc)
	{
		return LuaAddMsgCB(strPath, eRequestType, luaFunc);
	}
public:
	virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK") = 0;

private:
	virtual bool AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb) = 0;
	virtual bool AddFilterCB(const std::string& strCommand, const HTTP_FILTER_FUNCTOR& cb) = 0;

	virtual bool LuaAddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const std::string& luaFunc) = 0;
};