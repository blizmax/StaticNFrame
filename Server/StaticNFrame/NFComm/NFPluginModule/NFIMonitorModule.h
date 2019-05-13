// -------------------------------------------------------------------------
//    @FileName         :    NFIMonitorModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIModule.h"
#include <vector>
#include <string>
#include "NFSystemInfo.h"

class NFIMonitorModule : public NFIModule
{
public:
	virtual ~NFIMonitorModule()
	{

	}

	virtual const NFSystemInfo& GetSystemInfo() const = 0;
};

