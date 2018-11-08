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
class NFILogModule;

class NFCHttpServerModule
	: public NFIHttpServerModule
{

public:
	NFCHttpServerModule(NFIPluginManager* p);

	virtual ~NFCHttpServerModule();

public:
	virtual bool Init();
	virtual bool AfterInit();
	virtual bool Execute();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Finalize();

	virtual int InitServer(const unsigned short nPort);

	virtual bool AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR& cb);

	virtual bool AddComMsgCB(const HTTP_RECEIVE_FUNCTOR& cb);

	virtual bool ResponseMsg(const NFHttpRequest& req, const int nCommand, const std::string& strMsg);

	virtual bool
		ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code = NFWebStatus::WEB_OK,
			const std::string& reason = "OK");

	virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName);

private:
	void OnReceiveNetPack(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl);

private:
	NFILogModule* mLogModule;
	NFIHttpServer* m_pHttpServer;

	std::map<std::string, HTTP_RECEIVE_FUNCTOR> mMsgCBMap;
	std::list<HTTP_RECEIVE_FUNCTOR> mComMsgCBList;
};