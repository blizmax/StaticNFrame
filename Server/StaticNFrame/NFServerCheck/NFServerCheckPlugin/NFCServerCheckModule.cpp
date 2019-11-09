// -------------------------------------------------------------------------
//    @FileName         :    NFCServerCheckModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/11/9
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCheckPlugin
//
// -------------------------------------------------------------------------

#include "NFCServerCheckModule.h"
#include "NFComm/NFCore/NFTime.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCServerCheckModule::NFCServerCheckModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCServerCheckModule::~NFCServerCheckModule()
{
}

bool NFCServerCheckModule::Init()
{
	this->Subscribe(NFEVENT_LUA_FINISH_LOAD, 0, 0, "");
	return true;
}

void NFCServerCheckModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message)
{
	if (nEventID == NFEVENT_LUA_FINISH_LOAD)
	{

	}
}

void NFCServerCheckModule::OnTimer(uint32_t nTimerID)
{

}

bool NFCServerCheckModule::AfterInit()
{
	return true;
}


bool NFCServerCheckModule::Execute()
{
	return true;
}

bool NFCServerCheckModule::BeforeShut()
{
	return true;
}

bool NFCServerCheckModule::Shut()
{
	return true;
}