// -------------------------------------------------------------------------
//    @FileName         :    NFHttpServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include <map>
#include <list>

class NFCHttpServerModule
	: public NFIHttpServerModule
{

public:
	NFCHttpServerModule(NFIPluginManager* p);

	virtual ~NFCHttpServerModule();

public:
	virtual bool Awake();
	virtual bool Init();
	virtual bool AfterInit();
	virtual bool Execute();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Finalize();

	virtual int InitServer(const unsigned short nPort);

	virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK,
			const std::string& reason = "OK");
private:
	virtual bool OnReceiveNetPack(const NFHttpRequest& req);
	virtual NFWebStatus OnFilterPack(const NFHttpRequest& req);

	virtual bool AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb);
	virtual bool AddFilterCB(const std::string& strCommand, const HTTP_FILTER_FUNCTOR& cb);

	virtual bool LuaAddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const std::string& luaFunc);
private:
	NFIHttpServer* m_pHttpServer;
	std::map<NFHttpType, map<std::string, HTTP_RECEIVE_FUNCTOR>> mMsgCBMap;
	std::map<NFHttpType, map<std::string, std::string>> mMsgLuaCBMap;
	std::map<std::string, HTTP_FILTER_FUNCTOR> mMsgFliterMap;
	NFILuaScriptModule* m_pLuaScriptModule;
};