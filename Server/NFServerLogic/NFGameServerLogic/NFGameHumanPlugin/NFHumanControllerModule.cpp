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
#include "NFServerLogic/NFServerLogicCommon/NFPacketHuman.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"
#include "NFComm/NFCore/NFRandom.hpp"

NFHumanControllerModule::NFHumanControllerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFHumanControllerModule::~NFHumanControllerModule()
{

}

bool NFHumanControllerModule::Init()
{
	NFINetServerModule* pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();

	/**
	* @brief ��Э�鴦����
	*/
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_AccountLogin, this, &NFHumanControllerModule::OnHandleAccountLogin);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetPlayerInfo, this, &NFHumanControllerModule::OnHandleGetPlayerInfo);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_ReConnect, this, &NFHumanControllerModule::OnHandleReConnect);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_KitPlayer, this, &NFHumanControllerModule::OnHandleKitPlayer);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_HeartBeat, this, &NFHumanControllerModule::OnHandleHeartBeat);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetInitInfo, this, &NFHumanControllerModule::OnHandleGetInitInfo);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_BroadCast, this, &NFHumanControllerModule::OnHandleBroadCast);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailList, this, &NFHumanControllerModule::OnHandleGetMailList);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetReadMail, this, &NFHumanControllerModule::OnHandleGetReadMail);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_GetMailGoods, this, &NFHumanControllerModule::OnHandleGetMailGoods);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_DeleteMail, this, &NFHumanControllerModule::OnHandleDeleteMail);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_PlayerStatus, this, &NFHumanControllerModule::OnHandlePlayerStatus);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdatePlayerInfo, this, &NFHumanControllerModule::OnHandleUpdatePlayerInfo);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_UpdateGoodsList, this, &NFHumanControllerModule::OnHandleUpdateGoodsList);
	pNetServerModule->AddReceiveCallBack(NF_ST_GAME, ::NFMsg::Client_Msg_NoticeInfo, this, &NFHumanControllerModule::OnHandleNoticeInfo);

	return true;
}

bool NFHumanControllerModule::Shut()
{
	return true;
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

		NFBehaviorLog(cid, cgMsg.nickname(), "login", "AccountLogin", 0, "�Զ����������CID=" + cid);
	}
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