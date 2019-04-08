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
#include "NFComm/NFPluginModule/NFIPluginManager.h"

/**
* @brief 用来实现动态加载的模块，所有可以动态加载的引擎的module都需要继承这个类
*/
class NFIDynamicModule : public NFIModule
{
public:
	NFIDynamicModule(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFIDynamicModule()
	{

	}

	virtual bool Awake() final
	{
		NFINetServerModule* pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
		return true;
	}

	virtual bool Finalize() final
	{
		NFINetServerModule* pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
		return true;
	}

	template <typename BaseType>
	bool AddNetServerReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		NFINetServerModule* pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
		return pNetServerModule->AddReceiveCallBack(eType, pBase, handleRecieve);
	}
protected:
	std::unordered_map<uint32_t, uint32_t> mNetServerMap;
};
