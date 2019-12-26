CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

add_compile_options(-std=c++11)

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -std=c++11 -O3 -Wall")
	add_definitions(
        -DNDEBUG
        -DNF_STATIC_PLUGIN
		-DLUA_COMPAT_5_1
        -fPIC
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -Wall -g -ggdb -fPIC")
	add_definitions(
        -D_DEBUG
        -DNF_STATIC_PLUGIN
		-DLUA_COMPAT_5_1
        -fPIC
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "DynamicDebug")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -Wall -g -ggdb -fPIC")
	add_definitions(
        -D_DEBUG
		-DLUA_COMPAT_5_1
        -fPIC
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "DynimicRelease")
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -std=c++11 -O3 -Wall")
	add_definitions(
        -DNDEBUG
		-DLUA_COMPAT_5_1
        -fPIC
    )
endif()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

SET(INCLUDE_DIR "./" "/usr/local/include"
	"${CMAKE_SOURCE_DIR}/Server"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/LuaBind/lua"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/LuaBind/"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/g3dlite"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/libevent/include/"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/libevent/linux/include/"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/Theron/Include"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/protobuf/src"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/navigation"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mysql/include"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mysql/linux"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mysqlpp/lib"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mongo/src/libmongoc/src/mongoc"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mongo/src/libmongoc/src"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mongo/src/libbson/src"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mongo/src/libbson/src/bson"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/mongo/linux"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/openssl/include"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/openssl/linux/include"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/common/sigar/linux"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/common/"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/curl/include"
	"${CMAKE_SOURCE_DIR}/Server/StaticNFrame/NFComm/NFNetPlugin/"
	"${CMAKE_SOURCE_DIR}/Server/StaticNFrame/")


if (CMAKE_BUILD_TYPE STREQUAL "Release")
		SET(LIB_DIR "/usr/lib"
			"/usr/local/lib"
			"/usr/lib64"
			"/usr/local/lib64"
			"${CMAKE_SOURCE_DIR}/Product/run/Release"
			"${CMAKE_SOURCE_DIR}/Product/Lib/Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(LIB_DIR "/usr/lib"
		"/usr/local/lib"
		"/usr/lib64"
		"/usr/local/lib64"
		"${CMAKE_SOURCE_DIR}/Product/run/Debug"
		"${CMAKE_SOURCE_DIR}/Product/Lib/Debug")
elseif (CMAKE_BUILD_TYPE STREQUAL "DynamicRelease")
		SET(LIB_DIR "/usr/lib"
			"/usr/local/lib"
			"/usr/lib64"
			"/usr/local/lib64"
			"${CMAKE_SOURCE_DIR}/Product/run/Dynamic_Release"
			"${CMAKE_SOURCE_DIR}/Product/Lib/Dynamic_Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "DynamicDebug")
	SET(LIB_DIR "/usr/lib"
		"/usr/local/lib"
		"/usr/lib64"
		"/usr/local/lib64"
		"${CMAKE_SOURCE_DIR}/Product/run/Dynamic_Debug"
		"${CMAKE_SOURCE_DIR}/Product/Lib/Dynamic_Debug")
endif()

SET(LIB_LINK_DEP resolv dl rt libprotobuf.a  libcurl.a libssl.a libcrypto.a pthread libmysqlpp.a libmysqlclient.a libevent_core.a libevent_extra.a libtheron.a
		libevent.a libLuaBind.a libtheron.a  libCommon.a libNFMessageDefine.a libjemalloc.a)
		
SET(LIB_LINK_NF  libNFCore.a libNFPluginModule.a libprotobuf.a libNFActorPlugin.a libNFKernelPlugin.a libNFLuaScriptPlugin.a libNFLuaThreadScriptPlugin.a
				libNFMath.a  libNFNetPlugin.a libNFMonitorPlugin.a libNFNoSqlPlugin.a
			    libNFMysqlPlugin.a libNFMasterServerPlugin.a libNFCommonClient_MasterPlugin.a libNFServerCommon.a
				libNFGameServerPlugin.a libNFLoginServerPlugin.a 
				libNFProxyServerPlugin.a libNFLocationServerPlugin.a libNFLogServerPlugin.a libNFWebServerPlugin.a libNFWorldServerPlugin.a
				libNFGameLogicPlugin.a libNFLoginLogicPlugin.a libNFProxyLogicPlugin.a 
				libNFServerLogicCommon.a libNFTestPlugin.a libNFServerCheckPlugin.a libNFWorldLogicPlugin.a libNFRebotPlugin.a)
	
include_directories( ${INCLUDE_DIR} )
	

link_directories( ${LIB_DIR} )


ADD_SUBDIRECTORY("Server/Dependencies")
ADD_SUBDIRECTORY("Server/NFMessageDefine")
ADD_SUBDIRECTORY("Server/StaticNFrame/NFComm")
ADD_SUBDIRECTORY("Server/StaticNFrame/NFTest")
ADD_SUBDIRECTORY("Server/StaticNFrame/NFRebot")
ADD_SUBDIRECTORY("Server/StaticNFrame/NFServerCheck")
ADD_SUBDIRECTORY("Server/NFServer")
ADD_SUBDIRECTORY("Server/NFServerLogic")
ADD_SUBDIRECTORY("Server/StaticNFrame/NFPluginLoader")
