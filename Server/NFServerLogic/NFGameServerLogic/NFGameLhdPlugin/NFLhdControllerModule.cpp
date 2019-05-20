// -------------------------------------------------------------------------
//    @FileName         :    NFHumanControllerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFLhdControllerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServerLogic/NFServerLogicCommon/NFLhdDefine.h"


NFCLhdControllerModule::NFCLhdControllerModule(NFIPluginManager* p) :NFILhdControllerModule(p)
{

}

NFCLhdControllerModule::~NFCLhdControllerModule()
{

}

bool NFCLhdControllerModule::Init()
{
	/**
	* @brief 绑定协议处理函数
	*/

	AddNetServerReceiveCallBack(NF_ST_GAME, NFMsg::Client_Msg_Lhd_CreateTable, this, &NFCLhdControllerModule::OnHandleCreateTable);
	return true;
}

/**
* @brief 处理创建桌子
*
* @return void
*/
void NFCLhdControllerModule::OnHandleCreateTable(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cglhdcreatetable cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NFMsg::gclhdcreatetable gcMsg;

	const NFMsg::lhd_tableconfiginfo& lhdConfigInfo = cgMsg.conf_info();

	NFIObject* pPlayerObject = m_pPlayerModule->GetPlayerObject(cgMsg.userid());

	if (pPlayerObject == nullptr)
	{
		NFLogError(NF_LOG_SYSTEMLOG, cgMsg.userid(), "Lhd Create Failed! player:{} is not exist!", cgMsg.userid());
		gcMsg.set_result(RETURN_CODE_PLAYER_NOT_EXIST);
	}
}

bool NFCLhdControllerModule::DynamicLoadPlugin()
{
	m_pPlayerModule = m_pPluginManager->FindModule<NFIPlayerModule>();
	return true;
}

bool NFCLhdControllerModule::Shut()
{
	return true;
}