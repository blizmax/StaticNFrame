// -------------------------------------------------------------------------
//    @FileName         :    NFCWebServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCWebServerModule
//
// -------------------------------------------------------------------------

#include "NFWebServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFMessageDefine/db_server.pb.h"
#include "NFMessageDefine/msg_gm.pb.h"
#include "NFComm/NFCore/NFStringUtility.h"

NFCWebServerModule::NFCWebServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCWebServerModule::~NFCWebServerModule()
{
}

bool NFCWebServerModule::Init()
{
	FindModule<NFIHttpServerModule>()->AddRequestHandler(NF_ST_WEB, NFHttpType::NF_HTTP_REQ_POST, this, &NFCWebServerModule::HttpHandleHttpMsg);

	NFServerConfig* pServerConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_WEB);
	if (pServerConfig)
	{
		if (!m_pPluginManager->IsLoadAllServer())
		{
			if (pServerConfig->mServerType != NF_ST_WEB)
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
				exit(0);
			}
		}

		if (pServerConfig->mHttpPort > 0)
		{
			uint32_t unlinkId = (uint32_t)FindModule<NFIHttpServerModule>()->InitServer(NF_ST_WEB, pServerConfig->mHttpPort);
			if (unlinkId == 0)
			{
				NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "Web Server Open Http Port:{} Failed!", pServerConfig->mHttpPort);
				return false;
			}
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Web Server Open Http Port:{} Success!", pServerConfig->mHttpPort);
		}
	}

	return true;
}

bool NFCWebServerModule::AfterInit()
{
	return true;
}

bool NFCWebServerModule::Execute()
{
	return true;
}

bool NFCWebServerModule::BeforeShut()
{
	return true;
}

bool NFCWebServerModule::Shut()
{
	return true;
}

bool NFCWebServerModule::HttpHandleHttpMsg(uint32_t linkId, const NFIHttpHandle& req)
{
	std::string jsonMsg = req.GetBody();

	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		std::vector<std::string> vecPath;
		NFStringUtility::Split(req.GetPath(), "/", &vecPath);
		if (vecPath.size() >= 2)
		{
			pLuaScriptModule->RunHttpRecvLuaFunc("LuaNFrame.DispatchWebHttp", 0, req.GetRequestId(), vecPath[0], vecPath[1], jsonMsg);
		}
	}
	return true;
}