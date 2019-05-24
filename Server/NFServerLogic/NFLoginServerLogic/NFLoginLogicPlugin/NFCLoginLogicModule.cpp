

#include "NFCLoginLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFMD5.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"

NFCLoginLogicModule::NFCLoginLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCLoginLogicModule::~NFCLoginLogicModule()
{

}

bool NFCLoginLogicModule::Init()
{
	return true;
}

bool NFCLoginLogicModule::AfterInit()
{
	return true;
}

bool NFCLoginLogicModule::Execute()
{
	return true;
}

bool NFCLoginLogicModule::BeforeShut()
{
	return true;
}

bool NFCLoginLogicModule::Shut()
{
	return true;
}