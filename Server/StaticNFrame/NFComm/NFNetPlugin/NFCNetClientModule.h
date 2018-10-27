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
#include "NFClient.h"
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
	 * @brief 连接服务器ip和port，返回代表这里连接的唯一ID，可以连接同种服务器类型多次
	 * 内部通过数组来实现快速访问
	 *
	 * @param  eServerType  服务器类型
	 * @param  strIp		连接的监听IP
	 * @param  nPort		连接的端口
	 * @return uint32_t		返回客户端的唯一ID，返回0表示错误
	 */
	virtual uint32_t AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, const int nPort) override;

	/**
	 * @brief 关闭unLinkId所代表的连接
	 *
	 * @param  unLinkId 要关闭的客户端的唯一id
	 * @return void
	 */
	virtual void CloseServer(const uint32_t unLinkId) override;

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
	void KeepState(NFClient* pClient);

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
	void SendMsg(NFClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID);

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
	void OnConnected(NFClient* pClient);

	/**
	 * @brief
	 *
	 * @param  pClient
	 * @return void
	 */
	void OnDisConnected(NFClient* pClient);

public:
	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg);

	virtual void RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId);
private:
	std::vector<std::vector<NFClient*>> mxServerMap;
	NFBuffer mxSendBuffer;
	NFILuaScriptModule* m_pLuaScriptModule;
};

