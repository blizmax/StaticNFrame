// -------------------------------------------------------------------------
//    @FileName         :    NFServerCheckPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2019/11/9
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCheckPlugin
//
// -------------------------------------------------------------------------
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFServerCheckPlugin : public NFIPlugin
{
public:
	explicit NFServerCheckPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual int GetPluginVersion();

	virtual std::string GetPluginName();

	virtual void Install();

	virtual void Uninstall();
};
