// -------------------------------------------------------------------------
//    @FileName         :    NFEventModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFNetBase.h"

class NFPoll
{
public:
	virtual bool Init(int max_sock) = 0;
	virtual void UnInit() = 0;
	virtual bool Poll(bool poolWrite, bool poolRead, uint64_t timeout_ms) = 0;

public:
	virtual bool AddEvent(SOCKET sock, EventFlag flag, EventData* ptr) = 0;
	virtual bool ModEvent(SOCKET sock, EventFlag flag, EventData* ptr) = 0;
	virtual bool DelEvent(SOCKET sock, EventData* ptr) = 0;

public:
	NFPoll() {}
	virtual ~NFPoll() {}
};

NFPoll*  CreateModule();
void DeleteModule(NFPoll* module);
