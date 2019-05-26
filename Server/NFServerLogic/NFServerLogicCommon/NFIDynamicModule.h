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
* @brief 用来实现动态加载的模块，所有可以动态加载的引擎的module都需要继承这个类
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

	virtual bool Finalize()
	{
		for (auto it = mNetServerMap.begin(); it != mNetServerMap.end(); it++)
		{
			FindModule<NFINetServerModule>()->DelReceiveCallBack((NF_SERVER_TYPES)it->first, it->second);
		}
		for (auto it = mNetClientMap.begin(); it != mNetClientMap.end(); it++)
		{
			FindModule<NFINetClientModule>()->DelReceiveCallBack((NF_SERVER_TYPES)it->first, it->second);
		}
		return true;
	}

	template <typename BaseType>
	bool AddNetServerReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mNetServerMap.emplace((uint32_t)eType, nMsgID);
		return FindModule<NFINetServerModule>()->AddReceiveCallBack(eType, nMsgID, pBase, handleRecieve);
	}

	template <typename BaseType>
	bool AddNetClientReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mNetClientMap.emplace((uint32_t)eType, nMsgID);
		return FindModule<NFINetClientModule>()->AddReceiveCallBack(eType, nMsgID, pBase, handleRecieve);
	}

	virtual void OnTimer(uint32_t nTimerID) override { }

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) override { }
protected:
	std::unordered_multimap<uint32_t, uint32_t> mNetServerMap;
	std::unordered_multimap<uint32_t, uint32_t> mNetClientMap;
};
