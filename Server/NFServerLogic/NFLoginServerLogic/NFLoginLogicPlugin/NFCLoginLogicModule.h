// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFComm/NFPluginModule/NFEventObj.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>

#include "NFServerLogic/NFServerLogicCommon/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include <NFComm/NFPluginModule/NFIHttpServerModule.h>
#include "NFServer/NFServerCommon/NFILoginClient_MasterModule.h"

#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFMessageDefine/NFMsgDefine.h"

class NFCLoginLogicModule : public NFILoginLogicModule
{
public:
	explicit NFCLoginLogicModule(NFIPluginManager* p);
	virtual ~NFCLoginLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
private:
	NFIHttpServerModule* m_pHttpServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFILoginClient_MasterModule* m_pLoginClient_MasterModule;
	NFIKernelModule* m_pKernelModule;
};