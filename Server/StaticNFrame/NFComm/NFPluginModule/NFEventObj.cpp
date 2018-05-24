#include <stdint.h>

#include "NFEventObj.h"
#include "NFEventMgr.h"

NFEventObj::NFEventObj()
{
}

NFEventObj::~NFEventObj()
{
	UnSubscribeAll();
}

//发送执行事件
void NFEventObj::FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{
	NFEventMgr::Instance()->FireExecute(nEventID, nSrcID, bySrcType, pEventContext);
}

//订阅执行事件
bool NFEventObj::Subscribe(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc)
{
	return NFEventMgr::Instance()->Subscribe(this, nEventID, nSrcID, bySrcType, desc);
}

//取消订阅执行事件
bool NFEventObj::UnSubscribe(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType)
{
	return NFEventMgr::Instance()->UnSubscribe(this, nEventID, nSrcID, bySrcType);
}

//取消所有执行事件的订阅
bool NFEventObj::UnSubscribeAll()
{
	return NFEventMgr::Instance()->UnSubscribeAll(this);
}

