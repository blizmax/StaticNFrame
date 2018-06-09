// -------------------------------------------------------------------------
//    @FileName         :    NFEventMgr.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFEventMgr.h"

#include "NFComm/NFPluginModule/NFIEventModule.h"

NFEventMgr::NFEventMgr()
{
	m_pEventModule = nullptr;
}

NFEventMgr::~NFEventMgr()
{
}

bool NFEventMgr::Init(NFIEventModule* pEventModule)
{
	m_pEventModule = pEventModule;
	return true;
}

void NFEventMgr::UnInit()
{
	m_pEventModule = nullptr;
}

//发送执行事件
void NFEventMgr::FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) const
{
	if (m_pEventModule)
	{
		m_pEventModule->FireExecute(nEventID, nSrcID, bySrcType, pEventContext);
	}
}

//订阅执行事件
bool NFEventMgr::Subscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc) const
{
	if (m_pEventModule)
	{
		return m_pEventModule->Subscribe(pSink, nEventID, nSrcID, bySrcType, desc);
	}
	return false;
}

//取消订阅执行事件
bool NFEventMgr::UnSubscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType) const
{
	if (m_pEventModule)
	{
		return m_pEventModule->UnSubscribe(pSink, nEventID, nSrcID, bySrcType);
	}
	return false;
}

//取消所有执行事件的订阅
bool NFEventMgr::UnSubscribeAll(NFEventObj* pSink) const
{
	if (m_pEventModule)
	{
		return m_pEventModule->UnSubscribeAll(pSink);
	}
	return true;
}

