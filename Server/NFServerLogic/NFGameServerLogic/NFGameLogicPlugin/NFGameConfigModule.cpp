// -------------------------------------------------------------------------
//    @FileName         :    NFGameConfigModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameConfigModule
//
// -------------------------------------------------------------------------

#include "NFGameConfigModule.h"
#include "NFComm/NFPluginModule/NFCObject.h"

NFCGameConfigModule::NFCGameConfigModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_globalConfig = NF_NEW NFCObject(0, m_pPluginManager);;
}

NFCGameConfigModule::~NFCGameConfigModule()
{
	if (m_globalConfig)
	{
		NF_SAFE_DELETE(m_globalConfig);
	}
}

NFIObject* NFCGameConfigModule::GetGlobalConfig() const
{
	return m_globalConfig;
}

bool NFCGameConfigModule::Awake()
{
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