set(ProjectName "NFLuaScriptPlugin")
file(GLOB NFLuaScriptPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFLuaScriptPlugin_ROOT_Hpp 
	*.h)

add_library(NFLuaScriptPlugin SHARED
	${NFLuaScriptPlugin_ROOT_Cpp}
	${NFLuaScriptPlugin_ROOT_Hpp})
	
set_target_properties( NFLuaScriptPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLuaScriptPlugin_d")
set_target_properties( NFLuaScriptPlugin PROPERTIES PREFIX "")
set_target_properties( NFLuaScriptPlugin PROPERTIES 
	FOLDER "NFMidWare/NFLuaScriptPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
target_link_libraries(NFLuaScriptPlugin liblua.a)
link_NFSDK("NFLuaScriptPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
