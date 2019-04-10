// -------------------------------------------------------------------------
//    @FileName         :    NFHumanControllerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFHumanControllerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINetServerModule.h"

#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"
#include "NFComm/NFCore/NFRandom.hpp"

NFHumanControllerModule::NFHumanControllerModule(NFIPluginManager* p):NFIDynamicModule(p)
{
	m_pPluginManager = p;
}

NFHumanControllerModule::~NFHumanControllerModule()
{

}

bool NFHumanControllerModule::Init()
{
	/**
	* @brief ��Э�鴦����
	*/
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_AccountLogin, this, &NFHumanControllerModule::OnHandleAccountLogin);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetPlayerInfo, this, &NFHumanControllerModule::OnHandleGetPlayerInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_ReConnect, this, &NFHumanControllerModule::OnHandleReConnect);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_KitPlayer, this, &NFHumanControllerModule::OnHandleKitPlayer);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_HeartBeat, this, &NFHumanControllerModule::OnHandleHeartBeat);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetInitInfo, this, &NFHumanControllerModule::OnHandleGetInitInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_BroadCast, this, &NFHumanControllerModule::OnHandleBroadCast);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailList, this, &NFHumanControllerModule::OnHandleGetMailList);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetReadMail, this, &NFHumanControllerModule::OnHandleGetReadMail);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailGoods, this, &NFHumanControllerModule::OnHandleGetMailGoods);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_DeleteMail, this, &NFHumanControllerModule::OnHandleDeleteMail);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_PlayerStatus, this, &NFHumanControllerModule::OnHandlePlayerStatus);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdatePlayerInfo, this, &NFHumanControllerModule::OnHandleUpdatePlayerInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdateGoodsList, this, &NFHumanControllerModule::OnHandleUpdateGoodsList);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_NoticeInfo, this, &NFHumanControllerModule::OnHandleNoticeInfo);

	return true;
}

bool NFHumanControllerModule::Shut()
{
	return true;
}

void NFHumanControllerModule::OnTimer(uint32_t nTimerID)
{

}

/**
* @brief �����ʺŵ�¼����
*
* @return void
*/
void NFHumanControllerModule::OnHandleAccountLogin(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NFMsg::gcaccountlogin gcMsg;

	if (cgMsg.cid() == "")
	{
		std::string cid = NFRandomString(8);
		cgMsg.set_cid(cid);

		NFBehaviorLog(playerId, cid, "login", "AccountLogin", 0, "�Զ����������CID=" + cid);
	}

	bool quickLogin = false;
	if (cgMsg.account() == "")
	{
		quickLogin = true;
		cgMsg.set_account(cgMsg.cid());
		cgMsg.set_password(cgMsg.cid());
		NFBehaviorLog(playerId, cgMsg.cid(), "login", "AccountLogin", 0, "���ٵ�¼��account=" + cgMsg.cid());
	}

	if (cgMsg.nickname() == "")
	{
		if (cgMsg.devname() != "")
		{
			cgMsg.set_nickname(cgMsg.devname());
		}
		else
		{
			cgMsg.set_nickname(cgMsg.cid());
		}
	}

	gcMsg.set_result(0);
	gcMsg.mutable_pinfo()->set_userid(0);

}

/**
* @brief �����ȡ�����Ϣ
*
* @return void
*/
void NFHumanControllerModule::OnHandleGetPlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ������������
*
* @return void
*/
void NFHumanControllerModule::OnHandleReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����޳����
*
* @return void
*/
void NFHumanControllerModule::OnHandleKitPlayer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ��������
*
* @return void
*/
void NFHumanControllerModule::OnHandleHeartBeat(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ȡ��ʼ����Ϣ
*
* @return void
*/
void NFHumanControllerModule::OnHandleGetInitInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����㲥
*
* @return void
*/
void NFHumanControllerModule::OnHandleBroadCast(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ʼ��б�
*
* @return void
*/
void NFHumanControllerModule::OnHandleGetMailList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����Ķ��ʼ�
*
* @return void
*/
void NFHumanControllerModule::OnHandleGetReadMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ȡ�ʼ���Ʒ
*
* @return void
*/
void NFHumanControllerModule::OnHandleGetMailGoods(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����ɾ���ʼ�
*
* @return void
*/
void NFHumanControllerModule::OnHandleDeleteMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �������״̬
*
* @return void
*/
void NFHumanControllerModule::OnHandlePlayerStatus(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ������������Ϣ
*
* @return void
*/
void NFHumanControllerModule::OnHandleUpdatePlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ���������Ʒ��Ϣ
*
* @return void
*/
void NFHumanControllerModule::OnHandleUpdateGoodsList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����֪ͨ����
*
* @return void
*/
void NFHumanControllerModule::OnHandleNoticeInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}