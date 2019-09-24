// -------------------------------------------------------------------------
//    @FileName         :    NFCLogServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLogServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFILogServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCLogServerModule : public NFILogServerModule
{
public:
	explicit NFCLogServerModule(NFIPluginManager* p);
	virtual ~NFCLogServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
};
