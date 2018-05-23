#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFActorPlugin : public NFIPlugin
{
public:
	explicit NFActorPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual ~NFActorPlugin() { }

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};
