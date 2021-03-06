CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(NFPluginLoader)

AUX_SOURCE_DIRECTORY(. SRC)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} rt dl ${LIB_LINK_DEP} ${LIB_LINK_NF})

#TARGET_LINK_LIBRARIES(${PROJECT_NAME} -static-libstdc++)

ADD_DEPENDENCIES(${PROJECT_NAME} NFServerLogicCommon NFServerCommon NFPluginModule  LuaBind NFCore  NFMessageDefine  Common NFActorPlugin NFKernelPlugin
				NFLuaScriptPlugin NFLuaThreadScriptPlugin NFNetPlugin NFMonitorPlugin NFNoSqlPlugin NFMysqlPlugin
				NFGameClientPlugin NFGameServerPlugin NFLoginClientPlugin NFLoginServerPlugin NFMasterServerPlugin NFProxyClientPlugin NFProxyServerPlugin
				NFWorldClientPlugin NFWorldServerPlugin NFGameLogicPlugin NFLoginLogicPlugin NFProxyLogicPlugin NFTestPlugin NFWorldLogicPlugin NFRebotPlugin)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} 
		PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Product/run/Release"
	)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} 
		PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Product/run/Debug"
	)
elseif (CMAKE_BUILD_TYPE STREQUAL "DynamicRelease")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} 
		PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Product/run/Dynamic_Release"
	)
elseif(CMAKE_BUILD_TYPE STREQUAL "DynamicDebug")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} 
		PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Product/run/Dynamic_Debug"
	)
endif()




