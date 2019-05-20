// -------------------------------------------------------------------------
//    @FileName         :    NFGameLhdPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2019/05/20
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameLhdPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class NFGameLhdPlugin : public NFIPlugin
{
public:
	explicit NFGameLhdPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;

	virtual bool IsDynamicLoad() override;
};

