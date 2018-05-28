// -------------------------------------------------------------------------
//    @FileName         :    NFBasePoll.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFPoll.h"
#include "NFSelect.h"
#include "NFEpoll.h"

NFPoll* CreateModule()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	return new NFSelect();
#else
	return new NFEpoll();
#endif
}

void DeleteModule(NFPoll* module)
{
	assert(module);
	delete module;
}

