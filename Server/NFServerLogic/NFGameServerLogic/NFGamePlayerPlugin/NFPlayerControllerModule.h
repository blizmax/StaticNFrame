// -------------------------------------------------------------------------
//    @FileName         :    NFHumanControllerModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIPlayerControllerModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFPlayerDefine.h"
#include "NFServerLogic/NFServerLogicCommon/NFIPlayerModule.h"

class NFCPlayerControllerModule : public NFIPlayerControllerModule
{
public:
	NFCPlayerControllerModule(NFIPluginManager* p);

	virtual ~NFCPlayerControllerModule();

	virtual bool Init();

	virtual bool Shut();

	/*
	** 这个函数主要用来保存引擎指针, 动态加载的时候引擎指针可能会失效
	*/
	virtual bool DynamicLoadPlugin();
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

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) override;
public:
	static void CopyFromPlayerObject(NFMsg::playerinfo* pInfo, NFIObject* pPlayerObject);
protected:
	NFIPlayerModule* m_pPlayerModule;
};
