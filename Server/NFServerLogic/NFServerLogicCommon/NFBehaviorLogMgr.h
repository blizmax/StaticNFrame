// -------------------------------------------------------------------------
//    @FileName         :    NFBehaviorLogMgr.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/08
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerLogicCommon
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFCore/NFPlatform.h"
#include "common/spdlog/fmt/fmt.h"
#include "NFIBehaviorLogModule.h"

class _NFExport NFBehaviorLogMgr : public NFSingleton<NFBehaviorLogMgr>
{
public:
	NFBehaviorLogMgr();
	virtual ~NFBehaviorLogMgr();
public:
	bool Init(NFIBehaviorLogModule* pModule = nullptr);
	void UnInit();
public:
	/**
	* @brief 记录行为LOG
	*
	* @param  guid	玩家id
	* @param  nickName 玩家名字
	* @param  category
	* @return bool
	*/
	void BehaviorLog(const std::string& cid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg);
protected:
	NFIBehaviorLogModule* m_pLogModule;
};

#define NFBehaviorLog(guid, nickName, category, tag, retCode, msg) NFBehaviorLogMgr::Instance()->BehaviorLog(cid, nickName, category, tag, retCode, msg);


