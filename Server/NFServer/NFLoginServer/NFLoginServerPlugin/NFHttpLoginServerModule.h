// -------------------------------------------------------------------------
//    @FileName         :    NFCLoginServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLoginServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIHttpLoginServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFIHttpServerModule.h>

class NFCHttpLoginServerModule : public NFIHttpLoginServerModule
{
public:
	explicit NFCHttpLoginServerModule(NFIPluginManager* p);
	virtual ~NFCHttpLoginServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool HttpHandleHttpLogin(const NFHttpRequest& req);
private:
	NFIHttpServerModule* m_pHttpServerModule;
};

