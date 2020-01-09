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
	 * @brief 构造函数
	 */
	explicit NFCNetClientModule(NFIPluginManager* p);

	/**
	 * @brief 析构函数
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
	 * @brief 初始化线程池
	 */
	virtual void InitThreadPool(uint32_t threadCount) override;

	/**
	 * @brief 连接服务器ip和port，返回代表这里连接的唯一ID，可以连接同种服务器类型多次
	 * 内部通过数组来实现快速访问
	 *
	 * @param  eServerType  服务器类型
	 * @param  strIp		连接的监听IP
	 * @param  nPort		连接的端口
	 * @return uint32_t		返回客户端的唯一ID，返回0表示错误
	 */
	virtual uint32_t AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, uint32_t nPort, uint32_t packetParsetype = 0, bool bWebSocket = false) override;

	/**
	 * @brief 关闭unLinkId所代表的连接
	 *
	 * @param  unLinkId 要关闭的客户端的唯一id
	 * @return void
	 */
	virtual void CloseServer(const uint32_t unLinkId) override;

	/**
	 * @brief 关闭unLinkId所代表的连接
	 *
	 * @param  unLinkId 要关闭的客户端的唯一id
	 * @return void
	 */
	virtual void DisconnectServer(const uint32_t unLinkId) override;

	/**
	 * @brief 通过服务器类型，关闭客户端连接
	 *
	 * @param  eServerType 服务器类型
	 * @return void
	 */
	virtual void CloseServerByServerType(NF_SERVER_TYPES eServerType) override;

	/**
	 * @brief 关闭所有的客户端连接
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
	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) override;

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
	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) override;

	/**
	 * @brief
	 *
	 * @param  unLinkId
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) override;

	/**
	 * @brief
	 *
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) override;

	/**
	 * @brief
	 *
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) override;

	/**
	 * @brief
	 *
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) override;

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
	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void
	 */
	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) override;

	///////////////////////////////////////////////////////////////////////////////////////
	virtual void SendByServerID(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendByServerID(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToServerByPB(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToAllServerByPB(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) override;

	virtual NFIClient* GetClientByUnlinkId(uint32_t unLinkId);
public:
	virtual std::future<NFRpcReqResult> AsyncCall(uint32_t unLinkId, const std::string& rpc_name, const char* msg, const uint32_t nLen) override;
	virtual void AsyncCall(uint32_t unLinkId, const std::string& rpc_name, std::function<void(uint32_t error_code, const std::string& data)> cb, uint32_t timeout, const char* msg, const uint32_t nLen) override;
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
	void SendMsg(NFIClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId);

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
#ifdef USE_NET_EVPP
	evpp::EventLoop* m_eventLoop;
	evpp::EventLoopThreadPool* m_eventLoopThreadPool;
#endif
};

