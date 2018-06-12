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
	/**
	 * @brief 添加服务器
	 *
	 * @param  eType		服务器类型
	 * @param  nServerID	服务器ID
	 * @param  nMaxClient	服务器最大连接客户端数
	 * @param  nPort		服务器监听端口
	 * @return int			返回0错误
	 */
	virtual uint32_t AddServer(const NF_SERVER_TYPES eType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort) = 0;

	/**
	 * @brief 获得连接的IP
	 *
	 * @param  usLinkId
	 * @return std::string 
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToServerByPB(uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;
};

