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
	 * @brief ��ӷ�����
	 *
	 * @param  eType		����������
	 * @param  nServerID	������ID
	 * @param  nMaxClient	������������ӿͻ�����
	 * @param  nPort		�����������˿�
	 * @return int			����0����
	 */
	virtual uint32_t AddServer(const NF_SERVER_TYPES eType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort, bool bWebSocket = false, uint32_t nPacketParseType = 0, bool bForeignNetwork = false, bool bRefuseAttackIp = false) = 0;

	/**
	 * @brief ������ӵ�IP
	 *
	 * @param  usLinkId
	 * @return std::string 
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) = 0;

	/**
	* @brief �ر�����
	*
	* @param  usLinkId
	* @return
	*/
	virtual void CloseLinkId(uint32_t usLinkId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToServerByPB(uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToServerByPB(uint32_t usLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;
};