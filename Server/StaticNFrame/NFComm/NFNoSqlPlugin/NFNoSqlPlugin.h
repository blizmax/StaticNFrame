//------------------------------------------------------------------------ -
//    @FileName         :    NFNoSqlPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFNoSqlPlugin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFNoSqlPlugin : public NFIPlugin
{
public:
    NFNoSqlPlugin(NFIPluginManager* p)
    {
        m_pPluginManager = p;
    }

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

    virtual void Install();

    virtual void Uninstall();
};