CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(NFGameLogicPlugin)

AUX_SOURCE_DIRECTORY(. SRC)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/Product/Lib/Release")
	ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRC} )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/Product/Lib/Debug")
	ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRC} )
elseif (CMAKE_BUILD_TYPE STREQUAL "DynamicRelease")
	SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/Product/run/Dynamic_Release")
	ADD_LIBRARY( ${PROJECT_NAME} SHARED ${SRC} )
elseif(CMAKE_BUILD_TYPE STREQUAL "DynamicDebug")
	SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/Product/run/Dynamic_Debug")
	ADD_LIBRARY( ${PROJECT_NAME} SHARED ${SRC} )
	#TARGET_LINK_LIBRARIES(${PROJECT_NAME} NFServerLogicCommon NFServerCommon  NFPluginModule NFCore NFMessageDefine Common  LuaBind rt dl  bson-1.0 event_core event_extra event crypto mongoc-1.0 mysqlclient Sqlite3 mysqlpp ssl protobuf)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} NFServerLogicCommon NFServerCommon)
endif()


