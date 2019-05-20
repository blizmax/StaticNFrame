CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY("NFGameServerLogic/NFGameLogicPlugin")
ADD_SUBDIRECTORY("NFGameServerLogic/NFGameHumanPlugin")
ADD_SUBDIRECTORY("NFGameServerLogic/NFGameLhdPlugin")

ADD_SUBDIRECTORY("NFLoginServerLogic/NFLoginLogicPlugin")

ADD_SUBDIRECTORY("NFProxyServerLogic/NFProxyLogicPlugin")
ADD_SUBDIRECTORY("NFWorldLogicPlugin")


ADD_SUBDIRECTORY("NFServerLogicCommon")


