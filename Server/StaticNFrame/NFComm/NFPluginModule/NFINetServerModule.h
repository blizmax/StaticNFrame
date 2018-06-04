// -------------------------------------------------------------------------
//    @FileName         :    NFINetServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2017-10-16
//    @Module           :    NFINetServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"

class NFINetServerModule : public NFINetModule
{
public:
	virtual uint32_t AddServer(const NF_SERVER_TYPES eType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort) = 0;

	virtual void SendByServerID(NF_SERVER_TYPES eServerType, uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendByServerID(NF_SERVER_TYPES eServerType, uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToServerByPB(NF_SERVER_TYPES eServerType, uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;
};