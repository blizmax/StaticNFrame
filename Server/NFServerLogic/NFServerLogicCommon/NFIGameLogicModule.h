// -------------------------------------------------------------------------
//    @FileName         :    NFIGameLogicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerLogicCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"

class NFIGameLogicModule : public NFIModule
{
public:
	virtual void SendMsgToClientByPlayerId(uint64_t playerId, uint32_t nMsgId, const google::protobuf::Message& xData) = 0;
	virtual void SendMsgToWorldByPlayerId(uint64_t playerId, uint32_t nMsgId, const google::protobuf::Message& xData) = 0;
};

