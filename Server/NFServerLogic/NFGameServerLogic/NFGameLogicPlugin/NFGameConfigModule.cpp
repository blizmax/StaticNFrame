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
	std::string initFace = "90001_1001,90002_1001,90003_1001,90004_1001,90005_1001,90006_1001,";
	initFace += "90007_1001, 90008_1001, 90009_1001, 90010_1001, 90011_1001, 90012_1001, 90013_1001,";
	initFace += "90014_1001, 90015_1001, 90016_1001, 90017_1001, 90018_100";
	GetGlobalConfigObject()->AddNode(GAME_CONFIG_INIT_JETTON, NFCData(NF_DT_INT, 0));
	GetGlobalConfigObject()->AddNode(GAME_CONFIG_INIT_MONEY, NFCData(NF_DT_INT, 0));
	GetGlobalConfigObject()->AddNode(GAME_CONFIG_INIT_FACE, NFCData(NF_DT_STRING, initFace));
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