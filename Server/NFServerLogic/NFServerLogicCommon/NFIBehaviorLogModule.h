// -------------------------------------------------------------------------
//    @FileName         :    NFIBehaviorLogModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFIBehaviorLogModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFServerLogicCommon.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"

class NFIBehaviorLogModule : public NFIModule, public NFTimerObj, public NFEventObj
{
public:
	/**
	* @brief 记录行为LOG
	*
	* @param  guid	玩家id
	* @param  nickName 玩家名字
	* @param  category
	* @return bool
	*/
	virtual void BehaviorLog(uint64_t guid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg) = 0;

	virtual void OnTimer(uint32_t nTimerID) override { }

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) override { }
};