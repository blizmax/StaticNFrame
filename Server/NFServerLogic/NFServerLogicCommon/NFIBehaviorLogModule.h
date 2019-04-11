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

class NFIBehaviorLogModule : public NFIModule
{
public:
	/**
	* @brief ��¼��ΪLOG
	*
	* @param  guid	���id
	* @param  nickName �������
	* @param  category
	* @return bool
	*/
	virtual void BehaviorLog(uint64_t guid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg) = 0;
};