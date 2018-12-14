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
#include "NFComm/NFPluginModule/NFServerDefine.h"

class NFIHttpServerModule
	: public NFIModule
{
public:
	virtual ~NFIHttpServerModule() {};

	// register msg callback
	template<typename BaseType>
	bool AddRequestHandler(NF_SERVER_TYPES serverType, const std::string& strPath, const NFHttpType eRequestType, BaseType* pBase, bool (BaseType::*handleRecieve)(uint32_t, const NFHttpRequest& req))
	{
		HTTP_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		return AddMsgCB(serverType, strPath, eRequestType, functor);
	}

	template<typename BaseType>
	bool AddNetFilter(NF_SERVER_TYPES serverType, const std::string& strPath, BaseType* pBase, NFWebStatus(BaseType::*handleFilter)(uint32_t, const NFHttpRequest& req))
	{
		HTTP_FILTER_FUNCTOR functor = std::bind(handleFilter, pBase, std::placeholders::_1, std::placeholders::_2);

		return AddFilterCB(serverType, strPath, functor);
	}

	bool LuaAddRequestHandler(NF_SERVER_TYPES serverType, const std::string& strPath, const NFHttpType eRequestType, const std::string& luaFunc)
	{
		return LuaAddMsgCB(serverType, strPath, eRequestType, luaFunc);
	}
public:
	virtual int InitServer(NF_SERVER_TYPES serverType, uint32_t nPort) = 0;

	virtual bool ResponseMsg(NF_SERVER_TYPES serverType, const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK, const std::string& reason = "OK") = 0;

private:
	virtual bool AddMsgCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb) = 0;
	virtual bool AddFilterCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const HTTP_FILTER_FUNCTOR& cb) = 0;

	virtual bool LuaAddMsgCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const NFHttpType eRequestType, const std::string& luaFunc) = 0;
};