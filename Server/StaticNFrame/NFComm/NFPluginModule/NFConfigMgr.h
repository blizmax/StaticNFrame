// -------------------------------------------------------------------------
//    @FileName         :    NFConfigMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0
#include "Dependencies/LuaBind/luaintf/LuaIntf.h"

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFIPluginManager.h"
#include "NFServerDefine.h"

#include <unordered_map>
#include <vector>

namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr);
}

typedef LuaIntf::LuaRef NFLuaRef;
typedef LuaIntf::LuaTableRef NFLuaTableRef;

struct NFPluginConfig
{
	NFPluginConfig()
	{
		mServerType = NF_ST_NONE;
	}

public:
	std::vector<std::string> mVecPlugins;
	uint32_t mServerType;
};

struct NFServerConfig
{
public:
	NFServerConfig()
	{
		mServerType = NF_ST_NONE;
		mServerId = 0;
		mMasterId = 0;
		mWorldId = 0;
		mZoneId = 0;
		mServerPort = 0;
		mMaxConnectNum = 100;
		mWorkThreadNum = 1;
		mSecurity = false;
	}

public:
	std::string mServerName;
	uint32_t mServerType;
	uint32_t mServerId;
	uint32_t mMasterId;
	uint32_t mWorldId;
	uint32_t mZoneId;
	std::string mServerIp;
	uint32_t mServerPort;
	uint32_t mMaxConnectNum;
	uint32_t mWorkThreadNum;
	bool mSecurity;
};

class NFConfigMgr : public NFSingleton<NFConfigMgr>, public NFIModule
{
public:
	NFConfigMgr()
	{
	}

	virtual ~NFConfigMgr()
	{
	}
public:
	lua_State* GetLuaState() const
	{
		return l.state();
	}
public:
	template <typename V = LuaIntf::LuaRef>
	V GetGlobal(const std::string& keyName) const
	{
		return l.getGlobal(keyName.c_str());
	}

	template <typename T>
	bool GetValue(const std::string& keyName, T& value) const
	{
		LuaIntf::LuaRef ref = GetGlobal(keyName);
		if (!ref.isValid())
		{
			return false;
		}

		try
		{
			value = ref.toValue<T>();
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}

		return true;
	}

public:
	bool TryLoadScriptString(const std::string& strScript)
	{
		try
		{
			l.doString(strScript.c_str());
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}
		return false;
	}

	bool TryLoadScriptFile(const std::string& strFileName)
	{
		try
		{
			l.doFile(strFileName.c_str());
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}
		return false;
	}

	bool TryAddPackagePath(const std::string& strFilePath)
	{
		try
		{
			l.addPackagePath(strFilePath);
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}
		return false;
	}

public:
	bool TryRunGlobalScriptFunc(const std::string& strFuncName) const
	{
		try
		{
			LuaIntf::LuaRef func(l, strFuncName.c_str());
			func.call<LuaIntf::LuaRef>();
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}
		return false;
	}

	template <typename... Arg>
	bool TryRunGlobalScriptFunc(const std::string& strFuncName, Arg&&... args)
	{
		try
		{
			LuaIntf::LuaRef func(l, strFuncName.c_str());
			func.call<LuaIntf::LuaRef>(std::forward<Arg>(args)...);
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}
		return false;
	}

public:
	template <typename KEY, typename VALUE>
	bool GetLuaTableValue(const LuaIntf::LuaRef& table, const KEY& keyName, VALUE& value)
	{
		try
		{
			LuaIntf::LuaRef valueRef = table[keyName];
			if (!valueRef.isValid())
			{
				std::cout << "load lua table " << keyName << " failed!" << std::endl;
				return false;
			}

			value = valueRef.toValue<VALUE>();
			return true;
		}
		catch (LuaIntf::LuaException& e)
		{
			std::cout << e.what() << std::endl;
		}
		return false;
	}
public:
	int Init(NFIPluginManager* p)
	{
		pPluginManager = p;
		Init();
		return 0;
	}

	int UnInit()
	{
		Shut();
		return 0;
	}

	virtual bool Init() override;

	virtual bool Shut() override;

	NFPluginConfig* GetPluginConfig(const std::string& pluginName);
	NFServerConfig* GetServerConfig(uint32_t serverId);
	std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType);
protected:
	std::unordered_map<std::string, NFPluginConfig*> mPluginConfig; //pluginName--key
	std::unordered_map<int, NFServerConfig*> mServerConfig; //serverid--key
protected:
	LuaIntf::LuaContext l;
};

