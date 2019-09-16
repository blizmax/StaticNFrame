// -------------------------------------------------------------------------
//    @FileName         :    NFTestPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFRebotPlugin
//
// -------------------------------------------------------------------------
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFRebotPlugin : public NFIPlugin
{
public:
	explicit NFRebotPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual int GetPluginVersion();

	virtual std::string GetPluginName();

	virtual void Install();

	virtual void Uninstall();
};
