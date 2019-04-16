// -------------------------------------------------------------------------
//    @FileName         :    NFGameConfigModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameConfigModule
//
// -------------------------------------------------------------------------

#include "NFGameConfigModule.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include "NFComm/NFPluginModule/NFCObject.h"

NFCGameConfigModule::NFCGameConfigModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCGameConfigModule::~NFCGameConfigModule()
{

}


bool NFCGameConfigModule::Awake()
{
	GetGlobalConfigObject()->AddNode(GAME_CONFIG_INIT_JETTON, NFCData(NF_DT_INT, 0));
	GetGlobalConfigObject()->AddNode(GAME_CONFIG_INIT_MONEY, NFCData(NF_DT_INT, 0));
	return true;
}

bool NFCGameConfigModule::Finalize()
{
	return true;
}

bool NFCGameConfigModule::OnReloadPlugin()
{
	return true;
}