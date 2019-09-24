// -------------------------------------------------------------------------
//    @FileName         :    NFCWebServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCWebServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIWebServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCWebServerModule : public NFIWebServerModule
{
public:
	explicit NFCWebServerModule(NFIPluginManager* p);
	virtual ~NFCWebServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
};
