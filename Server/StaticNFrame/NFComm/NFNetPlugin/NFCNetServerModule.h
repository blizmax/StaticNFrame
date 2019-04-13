// -------------------------------------------------------------------------
//    @FileName         :    NFCNetServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2017-10-16
//    @Module           :    NFCNetServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFINetServerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer.h"
#include <NFComm/NFCore/NFBuffer.h>

class NFILuaScriptModule;

class NFCNetServerModule : public NFINetServerModule
{
public:
	/**
	 * @brief ���캯��
	 */
	NFCNetServerModule(NFIPluginManager* p);

	/**
	 * @brief ��������
	 */
	virtual ~NFCNetServerModule();

	/**
	* @brief
	*
	* @return bool
	*/
	virtual bool Awake() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool Init() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool AfterInit() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool BeforeShut() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool Shut() override;

	/**
	 * @brief �ͷ�����
	 *
	 * @return bool
	 */
	virtual bool Finalize() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool Execute() override;

	/**
	 * @brief ��ӷ�����
	 *
	 * @param  eType		����������
	 * @param  nServerID	������ID
	 * @param  nMaxClient	������������ӿͻ�����
	 * @param  nPort		�����������˿�
	 * @return int			����0����
	 */
	virtual uint32_t AddServer(const NF_SERVER_TYPES eServerType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort, bool bWebSocket = false, uint32_t nPacketParseType = 0) override;

	/**
	 * @brief ������ӵ�IP
	 *
	 * @param  usLinkId
	 * @return std::string 
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) override;

	/**
	* @brief �ر�����
	*
	* @param  usLinkId
	* @return
	*/
	virtual void CloseLinkId(uint32_t usLinkId) override;

	/**
	 * @brief
	 *
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToServerByPB(uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;

	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg) override;

	virtual void RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId) override;
protected:
	/**
	 * @brief ����Ϣ�����ͨ��pServer���ͳ�ȥ
	 *
	 * @param  pServer
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	void SendMsg(NFServer* pServer, uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID);

	/**
	 * @brief
	 *
	 * @param  pServer
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	void SendAllMsg(NFServer* pServer, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID);
private:
	std::vector<NFServer*> mServerArray;
	NFBuffer mxSendBuffer;
	NFILuaScriptModule* m_pLuaScriptModule;
};

