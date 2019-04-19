// -------------------------------------------------------------------------
//    @FileName         :    NFCLoadStaticPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------
#include "NFCPluginManager.h"
#include "NFComm/NFCore/NFPlatform.h"

#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFITimerModule.h"
#include "NFComm/NFPluginModule/NFIConfigModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"

#ifndef NF_DYNAMIC_PLUGIN
#include "NFComm/NFActorPlugin/NFActorPlugin.h"
#include "NFComm/NFKernelPlugin/NFKernelPlugin.h"

#include "NFComm/NFNetPlugin/NFNetPlugin.h"
#include "NFTest/NFTestPlugin/NFTestPlugin.h"
#include "NFComm/NFMysqlPlugin/NFMysqlPlugin.h"
#include "NFComm/NFMongoPlugin/NFMongoPlugin.h"
#include "NFComm/NFMonitorPlugin/NFMonitorPlugin.h"
#include "NFComm/NFSqlitePlugin/NFSqlitePlugin.h"
#include "NFComm/NFNoSqlPlugin/NFNoSqlPlugin.h"

#include <NFComm/NFXPlugin/NFLuaScriptPlugin/NFLuaScriptPlugin.h>

#include <NFServer/NFMasterServer/NFMasterServerPlugin/NFMasterServerPlugin.h>
#include <NFServer/NFLoginServer/NFLoginClientPlugin/NFLoginClientPlugin.h>
#include <NFServer/NFLoginServer/NFLoginServerPlugin/NFLoginServerPlugin.h>
#include <NFServer/NFWorldServer/NFWorldClientPlugin/NFWorldClientPlugin.h>
#include <NFServer/NFWorldServer/NFWorldServerPlugin/NFWorldServerPlugin.h>

#include <NFServer/NFGameServer/NFGameClientPlugin/NFGameClientPlugin.h>
#include <NFServer/NFGameServer/NFGameServerPlugin/NFGameServerPlugin.h>
#include <NFServer/NFProxyServer/NFProxyServerPlugin/NFProxyServerPlugin.h>
#include <NFServer/NFProxyServer/NFProxyClientPlugin/NFProxyClientPlugin.h>

#include <NFServerLogic/NFLoginServerLogic/NFLoginLogicPlugin/NFLoginLogicPlugin.h>
#include <NFServerLogic/NFGameServerLogic/NFGameLogicPlugin/NFGameLogicPlugin.h>
#include <NFServerLogic/NFProxyServerLogic/NFProxyLogicPlugin/NFProxyLogicPlugin.h>
#include <NFServerLogic/NFWorldLogicPlugin/NFWorldLogicPlugin.h>
#include <NFServerLogic/NFGameServerLogic/NFGameHumanPlugin/NFGameHumanPlugin.h>
#endif

bool NFCPluginManager::LoadKernelPlugin()
{
#ifndef NF_DYNAMIC_PLUGIN
	mPluginNameMap.insert(PluginNameMap::value_type("NFKernelPlugin", true));
	LoadStaticPlugin("NFKernelPlugin");
#else
	mPluginNameMap.insert(PluginNameMap::value_type("NFKernelPlugin", true));
	LoadPluginLibrary("NFKernelPlugin");
#endif
	return true;
}

bool NFCPluginManager::RegisterStaticPlugin()
{
#ifndef NF_DYNAMIC_PLUGIN
	REGISTER_STATIC_PLUGIN(this, NFKernelPlugin);
	REGISTER_STATIC_PLUGIN(this, NFActorPlugin);
	REGISTER_STATIC_PLUGIN(this, NFNetPlugin);
	REGISTER_STATIC_PLUGIN(this, NFMysqlPlugin);
	REGISTER_STATIC_PLUGIN(this, NFLuaScriptPlugin);
	REGISTER_STATIC_PLUGIN(this, NFTestPlugin);
	REGISTER_STATIC_PLUGIN(this, NFMongoPlugin);
	REGISTER_STATIC_PLUGIN(this, NFMonitorPlugin);
	REGISTER_STATIC_PLUGIN(this, NFSqlitePlugin);
	REGISTER_STATIC_PLUGIN(this, NFNoSqlPlugin);
	/////////////////////////////////////////////////////////
	REGISTER_STATIC_PLUGIN(this, NFMasterServerPlugin);

	REGISTER_STATIC_PLUGIN(this, NFLoginClientPlugin);
	REGISTER_STATIC_PLUGIN(this, NFLoginServerPlugin);

	REGISTER_STATIC_PLUGIN(this, NFWorldClientPlugin);
	REGISTER_STATIC_PLUGIN(this, NFWorldServerPlugin);

	REGISTER_STATIC_PLUGIN(this, NFGameClientPlugin);
	REGISTER_STATIC_PLUGIN(this, NFGameServerPlugin);

	REGISTER_STATIC_PLUGIN(this, NFProxyServerPlugin);
	REGISTER_STATIC_PLUGIN(this, NFProxyClientPlugin);
	///////////////////////////////////////////////////

	REGISTER_STATIC_PLUGIN(this, NFLoginLogicPlugin);
	REGISTER_STATIC_PLUGIN(this, NFGameLogicPlugin);
	REGISTER_STATIC_PLUGIN(this, NFProxyLogicPlugin);
	REGISTER_STATIC_PLUGIN(this, NFWorldLogicPlugin);

	////////////////////游戏服务器具体逻辑引擎实现////////////////////////////
	/////////////////////////////////////////////////////////////////////////

	REGISTER_STATIC_PLUGIN(this, NFGameHumanPlugin);

	////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
#endif
	return true;
}
