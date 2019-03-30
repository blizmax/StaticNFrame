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
	* @brief ���ط���������,������ÿһ��Plugin����Ҫ�������������ֻ��kernelplugin�Ƚ��������
	** ��Ϊconfig module ������ط��������ã������һ������
	* @return bool
	*/
	virtual bool OnReloadPlugin() override;
};

