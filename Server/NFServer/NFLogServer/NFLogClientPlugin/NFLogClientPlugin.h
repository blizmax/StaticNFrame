// -------------------------------------------------------------------------
//    @FileName         :    NFLogClientPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFLogClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class NFLogClientPlugin : public NFIPlugin
{
public:
	explicit NFLogClientPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};

