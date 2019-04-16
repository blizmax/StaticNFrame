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
#include "NFServerLogic/NFServerLogicCommon/NFIHumanModule.h"
#include "NFComm/NFCore/NFRandom.hpp"

NFCHumanControllerModule::NFCHumanControllerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCHumanControllerModule::~NFCHumanControllerModule()
{

}

bool NFCHumanControllerModule::Init()
{
	/**
	* @brief ��Э�鴦����
	*/
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_AccountLogin, this, &NFCHumanControllerModule::OnHandleAccountLogin);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetPlayerInfo, this, &NFCHumanControllerModule::OnHandleGetPlayerInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_ReConnect, this, &NFCHumanControllerModule::OnHandleReConnect);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_KitPlayer, this, &NFCHumanControllerModule::OnHandleKitPlayer);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_HeartBeat, this, &NFCHumanControllerModule::OnHandleHeartBeat);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetInitInfo, this, &NFCHumanControllerModule::OnHandleGetInitInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_BroadCast, this, &NFCHumanControllerModule::OnHandleBroadCast);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailList, this, &NFCHumanControllerModule::OnHandleGetMailList);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetReadMail, this, &NFCHumanControllerModule::OnHandleGetReadMail);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailGoods, this, &NFCHumanControllerModule::OnHandleGetMailGoods);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_DeleteMail, this, &NFCHumanControllerModule::OnHandleDeleteMail);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_PlayerStatus, this, &NFCHumanControllerModule::OnHandlePlayerStatus);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdatePlayerInfo, this, &NFCHumanControllerModule::OnHandleUpdatePlayerInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdateGoodsList, this, &NFCHumanControllerModule::OnHandleUpdateGoodsList);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_NoticeInfo, this, &NFCHumanControllerModule::OnHandleNoticeInfo);

	return true;
}

bool NFCHumanControllerModule::Shut()
{
	return true;
}

void NFCHumanControllerModule::OnTimer(uint32_t nTimerID)
{

}

/**
* @brief �����ʺŵ�¼����
*
* @return void
*/
void NFCHumanControllerModule::OnHandleAccountLogin(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NFMsg::gcaccountlogin gcMsg;
	NFINetServerModule* pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();

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
	NFMsg::playerinfo* pInfo = gcMsg.mutable_pinfo();

	NFIHumanModule* pHumanModule = m_pPluginManager->FindModule<NFIHumanModule>();

	uint32_t ret = pHumanModule->GetPlayerInfoByCID(cgMsg.account(), cgMsg.password(), pInfo);
	bool isNewPlayer = false;

	if (ret == RETURN_CODE_ACCOUNT_NO_EXIST)
	{
		pHumanModule->CreatePlayer(pInfo);

		uint32_t ret = pHumanModule->GetPlayerInfoByCID(cgMsg.account(), cgMsg.password(), pInfo);

		if (ret == RETURN_CODE_ACCOUNT_NO_EXIST)
		{
			gcMsg.set_result(RETURN_CODE_ACCOUNT_NO_EXIST);
			pNetServerModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
			NFBehaviorLog(playerId, cgMsg.cid(), "login", "AccountLogin", RETURN_CODE_ACCOUNT_NO_EXIST, "������ɫʧ�ܣ�account=" + cgMsg.account() + ",cid=" + cgMsg.cid());
			return;
		}

		isNewPlayer = true;
	}
	else if (ret == RETURN_CODE_PASSWORD_NOT_MATCH)
	{
		gcMsg.set_result(RETURN_CODE_PASSWORD_NOT_MATCH);
		pNetServerModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
		NFBehaviorLog(playerId, cgMsg.cid(), "login", "AccountLogin", RETURN_CODE_PASSWORD_NOT_MATCH, "����ʧ�ܣ����벻ƥ��=" + cgMsg.account() + ",password=" + cgMsg.password());
		return;
	}

	gcMsg.set_result(RETURN_CODE_SUCCESS);
	pNetServerModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
	NFBehaviorLog(pInfo->userid(), cgMsg.cid(), "login", "AccountLogin", RETURN_CODE_SUCCESS, "��ҵ�����Ϸ");
	return;
}

/**
* @brief �����ȡ�����Ϣ
*
* @return void
*/
void NFCHumanControllerModule::OnHandleGetPlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ������������
*
* @return void
*/
void NFCHumanControllerModule::OnHandleReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����޳����
*
* @return void
*/
void NFCHumanControllerModule::OnHandleKitPlayer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ��������
*
* @return void
*/
void NFCHumanControllerModule::OnHandleHeartBeat(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ȡ��ʼ����Ϣ
*
* @return void
*/
void NFCHumanControllerModule::OnHandleGetInitInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����㲥
*
* @return void
*/
void NFCHumanControllerModule::OnHandleBroadCast(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ʼ��б�
*
* @return void
*/
void NFCHumanControllerModule::OnHandleGetMailList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����Ķ��ʼ�
*
* @return void
*/
void NFCHumanControllerModule::OnHandleGetReadMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ȡ�ʼ���Ʒ
*
* @return void
*/
void NFCHumanControllerModule::OnHandleGetMailGoods(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����ɾ���ʼ�
*
* @return void
*/
void NFCHumanControllerModule::OnHandleDeleteMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �������״̬
*
* @return void
*/
void NFCHumanControllerModule::OnHandlePlayerStatus(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ������������Ϣ
*
* @return void
*/
void NFCHumanControllerModule::OnHandleUpdatePlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ���������Ʒ��Ϣ
*
* @return void
*/
void NFCHumanControllerModule::OnHandleUpdateGoodsList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����֪ͨ����
*
* @return void
*/
void NFCHumanControllerModule::OnHandleNoticeInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}