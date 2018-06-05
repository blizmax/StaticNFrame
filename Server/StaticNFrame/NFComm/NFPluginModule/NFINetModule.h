// -------------------------------------------------------------------------
//    @FileName         :    NFINetModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFServerDefine.h"

#include <map>
#include <unordered_map>
#include <list>
#include <string>
#include <map>
#include <functional>
#include "NFComm/NFPluginModule/NFLogMgr.h"

#include "google/protobuf/message.h"

#define MAX_CLIENT_MASK 0x00ffffff					//客户端掩码16777215
#define MAX_SERVER_TYPE_CLIENT_COUNT 0x0000ffff		//客户端实际数目65535

#define GetUnLinkId(serverType, serverIndex)	(((serverIndex) & MAX_CLIENT_MASK) | (serverType) << 24);
#define GetServerTypeFromUnlinkId(UnlinkId)		((UnlinkId) >> 24);
#define GetServerIndexFromUnlinkId(UnlinkId)	((UnlinkId) & MAX_CLIENT_MASK);

#define CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msgData, nLen, msg)                 \
    msg xMsg;                                           \
	if (!xMsg.ParseFromArray(msgData, nLen))				\
    {													\
		NFLogNormalError(playerId, "Parse Message Failed:", lexical_cast<std::string>(nMsgID));	\
        return ;										\
    }													\

class NFINetModule : public NFIModule
{
protected:
	NFINetModule()
	{
		mxCallBack.resize(NF_ST_MAX);
	}

public:
	virtual ~NFINetModule()
	{
	}

	template <typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

		return AddReceiveCallBack(eType, nMsgID, functor);
	}

	template <typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

		return AddReceiveCallBack(eType, functor);
	}

	virtual bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, const NET_RECEIVE_FUNCTOR& cb)
	{
		if (eType >= 0 && eType < NF_ST_MAX)
		{
			mxCallBack[eType].mxReceiveCallBack.emplace(nMsgID, cb);
			return true;
		}
		return false;
	}

	virtual bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const NET_RECEIVE_FUNCTOR& cb)
	{
		if (eType >= 0 && eType < NF_ST_MAX)
		{
			mxCallBack[eType].mxCallBackList.push_back(cb);
			return true;
		}
		return false;
	}

	template <typename BaseType>
	bool AddEventCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handler)(const eMsgType nEvent, const uint32_t unLinkId))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		return AddEventCallBack(eType, functor);
	}

	virtual bool AddEventCallBack(const NF_SERVER_TYPES eType, const NET_EVENT_FUNCTOR& cb)
	{
		if (eType >= 0 && eType < NF_ST_MAX)
		{
			mxCallBack[eType].mxEventCallBack.push_back(cb);
			return true;
		}
		return false;
	}

	static bool ReceivePB(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen, std::string& strMsg, uint64_t& nPlayer)
	{
		strMsg.assign(msg, nLen);
		nPlayer = valueId;
		return true;
	}

	static bool ReceivePB(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen, google::protobuf::Message& xData, uint64_t& nPlayer)
	{
		if (!xData.ParseFromString(std::string(msg, nLen)))
		{
			return false;
		}

		nPlayer = valueId;

		return true;
	}

	void OnReceiveNetPack(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
	{
		uint32_t eServerType = GetServerTypeFromUnlinkId(unLinkId);
		if (eServerType < NF_ST_MAX)
		{
			auto it = mxCallBack[eServerType].mxReceiveCallBack.find(nMsgId);
			if (mxCallBack[eServerType].mxReceiveCallBack.end() != it)
			{
				NET_RECEIVE_FUNCTOR& pFun = it->second;
				pFun(unLinkId, valueId, nMsgId, msg, nLen);
			}
			else
			{
				for (auto iterator = mxCallBack[eServerType].mxCallBackList.begin(); iterator != mxCallBack[eServerType].mxCallBackList.end(); ++iterator)
				{
					NET_RECEIVE_FUNCTOR& pFun = *iterator;
					pFun(unLinkId, valueId, nMsgId, msg, nLen);
				}
			}
		}
	}

	void OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
	{
		uint32_t eServerType = GetServerTypeFromUnlinkId(unLinkId);
		if (eServerType < NF_ST_MAX)
		{
			for (auto it = mxCallBack[eServerType].mxEventCallBack.begin(); it != mxCallBack[eServerType].mxEventCallBack.end(); ++it)
			{
				NET_EVENT_FUNCTOR& pFun = *it;
				pFun(nEvent, unLinkId);
			}
		}
	}

protected:
	struct CallBack
	{
		//call back
		std::unordered_map<uint32_t, NET_RECEIVE_FUNCTOR> mxReceiveCallBack;
		std::vector<NET_EVENT_FUNCTOR> mxEventCallBack;
		std::vector<NET_RECEIVE_FUNCTOR> mxCallBackList;
	};

	std::vector<CallBack> mxCallBack;
};
