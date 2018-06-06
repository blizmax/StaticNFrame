// -------------------------------------------------------------------------
//    @FileName         :    NFCLoadStaticPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------
#include "NFComm/NFPluginModule/NFCPluginManager.h"

#include "NFComm/NFCore/NFServerTimeMgr.h"

#include "NFComm/NFActorPlugin/NFActorPlugin.h"
#include "NFComm/NFKernelPlugin/NFKernelPlugin.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include "NFComm/NFPluginModule/NFDBActorMgr.h"

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFITimerModule.h"
#include "NFComm/NFNetPlugin/NFNetPlugin.h"
#include "NFTest/NFTestPlugin/NFTestPlugin.h"
#include <NFComm/NFMysqlPlugin/NFMysqlPlugin.h>

bool NFCPluginManager::InitSingleton()
{
	NFServerTimeMgr::Instance()->Init(mFrame);
	NFConfigMgr::Instance()->Init(this);

	NFILogModule* pLogModule = NFIPluginManager::FindModule<NFILogModule>();
	NFIEventModule* pEventModule = NFIPluginManager::FindModule<NFIEventModule>();
	NFITimerModule* pTimerModule = NFIPluginManager::FindModule<NFITimerModule>();

	//初始化事件系统
	NFEventMgr::Instance()->Init(pEventModule);
	NFTimerMgr::Instance()->Init(pTimerModule);
	NFLogMgr::Instance()->Init(pLogModule);

	//初始化DB系统
	NFDBActorMgr::Instance()->Init();
	return true;
}

bool NFCPluginManager::ReleaseSingletion()
{
	//初始化DB系统
	NFDBActorMgr::Instance()->UnInit();
	NFDBActorMgr::Instance()->ReleaseInstance();

	//释放时间系统
	NFServerTimeMgr::Instance()->UnInit();
	NFServerTimeMgr::Instance()->ReleaseInstance();

	//系统配置
	NFConfigMgr::Instance()->UnInit();
	NFConfigMgr::Instance()->ReleaseInstance();

	//释放定时器
	NFTimerMgr::Instance()->UnInit();
	NFTimerMgr::Instance()->ReleaseInstance();

	//事件系统
	NFEventMgr::Instance()->UnInit();
	NFEventMgr::Instance()->ReleaseInstance();

	//释放LOG
	NFLogMgr::Instance()->UnInit();
	NFLogMgr::Instance()->ReleaseInstance();

	return true;
}

bool NFCPluginManager::LoadKernelPlugin()
{
	mPluginNameMap.insert(PluginNameMap::value_type("NFKernelPlugin", true));
	LoadStaticPlugin("NFKernelPlugin");

	return true;
}

bool NFCPluginManager::RegisterStaticPlugin()
{
	REGISTER_STATIC_PLUGIN(this, NFKernelPlugin);
	REGISTER_STATIC_PLUGIN(this, NFActorPlugin);
	REGISTER_STATIC_PLUGIN(this, NFNetPlugin);
	REGISTER_STATIC_PLUGIN(this, NFMysqlPlugin);
	REGISTER_STATIC_PLUGIN(this, NFTestPlugin);
	return true;
}