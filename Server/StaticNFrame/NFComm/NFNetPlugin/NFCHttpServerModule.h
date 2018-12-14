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

	virtual int InitServer(NF_SERVER_TYPES serverType, uint32_t nPort);

	virtual bool ResponseMsg(NF_SERVER_TYPES serverType, const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK,
			const std::string& reason = "OK");
private:
	virtual bool OnReceiveNetPack(uint32_t unlinkId, const NFHttpRequest& req);
	virtual NFWebStatus OnFilterPack(uint32_t unlinkId, const NFHttpRequest& req);

	virtual bool AddMsgCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb);
	virtual bool AddFilterCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const HTTP_FILTER_FUNCTOR& cb);

	virtual bool LuaAddMsgCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const NFHttpType eRequestType, const std::string& luaFunc);
private:
	struct CallBack
	{
		std::map<NFHttpType, map<std::string, HTTP_RECEIVE_FUNCTOR>> mMsgCBMap;
		std::map<NFHttpType, map<std::string, std::string>> mMsgLuaCBMap;
		std::map<std::string, HTTP_FILTER_FUNCTOR> mMsgFliterMap;
	};

	std::vector<CallBack> mxCallBack;
	std::vector<NFIHttpServer*> mServerArray;
	NFILuaScriptModule* m_pLuaScriptModule;
};