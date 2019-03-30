//------------------------------------------------------------------------ -
//    @FileName         :    NFNetPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-15
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFNetPlugin : public NFIPlugin
{
public:
	explicit NFNetPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};

