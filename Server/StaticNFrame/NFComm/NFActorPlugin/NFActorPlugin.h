// -------------------------------------------------------------------------
//    @FileName         :    NFActorPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFActorPlugin
//
// -------------------------------------------------------------------------
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFActorPlugin : public NFIPlugin
{
public:
	explicit NFActorPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFActorPlugin()
	{
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};

