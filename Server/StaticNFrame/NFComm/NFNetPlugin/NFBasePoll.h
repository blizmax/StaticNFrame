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

class Module
{
public:
	virtual bool Init(int max_sock) = 0;
	virtual void UnInit() = 0;
	virtual bool Poll(bool poolWrite, bool poolRead, uint64_t timeout_ms) = 0;

public:
	virtual bool AddEvent(SOCKET sock, EventFlag flag, void* ptr) = 0;
	virtual bool ModEvent(SOCKET sock, EventFlag flag, void* ptr) = 0;
	virtual bool DelEvent(SOCKET sock, void* ptr) = 0;

public:
	Module() {}
	virtual ~Module() {}
};

Module*  CreateModule();
void DeleteModule(Module* module);
