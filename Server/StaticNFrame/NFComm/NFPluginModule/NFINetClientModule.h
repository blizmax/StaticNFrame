// -------------------------------------------------------------------------
//    @FileName         :    NFINetClientModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFINetModule.h"

class NFINetClientModule : public NFINetModule
{
public:
	/**
	 * @brief 初始化线程池
	 */
	virtual void InitThreadPool(uint32_t threadCount) = 0;
	/**
	 * @brief 连接服务器ip和port，返回代表这里连接的唯一ID，可以连接同种服务器类型多次
	 * 内部通过数组来实现快速访问
	 *
	 * @param  eServerType  服务器类型
	 * @param  strIp		连接的监听IP
	 * @param  nPort		连接的端口
	 * @return uint32_t		返回客户端的唯一ID，返回0表示错误
	 */
	virtual uint32_t AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, uint32_t nPort, uint32_t packetParsetype = 0, bool bWebSocket = false) = 0;

	/**
	* @brief 连接服务器ip和port，返回代表这里连接的唯一ID，可以连接同种服务器类型多次
	* 内部通过数组来实现快速访问
	*
	* @param  eServerType  服务器类型
	* @param  strIp		连接的监听IP
	* @param  nPort		连接的端口
	* @return uint32_t		返回客户端的唯一ID，返回0表示错误
	*/
	virtual uint32_t AddWebServer(NF_SERVER_TYPES eServerType, const std::string& url) = 0;

	virtual void CloseServer(const uint32_t unLinkId) = 0;

	virtual void CloseServerByServerType(NF_SERVER_TYPES eServerType) = 0;

	virtual void CloseAllServer() = 0;

	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) = 0;
};

