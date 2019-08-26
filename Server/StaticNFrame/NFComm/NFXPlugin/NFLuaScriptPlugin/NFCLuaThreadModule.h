// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaThreadModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/08/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFILuaThreadModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILuaModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

#include <vector>
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFTask.h"

class NFCLuaThreadModule : public NFILuaScriptModule
{
public:
	NFCLuaThreadModule(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFCLuaThreadModule()
	{

	}
public:
	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg) override;
	virtual void RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId) override;
	virtual void RunHtttpClientLuaFunc(const std::string& luaFunc, const int state_code, const std::string& strRespData, const std::string& strUserData) override;
	virtual void RunHttpServerLuaFunc(const std::string& luaFunc, uint32_t serverType, const NFIHttpHandle & req) override;
	virtual void RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData) override;
	virtual void RunAccountNetEventLuaFunc(const std::string& luaFunc, uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData) override;
	virtual void RunHttpRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg) override;
public:
	/**
* @brief ���һ��Actor���
*
* @return
*/
	virtual bool AddActorComponent(NFITaskComponent* pComonnet)
	{
		int actorId = FindModule<NFITaskModule>()->RequireActor();
		if (actorId <= 0)
		{
			return false;
		}

		FindModule<NFITaskModule>()->AddActorComponent(actorId, pComonnet);

		m_vecActorPool.push_back(actorId);
		return true;
	}

	/**
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceActor(uint64_t balanceId)
	{
		if (balanceId == 0)
		{
			return GetRandActor();
		}
		else
		{
			if (m_vecActorPool.size() <= 0)
			{
				return -1;
			}
			mnSuitIndex = balanceId % m_vecActorPool.size();
			return m_vecActorPool[mnSuitIndex];
		}
	}

	/**
	* @brief ������һ��actor
	*
	* @return actor����
	*/
	int GetRandActor()
	{
		if (m_vecActorPool.size() <= 0)
		{
			return -1;
		}

		mnSuitIndex++;
		mnSuitIndex = mnSuitIndex % m_vecActorPool.size();

		return m_vecActorPool[mnSuitIndex];
	}

	/**
	* @brief ͨ��ƽ��ID���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddTask(NFTask* pTask)
	{
		if (pTask)
		{
			int actorId = GetBalanceActor(pTask->GetBalanceId());
			if (actorId > 0)
			{
				return FindModule<NFITaskModule>()->AddTask(actorId, pTask);
			}
		}

		return false;
	}

protected:
	/**
	* @brief actor��������
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	size_t mnSuitIndex = 0;
};