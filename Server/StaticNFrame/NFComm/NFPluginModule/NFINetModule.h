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
#include "NFComm/NFPluginModule/NFProtobufCommon.h"

#include "google/protobuf/message.h"

#define MAX_CLIENT_MASK 0x00ffffff					//客户端掩码16777215
#define MAX_SERVER_TYPE_MASK 0x7f000000					//客户端掩码16777215
#define MAX_IS_SERVER_MASK 0x80000000					//客户端掩码16777215
#define MAX_SERVER_TYPE_CLIENT_COUNT 0x0000ffff		//客户端实际数目65535

#define NF_IS_SERVER 1
#define NF_IS_CLIENT 0

#define GetUnLinkId(isServer, serverType, serverIndex)	(((serverIndex) & MAX_CLIENT_MASK) | (((serverType) << 24) & MAX_SERVER_TYPE_MASK) | (((isServer << 31) & MAX_IS_SERVER_MASK)));
#define GetServerTypeFromUnlinkId(UnlinkId)		(((UnlinkId) & MAX_SERVER_TYPE_MASK) >> 24);
#define GetIsServerFromUnlinkId(UnlinkId)		(((UnlinkId) & MAX_IS_SERVER_MASK) >> 31);
#define GetServerIndexFromUnlinkId(UnlinkId)	((UnlinkId) & MAX_CLIENT_MASK);

#define CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, nValueId, msg, nLen, xMsg)                 \
	if (!xMsg.ParseFromArray(msg, nLen))				\
    {													\
		NFLogError(NF_LOG_PROTOBUF_PARSE, 0, "Protobuf Parse Message Failed, msgId:{}, nLen:{}", nMsgId, nLen); \
        return ;										\
    }\
	if (NFLogDebugEnable(NF_LOG_RECV_MSG_JSON_PRINTF, nValueId))\
	{\
		NFLogDebug(NF_LOG_RECV_MSG_JSON_PRINTF, nValueId, "recv msg:{}, json:{}", nMsgId, xMsg.DebugString()); \
	}\



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
		mxCallBack.clear();
	}

	template <typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t opreateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);

		return AddReceiveCallBack(eType, nMsgID, functor);
	}

	template <typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t opreateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);

		return AddReceiveCallBack(eType, functor);
	}

	template <typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, const uint16_t nMainMsgId, const uint16_t nSubMsgId, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t opreateId, const uint16_t nMainMsgId, const uint16_t nSubMsgId, const char* msg, const uint32_t nLen))
	{
		NET_RECEIVE_MAINSUB_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7);

		return AddReceiveCallBack(eType, nMainMsgId, nSubMsgId, functor);
	}

	template <typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t opreateId, const uint16_t nMainMsgId, const uint16_t nSubMsgId, const char* msg, const uint32_t nLen))
	{
		NET_RECEIVE_MAINSUB_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7);

		return AddReceiveCallBack(eType, functor);
	}

	bool DelReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID)
	{
		if (eType < mxCallBack.size())
		{
			mxCallBack[eType].mxReceiveCallBack.erase(nMsgID);
			return true;
		}
		return false;
	}

	virtual bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint32_t nMsgID, const NET_RECEIVE_FUNCTOR& cb)
	{
		if (eType < mxCallBack.size())
		{
			mxCallBack[eType].mxReceiveCallBack[nMsgID].push_back(cb);
			return true;
		}
		return false;
	}

	virtual bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const NET_RECEIVE_FUNCTOR& cb)
	{
		if (eType < mxCallBack.size())
		{
			mxCallBack[eType].mxCallBackList.push_back(cb);
			return true;
		}
		return false;
	}

	virtual bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMainMsgId, const uint16_t nSubMsgId, const NET_RECEIVE_MAINSUB_FUNCTOR& cb)
	{
		if (eType < mxCallBack.size())
		{
			mxCallBack[eType].mxReceiveMainSubCallBack[nMainMsgId][nSubMsgId].push_back(cb);
			return true;
		}
		return false;
	}

	virtual bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const NET_RECEIVE_MAINSUB_FUNCTOR& cb)
	{
		if (eType < mxCallBack.size())
		{
			mxCallBack[eType].mxMainSubCallBackList.push_back(cb);
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
		if (eType < mxCallBack.size())
		{
			mxCallBack[eType].mxEventCallBack.push_back(cb);
			return true;
		}
		return false;
	}

	void OnReceiveNetPack(const uint32_t unLinkId, const uint64_t valueId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
	{
		uint32_t eServerType = GetServerTypeFromUnlinkId(unLinkId);
		if (eServerType < mxCallBack.size())
		{
			auto it = mxCallBack[eServerType].mxReceiveCallBack.find(nMsgId);
			if (it != mxCallBack[eServerType].mxReceiveCallBack.end())
			{
				NFLogDebug(NF_LOG_RECV_MSG, valueId, "recv msg:{}", nMsgId);
				std::vector<NET_RECEIVE_FUNCTOR>& pVecFun = it->second;
				for (size_t i = 0; i < pVecFun.size(); i++)
				{
					NET_RECEIVE_FUNCTOR& pFun = pVecFun[i];
					pFun(unLinkId, valueId, operateId, nMsgId, msg, nLen);
				}
			}
			else
			{		
				for (auto iterator = mxCallBack[eServerType].mxCallBackList.begin(); iterator != mxCallBack[eServerType].mxCallBackList.end(); ++iterator)
				{
					NFLogDebug(NF_LOG_RECV_MSG, valueId, "recv msg:{}", nMsgId);
					NET_RECEIVE_FUNCTOR& pFun = *iterator;
					pFun(unLinkId, valueId, operateId, nMsgId, msg, nLen);
				}
			}

			uint16_t mainMsgId = HIGH_UINT16(nMsgId);
			uint16_t subMsgId = LOW_UINT16(nMsgId);
			bool flag = true;
			auto mainmsg_it = mxCallBack[eServerType].mxReceiveMainSubCallBack.find(mainMsgId);
			if (mainmsg_it != mxCallBack[eServerType].mxReceiveMainSubCallBack.end())
			{
				auto submsg_it = mainmsg_it->second.find(subMsgId);
				if (submsg_it != mainmsg_it->second.end())
				{
					flag = false;
					NFLogDebug(NF_LOG_RECV_MSG, valueId, "recv msg, mainmsgId:{} submsgId:{}", mainMsgId, subMsgId);
					std::vector<NET_RECEIVE_MAINSUB_FUNCTOR>& pVecFun = submsg_it->second;
					for (size_t i = 0; i < pVecFun.size(); i++)
					{
						NET_RECEIVE_MAINSUB_FUNCTOR& pFun = pVecFun[i];
						pFun(unLinkId, valueId, operateId, mainMsgId, subMsgId, msg, nLen);
					}
				}
			}

			if (flag)
			{
				for (auto iterator = mxCallBack[eServerType].mxMainSubCallBackList.begin(); iterator != mxCallBack[eServerType].mxMainSubCallBackList.end(); ++iterator)
				{
					NFLogDebug(NF_LOG_RECV_MSG, valueId, "recv msg, mainmsgId:{} submsgId:{}", mainMsgId, subMsgId);
					NET_RECEIVE_MAINSUB_FUNCTOR& pFun = *iterator;
					pFun(unLinkId, valueId, operateId, mainMsgId, subMsgId, msg, nLen);
				}
			}
		}
	}

	void OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
	{
		uint32_t eServerType = GetServerTypeFromUnlinkId(unLinkId);
		if (eServerType < mxCallBack.size())
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
		std::unordered_map<uint32_t, std::vector<NET_RECEIVE_FUNCTOR>> mxReceiveCallBack;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::vector<NET_RECEIVE_MAINSUB_FUNCTOR>>> mxReceiveMainSubCallBack;
		std::vector<NET_EVENT_FUNCTOR> mxEventCallBack;
		std::vector<NET_RECEIVE_FUNCTOR> mxCallBackList;
		std::vector<NET_RECEIVE_MAINSUB_FUNCTOR> mxMainSubCallBackList;
	};

	std::vector<CallBack> mxCallBack;
};

