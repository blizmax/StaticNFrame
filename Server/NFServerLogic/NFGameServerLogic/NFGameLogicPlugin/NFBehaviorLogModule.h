// -------------------------------------------------------------------------
//    @FileName         :    NFBehaviorLogModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameLogicPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFIBehaviorLogModule.h"

class NFILogModule;
class NFISqliteModule;

class NFCBehaviorLogModule : public NFIBehaviorLogModule
{
public:
	NFCBehaviorLogModule(NFIPluginManager* p);
	virtual ~NFCBehaviorLogModule();

	virtual bool Awake() override;
	virtual bool Finalize() override;

public:
	void BehaviorLog(uint64_t guid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg);
};
