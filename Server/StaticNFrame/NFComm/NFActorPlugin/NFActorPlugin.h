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

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
