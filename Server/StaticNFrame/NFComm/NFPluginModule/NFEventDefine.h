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
};

class NFTestEventContext : public NFEventContext
{
public:
	int a = 1;
	int b = 2;
	int c = 3;
};

