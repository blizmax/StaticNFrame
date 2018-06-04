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

class NFCNetServerModule : public NFINetServerModule
{
public:
    /**
     * @brief 构造函数
     */
    NFCNetServerModule(NFIPluginManager* p);

    /**
     * @brief 析构函数
     */
	virtual ~NFCNetServerModule();

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
	 * @brief 释放数据
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
     * @brief 添加服务器
     *
     * @param  eType		服务器类型
     * @param  nServerID	服务器ID
     * @param  nMaxClient	服务器最大连接客户端数
     * @param  nPort		服务器监听端口
     * @return int			返回0错误
     */
    virtual uint32_t AddServer(const NF_SERVER_TYPES eType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  strData
	 * @param  nPlayerID
	 * @return void 
	 */
	virtual void SendByServerID(NF_SERVER_TYPES eServerType, uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  msg
	 * @param  nLen
	 * @param  nPlayerID
	 * @return void 
	 */
	virtual void SendByServerID(NF_SERVER_TYPES eServerType, uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	/**
	 * @brief
	 *
	 * @param  eServerType
	 * @param  usLinkId
	 * @param  nMsgID
	 * @param  xData
	 * @param  nPlayerID
	 * @return void 
	 */
	virtual void SendToServerByPB(NF_SERVER_TYPES eServerType, uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;
protected:
	/**
	 * @brief 将消息编码后通过pServer发送出去
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
private:
	std::vector<NFServer*> mServerArray;
	NFBuffer mxSendBuffer;
};

