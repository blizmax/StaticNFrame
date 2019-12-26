// -------------------------------------------------------------------------
//    @FileName         :    NFCLocationServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLocationServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFILocationServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFServer/NFServerCommon/NFICommonClient_MasterModule.h"

class NFCLocationServerModule : public NFILocationServerModule
{
public:
	explicit NFCLocationServerModule(NFIPluginManager* p);
	virtual ~NFCLocationServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
protected:
	NFICommonClient_MasterModule* m_pMasterClientModule;
};
