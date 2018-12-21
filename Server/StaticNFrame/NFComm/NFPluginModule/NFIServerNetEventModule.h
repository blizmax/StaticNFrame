// -------------------------------------------------------------------------
//    @FileName         :    NFIServerNetEvent.h
//    @Author           :    GaoYi
//    @Date             :    2018/12/20
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"

#include "NFServerDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

typedef std::function<void(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)> SERVER_NET_EVENT_FUNCTOR;
typedef std::function<void(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData)> ACCOUNT_NET_EVENT_FUNCTOR;

class NFIServerNetEventModule : public NFIModule
{
public:
	template <typename BaseType>
	bool AddEventCallBack(NF_SERVER_TYPES eSourceType, NF_SERVER_TYPES eTargetType, BaseType* pBase, void (BaseType::*handleRecieve)(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData))
	{
		SERVER_NET_EVENT_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		return AddEventCallBack(eSourceType, eTargetType, functor);
	}

	virtual bool AddEventCallBack(NF_SERVER_TYPES eSourceType, NF_SERVER_TYPES eTargetType, const SERVER_NET_EVENT_FUNCTOR& cb)
	{
		if (eSourceType  < NF_ST_MAX && eTargetType < NF_ST_MAX)
		{
			mxCallBack[eSourceType].mxEventCallBack[eTargetType].push_back(cb);
			return true;
		}
		return false;
	}

	virtual bool AddEventLuaCallBack(NF_SERVER_TYPES eSourceType, NF_SERVER_TYPES eTargetType, const std::string& luaFunc)
	{
		if (eSourceType  < NF_ST_MAX && eTargetType < NF_ST_MAX)
		{
			mxCallBack[eSourceType].mxEventLuaCallBack[eTargetType].push_back(luaFunc);
			return true;
		}
		return false;
	}

	template <typename BaseType>
	bool AddAccountEventCallBack(NF_SERVER_TYPES eServerType, BaseType* pBase, void (BaseType::*handleRecieve)(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData))
	{
		ACCOUNT_NET_EVENT_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		return AddAccountEventCallBack(eServerType, functor);
	}

	virtual bool AddAccountEventCallBack(NF_SERVER_TYPES eServerType, const ACCOUNT_NET_EVENT_FUNCTOR& cb)
	{
		if (eServerType < NF_ST_MAX)
		{
			mxAccountCallBack[eServerType].mxEventCallBack.push_back(cb);
			return true;
		}
		return false;
	}

	virtual bool AddAccountEventLuaCallBack(NF_SERVER_TYPES eServerType, const std::string& luaFunc)
	{
		if (eServerType < NF_ST_MAX)
		{
			mxAccountCallBack[eServerType].mxEventLuaCallBack.push_back(luaFunc);
			return true;
		}
		return false;
	}

	void OnServerNetEvent(eMsgType nEvent, NF_SERVER_TYPES eSourceType, NF_SERVER_TYPES eTargetType, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
	{
		if (eSourceType  < NF_ST_MAX && eTargetType < NF_ST_MAX)
		{
			for (auto it = mxCallBack[eSourceType].mxEventCallBack[eTargetType].begin(); it != mxCallBack[eSourceType].mxEventCallBack[eTargetType].end(); ++it)
			{
				SERVER_NET_EVENT_FUNCTOR& pFun = *it;
				if (pFun)
				{
					pFun(nEvent, unLinkId, pServerData);
				}
			}

			for (auto it = mxCallBack[eSourceType].mxEventLuaCallBack[eTargetType].begin(); it != mxCallBack[eSourceType].mxEventLuaCallBack[eTargetType].end(); ++it)
			{
				RunServerNetEventLuaFunc(*it, nEvent, unLinkId, pServerData);
			}
		}
	}

	void OnAccountNetEvent(uint32_t nEvent, NF_SERVER_TYPES eServerType, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData)
	{
		if (eServerType < NF_ST_MAX)
		{
			for (auto it = mxAccountCallBack[eServerType].mxEventCallBack.begin(); it != mxAccountCallBack[eServerType].mxEventCallBack.end(); ++it)
			{
				ACCOUNT_NET_EVENT_FUNCTOR& pFun = *it;
				if (pFun)
				{
					pFun(nEvent, unLinkId, pServerData);
				}
			}

			for (auto it = mxAccountCallBack[eServerType].mxEventLuaCallBack.begin(); it != mxAccountCallBack[eServerType].mxEventLuaCallBack.end(); ++it)
			{
				RunAccountNetEventLuaFunc(*it, nEvent, unLinkId, pServerData);
			}
		}
	}

	virtual void RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
	{

	}

	virtual void RunAccountNetEventLuaFunc(const std::string& luaFunc, uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData)
	{

	}
protected:
	NFIServerNetEventModule()
	{
		mxCallBack.resize(NF_ST_MAX);
		mxAccountCallBack.resize(NF_ST_MAX);
	}

	virtual ~NFIServerNetEventModule()
	{
	}
protected:
	struct CallBack
	{
		//call back
		std::map<uint32_t, std::vector<SERVER_NET_EVENT_FUNCTOR>> mxEventCallBack; //key -- servertype
		std::map<uint32_t, std::vector<std::string>> mxEventLuaCallBack; //key -- servertype
	};

	struct AccountCallBack
	{
		//call back
		std::vector<ACCOUNT_NET_EVENT_FUNCTOR> mxEventCallBack; //key -- servertype
		std::vector<std::string> mxEventLuaCallBack; //key -- servertype
	};

	std::vector<CallBack> mxCallBack;
	std::vector<AccountCallBack> mxAccountCallBack;
};
