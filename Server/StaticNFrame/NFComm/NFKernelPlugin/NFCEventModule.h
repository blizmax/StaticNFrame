// -------------------------------------------------------------------------
//    @FileName         :    NFCEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EVENT_MODULE_H
#define NFC_EVENT_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIEventModule.h"

#include "NFComm/NFPluginModule/NFEventTemplate.h"
#include "NFComm/NFPluginModule/NFEventObj.h"

/* 事件使用注意事项
 取消订阅事件的时传入的参数一定要和订阅事件时传入的参数一致，
 事件内部是以订阅事件传入的参数（包括回调指针）组合为事件key的，
 如果取消和订阅时参数不一致， 取消订阅就会失败，就会导致有事件残留（包括野指针），下次该事件发生的时候触发回调就会异常

 事件嵌套层数不能太多，如果可以，尽量不要使用事件嵌套，主要是为了避免造成死循环，目前事件最大嵌套层数支持5层
*/


//事件执行对象
struct OnEventExecuteObj 
{
	bool operator()(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
	{
		if (nullptr == pSink) return false;

		pSink->OnExecute(nEventID, nSrcID, bySrcType, pEventContext);
		return true;
	}
};

class NFIKernelModule;

class NFCEventModule
    : public NFIEventModule
{
public:
    explicit NFCEventModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~NFCEventModule()
    {
    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute();
public:
	/**
	* @brief 发送事件,并执行收到事件的对象的对应函数
	*  
	* @param nEventID		事件ID
	* @param nSrcID			事件源ID，一般都是玩家，生物唯一id
	* @param bySrcType		事件源类型，玩家类型，怪物类型之类的 
	* @param pEventContext	事件传输的数据
	* @return				执行是否成功 
	*/
	virtual void FireExecute(uint16_t nEventID,uint64_t nSrcID,uint8_t bySrcType, NFEventContext* pEventContext);

	/**
	* @brief 订阅事件
	*  
	* @param nEventID	事件ID
	* @param nSrcID		事件源ID，一般都是玩家，生物唯一id
	* @param bySrcType	事件源类型，玩家类型，怪物类型之类的
	* @param desc		事件描述，用于打印，获取信息，查看BUG之类的
	* @return			订阅事件是否成功 
	*/
	virtual bool Subscribe(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc);

	/**
	* @brief 取消订阅事件
	*  
	* @param nEventID	事件ID
	* @param nSrcID		事件源ID，一般都是玩家，生物唯一id
	* @param bySrcType	事件源类型，玩家类型，怪物类型之类的
	* @return			取消订阅事件是否成功 
	*/
	virtual bool UnSubscribe(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType);

	/**
	* @brief 取消NFEventObj所有订阅事件
	*  
	* @return			取消订阅事件是否成功 
	*/
	virtual bool UnSubscribeAll(NFEventObj *pSink);
private:
	NFEventTemplate<NFEventObj, OnEventExecuteObj> m_ExecuteCenter;
};

#endif