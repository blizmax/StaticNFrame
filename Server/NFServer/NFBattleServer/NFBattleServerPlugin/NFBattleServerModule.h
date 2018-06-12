// -------------------------------------------------------------------------
//    @FileName         :    NFBattleServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServer/NFServerCommon/NFIBattleServerModule.h"

enum eMsgType;
class NFIPluginManager;
class NFINetServerModule;

class NFCBattleServerModule : public NFIBattleServerModule
{
public:
	/**
	 * @brief 构造函数
	 */
	explicit NFCBattleServerModule(NFIPluginManager* p);

	/**
	 * @brief 析构函数
	 */
	virtual ~NFCBattleServerModule();

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
	virtual bool Shut() override;

	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Execute() override;

	/**
	 * @brief BattleServer网络事件处理，主要是客户端的连接与断开
	 *
	 * @param  nEvent	事件eMsgType_CONNECTED连接，eMsgType_DISCONNECTED断开
	 * @param  unLinkId 网络连接的唯一ID
	 * @return void 
	 */
	void OnNetSocketEvent(const eMsgType nEvent, const uint32_t unLinkId);

	/**
	 * @brief 处理未注册的消息
	 *
	 * @param  unLinkId
	 * @param  playerId
	 * @param  nMsgId
	 * @param  msg
	 * @param  nLen
	 * @return void 
	 */
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
protected:
	/**
	 * @brief 处理连接
	 *
	 * @param  unLinkId
	 * @return void 
	 */
	void OnHandleConnected(const uint32_t unLinkId);

	/**
	 * @brief 处理端口
	 *
	 * @param  unLinkId
	 * @return void 
	 */
	void OnHnadleDisConnected(const uint32_t unLinkId);
private:
	NFINetServerModule* m_pNetServerModule;
};
