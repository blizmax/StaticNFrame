// -------------------------------------------------------------------------
//    @FileName         :    NFBasePoll.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFBasePoll.h"
#include "NFSelect.h"
#include "NFEpoll.h"

Module*  CreateModule()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	return new Select();
#else
	return new Epoll();
#endif
}

void DeleteModule(Module* module)
{
	assert(module);
	delete module;
}