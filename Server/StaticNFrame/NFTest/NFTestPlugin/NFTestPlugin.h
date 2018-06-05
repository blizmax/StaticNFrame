// -------------------------------------------------------------------------
//    @FileName         :    NFTestPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFTestPlugin : public NFIPlugin
{
public:
	explicit NFTestPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual int GetPluginVersion();

	virtual std::string GetPluginName();

	virtual void Install();

	virtual void Uninstall();
};
