//------------------------------------------------------------------------ -
//    @FileName         :    NFMysqlPlugin.h
//    @Author           :    Gao.Yi
//    @Date             :    2017-02-08
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class NFMysqlPlugin : public NFIPlugin
{
public:
	explicit NFMysqlPlugin(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	int GetPluginVersion() override;

	std::string GetPluginName() override;

	void Install() override;

	void Uninstall() override;
};
