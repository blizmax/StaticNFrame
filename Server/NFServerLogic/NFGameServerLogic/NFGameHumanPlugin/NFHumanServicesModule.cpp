// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFHumanServicesModule.h"

NFCHumanServicesModule::NFCHumanServicesModule(NFIPluginManager* p):NFIHumanServicesModule(p)
{

}

NFCHumanServicesModule::~NFCHumanServicesModule()
{

}

bool NFCHumanServicesModule::DynamicLoadPlugin()
{
	return true;
}