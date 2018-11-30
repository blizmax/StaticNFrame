// -------------------------------------------------------------------------
//    @FileName         :    NFEventDefine.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFEventObj.h"

enum NFEventDefine
{
	NFEVENT_TEST = 0, //测试事件， 用来测试
	NFEVENT_TEST1 = 1, //测试事件， 用来测试
	NFEVENT_TEST2 = 2, //测试事件， 用来测试
	NFEVENT_PROXY_CONNECT_GAME_SUCCESS, //PROXY 链接 GAME SERVER
	NFEVENT_PROXY_CONNECT_GAME_FAIL, //PROXY 链接 GAME SERVER
	NFEVENT_PROXY_CONNECT_WORLD_SUCCESS, //PROXY 链接 WORLD SERVER
	NFEVENT_PROXY_CONNECT_WORLD_FAIL, //PROXY 链接 WORLD SERVER
	NFEVENT_GAME_CONNECT_WORLD_SUCCESS, //GAME 链接 WORLD SERVER
	NFEVENT_GAME_CONNECT_WORLD_FAIL, //GAME 链接 WORLD SERVER
	NFEVENT_LOGIN_CONNECT_MASTER_SUCCESS, //LOGIN 链接 MASTER SERVER
	NFEVENT_LOGIN_CONNECT_MASTER_FAIL, //LOGIN 链接 MASTER SERVER
	NFEVENT_WORLD_CONNECT_MASTER_SUCCESS, //LOGIN 链接 MASTER SERVER
	NFEVENT_WORLD_CONNECT_MASTER_FAIL, //LOGIN 链接 MASTER SERVER
};

