// -------------------------------------------------------------------------
//    @FileName         :    NFMasterServerPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFMasterServerPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class NFMasterServerPlugin : public NFIPlugin
{
public:
	explicit NFMasterServerPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};

