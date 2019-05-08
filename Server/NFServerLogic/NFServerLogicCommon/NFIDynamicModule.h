// -------------------------------------------------------------------------
//    @FileName         :    NFIDynamicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFIDynamicModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFINetServerModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"

/**
* @brief ����ʵ�ֶ�̬���ص�ģ�飬���п��Զ�̬���ص������module����Ҫ�̳������
*/
class NFIDynamicModule : public NFIModule, public NFTimerObj, public NFEventObj
{
public:
	NFIDynamicModule(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFIDynamicModule()
	{

	}

	virtual bool Awake()
	{
		m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
		m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
		DynamicLoadPlugin();
		return true;
	}

	/*
	** ���������Ҫ������������ָ��, ��̬���ص�ʱ������ָ����ܻ�ʧЧ 
	*/
	virtual bool DynamicLoadPlugin() = 0;

	virtual bool Finalize()
	{
		for (auto it = mNetServerMap.begin(); it != mNetServerMap.end(); it++)
		{
			m_pNetServerModule->DelReceiveCallBack((NF_SERVER_TYPES)it->first, it->second);
		}
		for (auto it = mNetClientMap.begin(); it != mNetClientMap.end(); it++)
		{
			m_pNetClientModule->DelReceiveCallBack((NF_SERVER_TYPES)it->first, it->second);
		}
		return true;
	}

	template <typename BaseType>
	bool AddNetServerReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mNetServerMap.emplace((uint32_t)eType, nMsgID);
		return m_pNetServerModule->AddReceiveCallBack(eType, nMsgID, pBase, handleRecieve);
	}

	template <typename BaseType>
	bool AddNetClientReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mNetClientMap.emplace((uint32_t)eType, nMsgID);
		return m_pNetClientModule->AddReceiveCallBack(eType, nMsgID, pBase, handleRecieve);
	}

	virtual void OnTimer(uint32_t nTimerID) override { }

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) override { }
protected:
	std::unordered_multimap<uint32_t, uint32_t> mNetServerMap;
	std::unordered_multimap<uint32_t, uint32_t> mNetClientMap;
	NFINetServerModule* m_pNetServerModule;
	NFINetClientModule* m_pNetClientModule;
};
