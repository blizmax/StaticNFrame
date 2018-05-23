// -------------------------------------------------------------------------
//    @FileName         :    NFCEventModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventModule
//
// -------------------------------------------------------------------------

#include "NFCEventModule.h"

#include "NFComm/NFPluginModule/NFIKernelModule.h"

bool NFCEventModule::Init()
{
	return true;
}

bool NFCEventModule::AfterInit()
{
	return true;
}

bool NFCEventModule::BeforeShut()
{
	return true;
}

bool NFCEventModule::Shut()
{
	return true;
}

bool NFCEventModule::Execute()
{
	return true;
}

//发送执行事件
void NFCEventModule::FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{
	m_ExecuteCenter.Fire(nEventID, nSrcID, bySrcType, pEventContext);
}
//订阅执行事件
bool NFCEventModule::Subscribe(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc)
{
	return m_ExecuteCenter.Subscribe(pSink, nEventID, nSrcID, bySrcType, desc);
}
//取消订阅执行事件
bool NFCEventModule::UnSubscribe(NFEventObj *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType)
{
	return m_ExecuteCenter.UnSubscribe(pSink, nEventID, nSrcID, bySrcType);
}

//取消所有执行事件的订阅
bool NFCEventModule::UnSubscribeAll(NFEventObj *pSink)
{
	m_ExecuteCenter.UnSubscribeAll(pSink);
	return true;
}