CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY("NFMasterServer/NFMasterServerPlugin")
ADD_SUBDIRECTORY("NFMasterServer/NFCommonClient_MasterPlugin")
ADD_SUBDIRECTORY("NFServerCommon")

ADD_SUBDIRECTORY("NFGameServer/NFGameServerPlugin")

ADD_SUBDIRECTORY("NFLoginServer/NFLoginServerPlugin")

ADD_SUBDIRECTORY("NFProxyServer/NFProxyServerPlugin")


ADD_SUBDIRECTORY("NFWorldServer/NFWorldServerPlugin")

ADD_SUBDIRECTORY("NFLocationServer/NFLocationServerPlugin")

ADD_SUBDIRECTORY("NFLogServer/NFLogServerPlugin")

ADD_SUBDIRECTORY("NFWebServer/NFWebServerPlugin")


