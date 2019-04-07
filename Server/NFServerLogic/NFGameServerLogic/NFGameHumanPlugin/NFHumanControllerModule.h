// -------------------------------------------------------------------------
//    @FileName         :    NFHumanControllerModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"

class NFHumanControllerModule : public NFIModule
{
public:
	NFHumanControllerModule(NFIPluginManager* p);

	virtual ~NFHumanControllerModule();

	virtual bool Init();
public:
	/**
	* @brief 处理帐号登录功能
	*
	* @return void
	*/
	void OnHandleAccountLogin(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理获取玩家信息
	*
	* @return void
	*/
	void OnHandleGetPlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理重新连接
	*
	* @return void
	*/
	void OnHandleReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理剔除玩家
	*
	* @return void
	*/
	void OnHandleKitPlayer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理心跳
	*
	* @return void
	*/
	void OnHandleHeartBeat(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理获取初始化信息
	*
	* @return void
	*/
	void OnHandleGetInitInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理广播
	*
	* @return void
	*/
	void OnHandleBroadCast(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理邮件列表
	*
	* @return void
	*/
	void OnHandleGetMailList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理阅读邮件
	*
	* @return void
	*/
	void OnHandleGetReadMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理获取邮件物品
	*
	* @return void
	*/
	void OnHandleGetMailGoods(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理删除邮件
	*
	* @return void
	*/
	void OnHandleDeleteMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理玩家状态
	*
	* @return void
	*/
	void OnHandlePlayerStatus(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理更新玩家信息
	*
	* @return void
	*/
	void OnHandleUpdatePlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理更新物品信息
	*
	* @return void
	*/
	void OnHandleUpdateGoodsList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief 处理通知功能
	*
	* @return void
	*/
	void OnHandleNoticeInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
};
