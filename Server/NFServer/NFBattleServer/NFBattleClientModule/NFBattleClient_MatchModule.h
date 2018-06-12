// -------------------------------------------------------------------------
//    @FileName         :    NFBattleClient_MatchModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFBattleClientModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServer/NFServerCommon/NFIBattleClient_MatchModule.h"
#include <NFComm/NFPluginModule/NFINetClientModule.h>

enum eMsgType;

class NFCBattleClient_MatchModule : public NFIBattleClient_MatchModule
{
public:
	/**
	 * @brief 构造函数
	 */
	explicit NFCBattleClient_MatchModule(NFIPluginManager* p);

	/**
	 * @brief 析构函数
	 */
	virtual ~NFCBattleClient_MatchModule();

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
protected:
	/**
	 * @brief 处理连接
	 *
	 * @param  unLinkId
	 * @return void 
	 */
	void OnHandleConnected(const uint32_t unLinkId);

	/**
	 * @brief 处理连接断开
	 *
	 * @param  unLinkId
	 * @return void 
	 */
	void OnHnadleDisConnected(const uint32_t unLinkId);
private:
	NFINetClientModule* m_pNetClientModule;
	uint32_t mMatchServerUnlinkId;
};
