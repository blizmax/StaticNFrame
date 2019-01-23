//------------------------------------------------------------------------ -
//    @FileName         :    NFAcePlugin.h
//    @Author           :    Gao.Yi
//    @Date             :    2019-01-22
//    @Module           :    NFAcePlugin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFAcePlugin : public NFIPlugin
{
public:
	explicit NFAcePlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};

