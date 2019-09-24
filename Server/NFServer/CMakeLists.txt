CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY("NFGameServer/NFGameClientPlugin")
ADD_SUBDIRECTORY("NFGameServer/NFGameServerPlugin")


ADD_SUBDIRECTORY("NFLoginServer/NFLoginClientPlugin")
ADD_SUBDIRECTORY("NFLoginServer/NFLoginServerPlugin")


ADD_SUBDIRECTORY("NFMasterServer/NFMasterServerPlugin")


ADD_SUBDIRECTORY("NFProxyServer/NFProxyClientPlugin")
ADD_SUBDIRECTORY("NFProxyServer/NFProxyServerPlugin")


ADD_SUBDIRECTORY("NFServerCommon")

ADD_SUBDIRECTORY("NFWorldServer/NFWorldClientPlugin")
ADD_SUBDIRECTORY("NFWorldServer/NFWorldServerPlugin")

ADD_SUBDIRECTORY("NFLocationServer/NFLocationClientPlugin")
ADD_SUBDIRECTORY("NFLocationServer/NFLocationServerPlugin")

ADD_SUBDIRECTORY("NFLogServer/NFLogClientPlugin")
ADD_SUBDIRECTORY("NFLogServer/NFLogServerPlugin")

ADD_SUBDIRECTORY("NFWebServer/NFWebClientPlugin")
ADD_SUBDIRECTORY("NFWebServer/NFWebServerPlugin")


