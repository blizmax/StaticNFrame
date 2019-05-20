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
	** ���������Ҫ������������ָ��, ��̬���ص�ʱ������ָ����ܻ�ʧЧ
	*/
	virtual bool DynamicLoadPlugin();
public:
	/**
	* @brief �����ʺŵ�¼����
	*
	* @return void
	*/
	void OnHandleAccountLogin(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �����ȡ�����Ϣ
	*
	* @return void
	*/
	void OnHandleGetPlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief ������������
	*
	* @return void
	*/
	void OnHandleReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �����޳����
	*
	* @return void
	*/
	void OnHandleKitPlayer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �����ȡ��ʼ����Ϣ
	*
	* @return void
	*/
	void OnHandleGetInitInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief ����㲥
	*
	* @return void
	*/
	void OnHandleBroadCast(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �����ʼ��б�
	*
	* @return void
	*/
	void OnHandleGetMailList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �����Ķ��ʼ�
	*
	* @return void
	*/
	void OnHandleGetReadMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �����ȡ�ʼ���Ʒ
	*
	* @return void
	*/
	void OnHandleGetMailGoods(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief ����ɾ���ʼ�
	*
	* @return void
	*/
	void OnHandleDeleteMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief �������״̬
	*
	* @return void
	*/
	void OnHandlePlayerStatus(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief ������������Ϣ
	*
	* @return void
	*/
	void OnHandleUpdatePlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief ���������Ʒ��Ϣ
	*
	* @return void
	*/
	void OnHandleUpdateGoodsList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	* @brief ����֪ͨ����
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
