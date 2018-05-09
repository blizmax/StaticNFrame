// -------------------------------------------------------------------------
//    @FileName         :    NFIEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEventModule
//
// -------------------------------------------------------------------------

#ifndef NFI_EVENT_MODULE_H
#define NFI_EVENT_MODULE_H

#include <iostream>
#include "NFIModule.h"

#include "NFComm/NFPluginModule/NFEventObj.h"

class NFIEventModule
    : public NFIModule
{
public:
	public:
	//发送执行事件
	virtual void FireExecute(uint16_t nEventID,uint64_t nSrcID,uint8_t bySrcType, NFEventContext* pEventContext) = 0;

	//订阅执行事件
	virtual bool Subscribe(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc) = 0;

	//取消订阅执行事件
	virtual bool UnSubscribe(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType) = 0;

	//取消所有执行事件的订阅
	virtual bool UnSubscribeAll(NFEventObj *pSink) = 0;
};

#endif