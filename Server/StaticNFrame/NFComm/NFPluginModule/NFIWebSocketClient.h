// -------------------------------------------------------------------------
//    @FileName         :    NFIWebSocketClient.h
//    @Author           :    GaoYi
//    @Date             :    2018/11/13
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIWebSocketServer.h"

#include <string>

template <typename SessionPTR>
class NFNetMsg
{
public:
	NFNetMsg(const SessionPTR session_ptr) : mSessionPtr(session_ptr)
	{
		mEventType = 0;
		mLinkId = 0;
	}

	uint32_t mEventType;
	uint32_t mLinkId;
	SessionPTR mSessionPtr;
	std::string mMsgData;
};

class NFIWebSocketClient : public NFIModule
{

};

