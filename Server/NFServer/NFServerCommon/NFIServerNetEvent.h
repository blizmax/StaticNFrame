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

#include "NFServerCommon.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

typedef std::function<void(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)> SERVER_NET_EVENT_FUNCTOR;

class NFIServerNetEvent : public NFIModule
{
public:
	template <typename BaseType>
	bool AddReceiveCallBack(NF_SERVER_TYPES eType, void (BaseType::*handleRecieve)(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		return AddReceiveCallBack(nEvent, unLinkId, functor);
	}

	virtual bool AddReceiveCallBack(NF_SERVER_TYPES eType, const SERVER_NET_EVENT_FUNCTOR& cb)
	{
		if (eType >= 0 && eType < NF_ST_MAX)
		{
			mxCallBack[eType].mxEventCallBack.push_back(cb);
			return true;
		}
		return false;
	}

	virtual bool AddEventLuaCallBack(const NF_SERVER_TYPES eType, const std::string& luaFunc)
	{
		if (eType >= 0 && eType < NF_ST_MAX)
		{
			mxCallBack[eType].mxEventLuaCallBack.push_back(luaFunc);
			return true;
		}
		return false;
	}

	void OnServerNetEvent(const eMsgType nEvent, const uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
	{
		uint32_t eServerType = GetServerTypeFromUnlinkId(unLinkId);
		if (eServerType < NF_ST_MAX)
		{
			for (auto it = mxCallBack[eServerType].mxEventCallBack.begin(); it != mxCallBack[eServerType].mxEventCallBack.end(); ++it)
			{
				SERVER_NET_EVENT_FUNCTOR& pFun = *it;
				if (pFun)
				{
					pFun(nEvent, unLinkId, pServerData);
				}
			}

			for (auto it = mxCallBack[eServerType].mxEventLuaCallBack.begin(); it != mxCallBack[eServerType].mxEventLuaCallBack.end(); ++it)
			{
				RunServerNetEventLuaFunc(*it, nEvent, unLinkId, pServerData);
			}
		}
	}

	virtual void RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
	{

	}
protected:
	NFIServerNetEvent()
	{
		mxCallBack.resize(NF_ST_MAX);
	}

	virtual ~NFIServerNetEvent()
	{
	}
protected:
	struct CallBack
	{
		//call back
		std::vector<SERVER_NET_EVENT_FUNCTOR> mxEventCallBack;
		std::vector<std::string> mxEventLuaCallBack;
	};

	std::vector<CallBack> mxCallBack;
};
