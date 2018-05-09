// -------------------------------------------------------------------------
//    @FileName         :    {Filename}
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Module           :   {Filename}
//
// -------------------------------------------------------------------------

#include "NFCTestKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

NFCTestKernelModule::NFCTestKernelModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

bool NFCTestKernelModule::Init()
{
	lastTime = NFGetTime();
	SetTimer(eTimer_test, 1000, 10);
	Subscribe(NFEVENT_TEST, eTimer_test, 0, "NFCTestKernelModule");
    return true;
}

bool NFCTestKernelModule::AfterInit()
{
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
		LogWarning(eTimer_test, "eTimer_test", "use Time:" + lexical_cast<std::string>(NFGetTime()-lastTime));
		lastTime = NFGetTime();
	}
}

void NFCTestKernelModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{
	if (nEventID == NFEVENT_TEST)
	{
		NFTestEvent* pEvent = static_cast<NFTestEvent*>(pEventContext);
		if (pEvent)
		{
			std::cout << "nEventID:" << nEventID << ", nSrcID:" << nSrcID << ", bySrcType:" << bySrcType << std::endl;
		}
		//为什么能转换过来
		NFTestEvent0* pEvent0 = static_cast<NFTestEvent0*>(pEventContext);
		if (pEvent0)
		{
			std::cout << "nEventID:" << nEventID << ", nSrcID:" << nSrcID << ", bySrcType:" << bySrcType << std::endl;
		}
	}
}
