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

#ifndef NF_DYNAMIC_PLUGIN
//#pragma comment(lib, "ws2_32.lib")

#pragma comment(lib,"msvcrtd.lib")
//#pragma comment(lib, "libssl.lib")
//#pragma comment(lib, "libcrypto.lib")

#pragma comment( lib, "bson-1.0.lib" )
#pragma comment( lib, "mongoc-1.0.lib" )

#pragma comment( lib, "luaBind.lib" )
#pragma comment( lib, "event.lib" )
#pragma comment( lib, "event_core.lib" )
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "Theron.lib" )
#pragma comment( lib, "user32.lib")
#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "mysqlclient.lib" )
#pragma comment( lib, "mysqlpp.lib" )


/////////////////////////////////////////////////////////////////
#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "NFPluginModule.lib" )
#pragma comment( lib, "NFMessageDefine.lib" )
#pragma comment( lib, "NFMath.lib" )

#pragma comment( lib, "NFServerCommon.lib" )

////////////////////////////////////////////////////////////////
#pragma comment( lib, "NFKernelPlugin.lib" )
#pragma comment( lib, "NFActorPlugin.lib" )
#pragma comment( lib, "NFNetPlugin.lib" )
#pragma comment( lib, "NFMysqlPlugin.lib" )
#pragma comment( lib, "NFLuaScriptPlugin.lib" )
#pragma comment( lib, "NFTestPlugin.lib" )

/////////////////////////////////////////////////////

#pragma comment( lib, "NFMasterServerPlugin.lib" )
#pragma comment( lib, "NFLoginClientPlugin.lib" )
#pragma comment( lib, "NFLoginServerPlugin.lib" )
#pragma comment( lib, "NFWorldClientPlugin.lib" )
#pragma comment( lib, "NFWorldServerPlugin.lib" )

/////////////////////////////////////////////////////////////
#pragma comment( lib, "NFGameClientPlugin.lib" )
#pragma comment( lib, "NFGameServerPlugin.lib" )
#pragma comment( lib, "NFProxyServerPlugin.lib" )
#pragma comment( lib, "NFProxyClientPlugin.lib" )

#else
#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "NFPluginModule.lib" )

#pragma comment( lib, "user32.lib")
#pragma comment( lib, "Dbghelp.lib" )
#endif
#endif