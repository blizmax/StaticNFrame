// -------------------------------------------------------------------------
//    @FileName         :    NFCTestKernelModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include <memory>

NFCTestKernelModule::NFCTestKernelModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pObject = nullptr;
}

bool NFCTestKernelModule::Init()
{
	//测试经可能覆盖所有的代码
	NF_SHARE_PTR<NFTestObject> pObject = NF_SHARE_PTR<NFTestObject>(NF_NEW NFTestObject());
	NF_SHARE_PTR<NFTestObject> pObject1 = NF_SHARE_PTR<NFTestObject>(NF_NEW NFTestObject());
	lastTime = 123456789;
	Subscribe(NFEVENT_TEST, lastTime, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST, lastTime, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST, lastTime, eEvent_two, __FUNCTION__);
	Subscribe(NFEVENT_TEST, lastTime, eEvent_three, __FUNCTION__);
	Subscribe(NFEVENT_TEST1, lastTime, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, lastTime, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST, 0, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 4, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 5, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 6, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 7, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 8, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 9, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 10, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 11, eEvent_one_cycle, __FUNCTION__);
	Subscribe(NFEVENT_TEST2, 12, eEvent_one_cycle, __FUNCTION__);
	FireExecute(NFEVENT_TEST, lastTime, eEvent_one_cycle, nullptr);
	pObject->UnSubscribeAll();
	pObject1->UnSubscribe(NFEVENT_TEST, lastTime, eEvent_one_cycle);
	pObject1->UnSubscribe(NFEVENT_TEST, lastTime, eEvent_two);
	pObject1->UnSubscribe(NFEVENT_TEST, lastTime, eEvent_three);
	return true;
}

bool NFCTestKernelModule::AfterInit()
{
	this->FireExecute(NFEVENT_TEST, lastTime, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST, 1, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST, 2, eEvent_one_cycle, nullptr);
	this->UnSubscribe(NFEVENT_TEST, 0, eEvent_one_cycle);
	this->UnSubscribe(NFEVENT_TEST, 0, eEvent_one_cycle);
	this->FireExecute(NFEVENT_TEST, 3, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST, 4, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST, lastTime, eEvent_two, nullptr);
	this->FireExecute(NFEVENT_TEST, lastTime, eEvent_three, nullptr);
	this->FireExecute(NFEVENT_TEST1, lastTime, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST2, lastTime, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST2, 4, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST2, 5, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST2, 6, eEvent_one_cycle, nullptr);
	this->FireExecute(NFEVENT_TEST2, 7, eEvent_one_cycle, nullptr);
	this->UnSubscribeAll();
	return true;
}

bool NFCTestKernelModule::Execute()
{
	return true;
}

bool NFCTestKernelModule::BeforeShut()
{
	return true;
}

bool NFCTestKernelModule::Shut()
{
	return true;
}

void NFCTestKernelModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == eTimer_test)
	{
		LogWarning(eTimer_test, "eTimer_test", "use Time:" + lexical_cast<std::string>(NFGetTime() - lastTime));
		lastTime = NFGetTime();
	}
}

void NFCTestKernelModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{
	std::cout << __FUNCTION__ << "::";
	std::cout << "--nEventID:" << nEventID;
	std::cout << "--nSrcID:" << nSrcID;
	std::cout << "--bySrcType:" << static_cast<uint32_t>(bySrcType);
	std::cout << std::endl;
	if (nEventID == NFEVENT_TEST2 && nSrcID == 4 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 4, eEvent_one_cycle, nullptr);
		this->UnSubscribe(nEventID, nSrcID, bySrcType);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 5 && bySrcType == eEvent_one_cycle)
	{
		this->UnSubscribe(nEventID, nSrcID, bySrcType);
		this->FireExecute(NFEVENT_TEST2, 5, eEvent_one_cycle, nullptr);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 6 && bySrcType == eEvent_one_cycle)
	{
		m_pObject = new NFTestObject();
		m_pObject->Subscribe(NFEVENT_TEST2, nSrcID, bySrcType, __FUNCTION__);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 7 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 8, eEvent_one_cycle, nullptr);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 8 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 9, eEvent_one_cycle, nullptr);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 9 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 10, eEvent_one_cycle, nullptr);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 10 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 11, eEvent_one_cycle, nullptr);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 11 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 12, eEvent_one_cycle, nullptr);
	}
	if (nEventID == NFEVENT_TEST2 && nSrcID == 12 && bySrcType == eEvent_one_cycle)
	{
		this->FireExecute(NFEVENT_TEST2, 7, eEvent_one_cycle, nullptr);
	}
}

