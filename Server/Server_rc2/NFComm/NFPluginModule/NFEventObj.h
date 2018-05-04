#pragma once

#include <string>
#include <stdint.h>

/* 事件使用注意事项
 取消订阅事件的时传入的参数一定要和订阅事件时传入的参数一致，
 事件内部是以订阅事件传入的参数（包括回调指针）组合为事件key的，
 如果取消和订阅时参数不一致， 取消订阅就会失败，就会导致有事件残留（包括野指针），下次该事件发生的时候触发回调就会异常

 事件嵌套层数不能太多，如果可以，尽量不要使用事件嵌套，主要是为了避免造成死循环，目前事件最大嵌套层数支持5层
*/

//所有事件发送过去的数据必须继承
class NFEventContext
{

};

//执行事件
class NFEventObj
{
public:
	NFEventObj();
	virtual ~NFEventObj();
public:
	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) = 0;
public:
	//发送执行事件
	void FireExecute(uint16_t nEventID,uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext);
	//订阅执行事件
	bool Subscribe(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc);
	//取消订阅执行事件
	bool UnSubscribe(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType);
	//取消所有执行事件的订阅
	bool UnSubscribeAll();
};
