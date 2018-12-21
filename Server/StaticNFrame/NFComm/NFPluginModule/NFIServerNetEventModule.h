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

class NFIServerNetEventModule : public NFIModule
{
public:
	template <typename BaseType>
	bool AddReceiveCallBack(NF_SERVER_TYPES eSourceType, NF_SERVER_TYPES eTargetType, void (BaseType::*handleRecieve)(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		return AddReceiveCallBack(eSourceType, eTargetType, functor);
	}

	virtual bool AddReceiveCallBack(NF_SERVER_TYPES eSourceType, NF_SERVER_TYPES eTargetType, const SERVER_NET_EVENT_FUNCTOR& cb)
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

	virtual void RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
	{

	}
protected:
	NFIServerNetEventModule()
	{
		mxCallBack.resize(NF_ST_MAX);
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

	std::vector<CallBack> mxCallBack;
};
