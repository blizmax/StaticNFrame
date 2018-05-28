// -------------------------------------------------------------------------
//    @FileName         :    $itemname$.cpp
//    @Author           :    GaoYi
//    @Date             :    $time$
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFEventData.h"
#include "NFPoll.h"

EventData::EventData()
{
	sock_flag = SOCKET_ACCEPT;
	event_flag = EVENT_NULL;
	sock = INVALID_SOCKET;
	poll = CreateModule();
}

EventData::~EventData()
{
	DeleteModule(poll);
}

