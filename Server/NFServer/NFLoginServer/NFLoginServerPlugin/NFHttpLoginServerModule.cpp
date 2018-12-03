// -------------------------------------------------------------------------
//    @FileName         :    NFCLoginServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLoginServerModule
//
// -------------------------------------------------------------------------

#include "NFHttpLoginServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCHttpLoginServerModule::NFCHttpLoginServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCHttpLoginServerModule::~NFCHttpLoginServerModule()
{
}

bool NFCHttpLoginServerModule::Init()
{
	m_pHttpServerModule = pPluginManager->FindModule<NFIHttpServerModule>();
	m_pHttpServerModule->AddRequestHandler("/httplogin", NFHttpType::NF_HTTP_REQ_POST, this, &NFCHttpLoginServerModule::HttpHandleHttpLogin);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_LOGIN);
	if (pConfig)
	{
		if (pConfig->mHttpPort == 0) return true;

		int ret = m_pHttpServerModule->InitServer(pConfig->mHttpPort);
		if (ret != 0)
		{
			NFLogError("Login Server Open Http Port:{} Failed!", pConfig->mHttpPort);
			return false;
		}
		NFLogInfo("Login Server Open Http Port:{} Success!", pConfig->mHttpPort);
	}
	else
	{
		NFLogError("I Can't get the Login Server config!");
		return false;
	}

	return true;
}

bool NFCHttpLoginServerModule::AfterInit()
{
	return true;
}

bool NFCHttpLoginServerModule::Execute()
{
	return true;
}

bool NFCHttpLoginServerModule::BeforeShut()
{
	return true;
}

bool NFCHttpLoginServerModule::Shut()
{
	return true;
}

bool NFCHttpLoginServerModule::HttpHandleHttpLogin(const NFHttpRequest& req)
{
	NFLogDebug("http login | body | {}", req.body);
	return true;
}
