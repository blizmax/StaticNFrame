// -------------------------------------------------------------------------
//    @FileName         :    NFKernelPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFKernelPlugin : public NFIPlugin
{
public:
	explicit NFKernelPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFKernelPlugin()
	{
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;

	/**
	* @brief 加载服务器引擎,并不是每一个Plugin都需要重载这个函数，只是kernelplugin比较特殊罢了
	** 因为config module 负责加载服务器配置，必须第一个启动
	* @return bool
	*/
	virtual bool OnReloadPlugin() override;
};

