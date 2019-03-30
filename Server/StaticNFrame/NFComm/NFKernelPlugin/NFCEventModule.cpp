// -------------------------------------------------------------------------
//    @FileName         :    NFCEventModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#include "NFCEventModule.h"

#include "NFComm/NFPluginModule/NFEventMgr.h"

NFCEventModule::NFCEventModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	NFEventMgr::Instance()->Init(this);
}

NFCEventModule::~NFCEventModule()
{

}

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

//����ִ���¼�
void NFCEventModule::FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{
	m_ExecuteCenter.Fire(nEventID, nSrcID, bySrcType, pEventContext);
}

//����ִ���¼�
bool NFCEventModule::Subscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc)
{
	return m_ExecuteCenter.Subscribe(pSink, nEventID, nSrcID, bySrcType, desc);
}

//ȡ������ִ���¼�
bool NFCEventModule::UnSubscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType)
{
	return m_ExecuteCenter.UnSubscribe(pSink, nEventID, nSrcID, bySrcType);
}

//ȡ������ִ���¼��Ķ���
bool NFCEventModule::UnSubscribeAll(NFEventObj* pSink)
{
	m_ExecuteCenter.UnSubscribeAll(pSink);
	return true;
}

