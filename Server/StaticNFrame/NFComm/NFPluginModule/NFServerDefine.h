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
	NF_ST_PROXY_INNER = 6, //���ض��ڷ�����, ������Ϸ��������������������������໥����
	NF_ST_MAX = 10, //
};

#define NF_JSON_MSG_ID 0

const std::string gArrayServer[NF_ST_MAX] = {
	"NoneServer",
	"MasterServer",
	"LoginServer",
	"WorldServer",
	"GameServer",
	"ProxyServer",
	"ProxyInnerServer",
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
	uint32_t GetGameId() { return mServerInfo.game_id(); }
	std::string GetGameName() { return mServerInfo.game_name(); }
	uint32_t GetZoneId() { return mServerInfo.server_id(); }
	std::function<uint32_t()> LuaGetGameId() { return [this]()->uint32_t { return mServerInfo.game_id(); }; }
	std::function<uint32_t()> LuaGetZoneId() { return [this]()->uint32_t { return mServerInfo.server_id(); }; }
	LUA_SEND_MSG_FUNCTION GetSendString() { return mSendString; }
	void SetSendString(LUA_SEND_MSG_FUNCTION cb) { mSendString = cb; }

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
	LUA_SEND_MSG_FUNCTION sendMsgFunc;

	void SetUnlinkId(uint32_t id) { unlinkId = id; }
	uint32_t GetUnlinkId() { return unlinkId; }

	void SetUid(uint64_t id) { uid = id; }
	uint64_t GetUid() { return uid; }

	void SetIp(const std::string& _ip) { ip = _ip; }
	std::string GetIp() { return ip; }

	void SetAccount(const std::string& a) { account = a; }
	std::string GetAccount() { return account; }

	void SetSendMsg(const LUA_SEND_MSG_FUNCTION& func) { sendMsgFunc = func; }
	LUA_SEND_MSG_FUNCTION GetSendMsg() { return sendMsgFunc; }
};

