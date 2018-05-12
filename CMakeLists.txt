CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

add_compile_options(-std=c++11)

if( CMAKE_BUILD_TYPE MATCHES "Release" )
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -std=c++11 -O3 -Wall -DNDEBUG")
else()
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -Wall -g -ggdb -D_DEBUG -fPIC")
endif()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

SET(INCLUDE_DIR "./" "/usr/local/include"
	"${CMAKE_SOURCE_DIR}/Server"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/lua"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/g3dlite"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/libevent/include/event2"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/libevent/include/"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/libevent/compat/"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/Theron/Include"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/protobuf/src"
	"${CMAKE_SOURCE_DIR}/Server/Dependencies/navigation"
	"${CMAKE_SOURCE_DIR}/Server/StaticNFrame/")


if (CMAKE_BUILD_TYPE MATCHES "Release")
		SET(LIB_DIR "/usr/lib"
			"/usr/local/lib"
			"/usr/lib64"
			"/usr/local/lib64"
			"${CMAKE_SOURCE_DIR}/Product/Lib/Release")
else()
	SET(LIB_DIR "/usr/lib"
		"/usr/local/lib"
		"/usr/lib64"
		"/usr/local/lib64"
		"${CMAKE_SOURCE_DIR}/Product/Lib/Debug")
endif()

SET(LIB_LINK_DEP libevent_core.a libevent_extra.a libtheron.a
		libevent.a liblua.a libprofiler.a libtcmalloc_minimal.a libtheron.a libevent_pthreads.a protobuf pthread dl rt)
		
SET(LIB_LINK_NF libCommon.a libG3Dlite.a libNavigation.a libNFActorPlugin.a libNFCore.a libNFKernelPlugin.a
				libNFMath.a libNFMessageDefine.a libNFNetPlugin.a libNFPluginModule.a libNFTestPlugin.a)
	
include_directories( ${INCLUDE_DIR} )
	
link_directories( ${LIB_DIR} )

PROJECT(Dependencies)
ADD_SUBDIRECTORY("Server/Dependencies")

PROJECT(NFMessageDefine)
ADD_SUBDIRECTORY("Server/NFMessageDefine")

PROJECT(NFComm)
ADD_SUBDIRECTORY("Server/StaticNFrame/NFComm")

PROJECT(NFTest)
ADD_SUBDIRECTORY("Server/StaticNFrame/NFTest")

PROJECT(NFPluginLoader)
ADD_SUBDIRECTORY("Server/StaticNFrame/NFPluginLoader")