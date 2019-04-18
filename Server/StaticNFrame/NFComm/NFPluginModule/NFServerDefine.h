// -------------------------------------------------------------------------
//    @FileName         :    NFServerDefine.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include <stdint.h>
#include <functional>
#include <string>
#include "NFMessageDefine/NFMsgDefine.h"

enum NF_SERVER_TYPES
{
	NF_ST_NONE = 0, // NONE
	NF_ST_MASTER = 1, //
	NF_ST_LOGIN = 2, //
	NF_ST_WORLD = 3, //
	NF_ST_GAME = 4, //
	NF_ST_PROXY = 5, //
	NF_ST_MAX = 10, //
};

enum PacketParseType
{
	PACKET_PARSE_TYPE_NORMAL = 0,
	PACKET_PARSE_TYPE_QIPAI = 1,
};

#define NF_JSON_MSG_ID 0

const std::string gArrayServer[NF_ST_MAX] = {
	"NoneServer",
	"MasterServer",
	"LoginServer",
	"WorldServer",
	"GameServer",
	"ProxyServer",
};

std::string _NFExport GetServerName(NF_SERVER_TYPES serverId);

enum eMsgType
{
	eMsgType_Num = 0,
	eMsgType_CONNECTED = 1,
	eMsgType_DISCONNECTED = 2,
	eMsgType_RECIVEDATA = 3,
};

enum eAccountEventType
{
	eAccountEventType_Num = 0,
	eAccountEventType_CONNECTED = 1,
	eAccountEventType_DISCONNECTED = 2,
	eAccountEventType_RECONNECTED = 3,
};

typedef std::function<void(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)> NET_RECEIVE_FUNCTOR;

typedef std::function<void(const eMsgType nEvent, const uint32_t unLinkId)> NET_EVENT_FUNCTOR;

typedef std::function<void(int severity, const char* msg)> NET_EVENT_LOG_FUNCTOR;

typedef std::function<void(const std::string& msg)> LUA_SEND_MSG_FUNCTION;

//////////////////////////////////////////////////////////////////////////
class NFServerData
{
public:
	NFServerData()
	{
		mUnlinkId = 0;
	}

	~NFServerData()
	{
		mUnlinkId = 0;
	}

	uint32_t GetUnlinkId() { return mUnlinkId; }
	uint32_t GetServerId() { return mServerInfo.server_id(); }
	uint32_t GetServerType() { return mServerInfo.server_type(); }
	std::string GetServerName() { return mServerInfo.server_name(); }

	uint32_t mUnlinkId;
	NFMsg::ServerInfoReport mServerInfo;

	LUA_SEND_MSG_FUNCTION mSendString;
};

class AccountInfo
{
public:
	AccountInfo()
	{
		unlinkId = 0;
		uid = 0;
	}
	uint32_t unlinkId;
	uint64_t uid;
	std::string ip;
	std::string account;
};

class ProxyLinkInfo
{
public:
	ProxyLinkInfo()
	{
		mUnlinkId = 0;
		mPlayerId = 0;
		mIsLogin = false;
		mGameServerId = 0;
		mWorldServerId = 0;
		mGameServerUnlinkId = 0;
		mSendMsgCount = 0;
		mRecvMsgCount = 0;
	}

	uint32_t mUnlinkId;
	uint64_t mPlayerId;
	std::string mIPAddr;
	std::string mAccount;
	bool mIsLogin;
	uint32_t mGameServerUnlinkId;
	uint32_t mGameServerId;
	uint32_t mWorldServerId;
	uint32_t mSendMsgCount;
	uint32_t mRecvMsgCount;
};

/**
* @brief log��ӡ����
*
* @param  mLogId log����ID
* @param  mDisplay �Ƿ��ӡ���LOG
* @param  mLevel ����ȼ�
* @param  mLogName ��ʾ����
* @param  mGuid 0��ʾ��ӡ�������LOG�����ID��ʾֻ��ӡ�����ҵ�LOG
*/
class LogInfoConfig
{
public:
	LogInfoConfig()
	{
		mLogId = 0;
		mDisplay = false;
		mLevel = 0;
	}

	bool Exist(uint64_t guid)
	{
		for (size_t i = 0; i < mVecGuid.size(); i++)
		{
			if (mVecGuid[i] == guid)
			{
				return true;
			}
			return false;
		}
		return true;
	}

	uint32_t mLogId;
	bool mDisplay;
	uint32_t mLevel;
	std::string mLogName;
	std::vector<uint64_t> mVecGuid;
};

