//------------------------------------------------------------------------ -
//    @FileName         :    NFSqlitePlugin.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-21
//    @Module           :    NFSqlitePlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class NFSqlitePlugin : public NFIPlugin
{
public:
	explicit NFSqlitePlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};



