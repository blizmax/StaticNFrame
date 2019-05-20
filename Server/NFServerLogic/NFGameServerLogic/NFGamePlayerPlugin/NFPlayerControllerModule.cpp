// -------------------------------------------------------------------------
//    @FileName         :    NFHumanControllerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFPlayerControllerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINetServerModule.h"

#include "NFServerLogic/NFServerLogicCommon/NFPlayerDefine.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"

#include "NFComm/NFCore/NFRandom.hpp"
#include "NFMessageDefine/NFNodeClass.h"
#include "NFMessageDefine/server_to_server_msg.pb.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

NFCPlayerControllerModule::NFCPlayerControllerModule(NFIPluginManager* p):NFIPlayerControllerModule(p)
{
	
}

NFCPlayerControllerModule::~NFCPlayerControllerModule()
{

}

bool NFCPlayerControllerModule::Init()
{
	/**
	* @brief ��Э�鴦����
	*/
	AddNetClientReceiveCallBack(NF_ST_WORLD, ::NFMsg::Client_Msg_AccountLogin, this, &NFCPlayerControllerModule::OnHandleAccountLogin);

	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetPlayerInfo, this, &NFCPlayerControllerModule::OnHandleGetPlayerInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_ReConnect, this, &NFCPlayerControllerModule::OnHandleReConnect);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_KitPlayer, this, &NFCPlayerControllerModule::OnHandleKitPlayer);

	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetInitInfo, this, &NFCPlayerControllerModule::OnHandleGetInitInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_BroadCast, this, &NFCPlayerControllerModule::OnHandleBroadCast);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailList, this, &NFCPlayerControllerModule::OnHandleGetMailList);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetReadMail, this, &NFCPlayerControllerModule::OnHandleGetReadMail);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailGoods, this, &NFCPlayerControllerModule::OnHandleGetMailGoods);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_DeleteMail, this, &NFCPlayerControllerModule::OnHandleDeleteMail);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_PlayerStatus, this, &NFCPlayerControllerModule::OnHandlePlayerStatus);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdatePlayerInfo, this, &NFCPlayerControllerModule::OnHandleUpdatePlayerInfo);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdateGoodsList, this, &NFCPlayerControllerModule::OnHandleUpdateGoodsList);
	AddNetServerReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_NoticeInfo, this, &NFCPlayerControllerModule::OnHandleNoticeInfo);

	this->Subscribe(NFEVENT_MYSQL_UPDATE_MESSAGE, 0, NF_MYSQL_LOGIN_EVENT, __FUNCTION__);
	return true;
}

bool NFCPlayerControllerModule::DynamicLoadPlugin()
{
	m_pPlayerModule = m_pPluginManager->FindModule<NFIPlayerModule>();
	return true;
}

void NFCPlayerControllerModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message)
{
	if (nEventID == NFEVENT_MYSQL_UPDATE_MESSAGE)
	{
		if (bySrcType == NF_MYSQL_LOGIN_EVENT)
		{
			NFLogInfo(NF_LOG_SYSTEMLOG, 0, "player asyn login");
		}
	}
}

bool NFCPlayerControllerModule::Shut()
{
	return true;
}

/**
* @brief �����ʺŵ�¼����
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleAccountLogin(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	uint32_t proxyServerId = (uint32_t)playerId;
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
	NFMsg::playerinfo* pInfo = gcMsg.mutable_pinfo();

	uint32_t ret = 0;
	NFIObject* pPlayerObject = m_pPlayerModule->GetPlayerInfoByCID(cgMsg.account(), cgMsg.password(), ret);
	bool isNewPlayer = false;

	if (ret == RETURN_CODE_ACCOUNT_NO_EXIST || pPlayerObject == nullptr)
	{
		m_pPlayerModule->CreatePlayer(cgMsg);

		ret = 0;
		pPlayerObject = m_pPlayerModule->GetPlayerInfoByCID(cgMsg.account(), cgMsg.password(), ret);

		if (ret == RETURN_CODE_ACCOUNT_NO_EXIST || pPlayerObject == nullptr)
		{
			gcMsg.set_result(RETURN_CODE_ACCOUNT_NO_EXIST);
			m_pNetClientModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
			NFBehaviorLog(playerId, cgMsg.cid(), "login", "AccountLogin", RETURN_CODE_ACCOUNT_NO_EXIST, "������ɫʧ�ܣ�account=" + cgMsg.account() + ",cid=" + cgMsg.cid());
			return;
		}

		isNewPlayer = true;
	}
	else if (ret == RETURN_CODE_PASSWORD_NOT_MATCH)
	{
		gcMsg.set_result(RETURN_CODE_PASSWORD_NOT_MATCH);
		m_pNetClientModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
		NFBehaviorLog(playerId, cgMsg.cid(), "login", "AccountLogin", RETURN_CODE_PASSWORD_NOT_MATCH, "����ʧ�ܣ����벻ƥ��=" + cgMsg.account() + ",password=" + cgMsg.password());
		return;
	}

	pPlayerObject->SetNodeBool(NF_PLAYER_NODE_BOOL_ONLINE, true);
	pPlayerObject->SetNodeInt64(NF_PLAYER_NODE_UINT64_LOGINTIME, NFGetSecondTime());
	pPlayerObject->SetNodeInt64(NF_PLAYER_NODE_UINT64_LOGOUTTIME, 0);

	if (pPlayerObject->GetNodeUInt32(NF_PLAYER_NODE_UINT32_BLACKLIST) != 0)
	{
		pPlayerObject->SetNodeInt64(NF_PLAYER_NODE_UINT64_LOGOUTTIME, NFGetSecondTime());
		pPlayerObject->SetNodeBool(NF_PLAYER_NODE_BOOL_ONLINE, false);
		gcMsg.set_result(RETURN_CODE_ACCOUNT_LOGIN_ERROR);
		m_pNetClientModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
		return;
	}

	CopyFromPlayerObject(pInfo, pPlayerObject);

	gcMsg.set_result(RETURN_CODE_SUCCESS);
	m_pNetClientModule->SendToServerByPB(unLinkId, ::NFMsg::Server_Msg_AccountLogin, gcMsg, 0);
	NFBehaviorLog(pInfo->userid(), cgMsg.cid(), "login", "AccountLogin", RETURN_CODE_SUCCESS, "��ҵ�����Ϸ");
	return;
}

/**
* @brief �����ȡ�����Ϣ
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleGetPlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ������������
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����޳����
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleKitPlayer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ȡ��ʼ����Ϣ
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleGetInitInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����㲥
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleBroadCast(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ʼ��б�
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleGetMailList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����Ķ��ʼ�
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleGetReadMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �����ȡ�ʼ���Ʒ
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleGetMailGoods(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����ɾ���ʼ�
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleDeleteMail(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief �������״̬
*
* @return void
*/
void NFCPlayerControllerModule::OnHandlePlayerStatus(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ������������Ϣ
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleUpdatePlayerInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ���������Ʒ��Ϣ
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleUpdateGoodsList(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

/**
* @brief ����֪ͨ����
*
* @return void
*/
void NFCPlayerControllerModule::OnHandleNoticeInfo(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

void NFCPlayerControllerModule::CopyFromPlayerObject(NFMsg::playerinfo* pInfo, NFIObject* pPlayerObject)
{
	if (pInfo == nullptr || pPlayerObject == nullptr) return;

	NFProtobufCommon::NFObjectToMessage(pPlayerObject, *pInfo);
}