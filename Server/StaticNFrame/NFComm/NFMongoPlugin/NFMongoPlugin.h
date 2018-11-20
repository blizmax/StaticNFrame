//------------------------------------------------------------------------ -
//    @FileName         :    NFMongoPlugin.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-20
//    @Module           :    NFMongoPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class NFMongoPlugin : public NFIPlugin
{
public:
	explicit NFMongoPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual int GetPluginVersion() override;

	virtual std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};


