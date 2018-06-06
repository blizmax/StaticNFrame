// -------------------------------------------------------------------------
//    @FileName         :    dllmain.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------
#include "NFComm/NFCore/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "common.lib" )
#pragma comment( lib, "lua.lib" )
#pragma comment( lib, "navigation.lib" )
#pragma comment( lib, "libevent.lib" )
#pragma comment( lib, "libevent_core.lib" )
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "Theron.lib" )
#pragma comment( lib, "g3dlite.lib" )
#pragma comment( lib, "libtcmalloc_minimal.lib" )
#pragma comment( lib, "user32.lib")
#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "mysqlclient.lib" )
#pragma comment( lib, "mysqlpp.lib" )

/////////////////////////////////////////////////////////////////
#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "NFPluginModule.lib" )
#pragma comment( lib, "NFMessageDefine.lib" )
#pragma comment( lib, "NFMath.lib" )
////////////////////////////////////////////////////////////////
#pragma comment( lib, "NFKernelPlugin.lib" )
#pragma comment( lib, "NFActorPlugin.lib" )
#pragma comment( lib, "NFNetPlugin.lib" )
#pragma comment( lib, "NFMysqlPlugin.lib" )
#pragma comment( lib, "NFTestPlugin.lib" )
#endif