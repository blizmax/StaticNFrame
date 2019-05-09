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

//����ִ���¼�
void NFEventMgr::FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) const
{
	if (m_pEventModule)
	{
		m_pEventModule->FireExecute(nEventID, nSrcID, bySrcType, message);
	}
}

//����ִ���¼�
bool NFEventMgr::Subscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc) const
{
	if (m_pEventModule)
	{
		return m_pEventModule->Subscribe(pSink, nEventID, nSrcID, bySrcType, desc);
	}
	return false;
}

//ȡ������ִ���¼�
bool NFEventMgr::UnSubscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType) const
{
	if (m_pEventModule)
	{
		return m_pEventModule->UnSubscribe(pSink, nEventID, nSrcID, bySrcType);
	}
	return false;
}

//ȡ������ִ���¼��Ķ���
bool NFEventMgr::UnSubscribeAll(NFEventObj* pSink) const
{
	if (m_pEventModule)
	{
		return m_pEventModule->UnSubscribeAll(pSink);
	}
	return true;
}

