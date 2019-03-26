//------------------------------------------------------------------------ -
//    @FileName         :    NFMonitorPlugin.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-21
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

/*
** ·þÎñÆ÷¼à¿ØÒýÇæ
*/
//////////////////////////////////////////////////////////////////////////
class NFMonitorPlugin : public NFIPlugin
{
public:
	explicit NFMonitorPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};



