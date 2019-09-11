// -------------------------------------------------------------------------
//    @FileName         :    NFCNetClientModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFIClient.h"
#include "NFComm/NFCore/NFDataStream.h"
#include "NFComm/NFCore/NFBuffer.h"

class NFILuaScriptModule;

class NFCNetClientModule : public NFINetClientModule
{
public:
	/**
	 * @brief ���캯��
	 */
	explicit NFCNetClientModule(NFIPluginManager* p);

	/**
	 * @brief ��������
	 */
	virtual ~NFCNetClientModule();

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
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool Execute() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual bool Finalize() override;

	/**
	 * @brief ��ʼ���̳߳�
	 */
	virtual void InitThreadPool(uint32_t threadCount) override;

	/**
	 * @brief ���ӷ�����ip��port�����ش����������ӵ�ΨһID����������ͬ�ַ��������Ͷ��
	 * �ڲ�ͨ��������ʵ�ֿ��ٷ���
	 *
	 * @param  eServerType  ����������
	 * @param  strIp		���ӵļ���IP
	 * @param  nPort		���ӵĶ˿�
	 * @return uint32_t		���ؿͻ��˵�ΨһID������0��ʾ����
	 */
	virtual uint32_t AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, uint32_t nPort, uint32_t packetParsetype = 0, bool bWebSocket = false) override;

	/**
	* @brief ���ӷ�����ip��port�����ش����������ӵ�ΨһID����������ͬ�ַ��������Ͷ��
	* �ڲ�ͨ��������ʵ�ֿ��ٷ���
	*
	* @param  eServerType  ����������
	* @param  strIp		���ӵļ���IP
	* @param  nPort		���ӵĶ˿�
	* @return uint32_t		���ؿͻ��˵�ΨһID������0��ʾ����
	*/
	virtual uint32_t AddWebServer(NF_SERVER_TYPES eServerType, const std::string& url) override;

	/**
	 * @brief �ر�unLinkId�����������
	 *
	 * @param  unLinkId Ҫ�رյĿͻ��˵�Ψһid
	 * @return void
	 */
	virtual void CloseServer(const uint32_t unLinkId) override;

	/**
	 * @brief ͨ�����������ͣ��رտͻ�������
	 *
	 * @param  eServerType ����������
	 * @return void
	 */
	virtual void CloseServerByServerType(NF_SERVER_TYPES eServerType) override;

	/**
	 * @brief �ر����еĿͻ�������
	 *
	 * @return void
	 */
	virtual void CloseAllServer() override;

	/**
	 * @brief
	 *
	 * @param  unLinkId
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  unLinkId
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  unLinkId
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;

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
protected:
	/**
	 * @brief
	 *
	 * @return void
	 */
	void ProcessExecute();

	/**
	 * @brief
	 *
	 * @return void
	 */
	void ExecuteClose();

	/**
	 * @brief
	 *
	 * @param  pClient
	 * @return void
	 */
	void KeepState(NFIClient* pClient);

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @return uint32_t
	 */
	uint32_t GetFreeUnLinkId(NF_SERVER_TYPES eServerType);

	/**
	 * @brief
	 *
	 * @param  pClient
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	void SendMsg(NFIClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID);

	/**
	 * @brief
	 *
	 * @param  nEvent
	 * @param  unLinkId
	 * @return void
	 */
	void OnHandleNetEvent(const eMsgType nEvent, const uint32_t unLinkId);

	/**
	 * @brief
	 *
	 * @param  pClient
	 * @return void
	 */
	void OnConnected(NFIClient* pClient);

	/**
	 * @brief
	 *
	 * @param  pClient
	 * @return void
	 */
	void OnDisConnected(NFIClient* pClient);
private:
	std::vector<std::vector<NFIClient*>> mxServerMap;
	NFBuffer mxSendBuffer;
	evpp::EventLoop* m_eventLoop;
	evpp::EventLoopThreadPool* m_eventLoopThreadPool;
};

