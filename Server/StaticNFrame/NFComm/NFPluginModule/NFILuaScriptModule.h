#pragma once

#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0
#include "Dependencies/LuaIntf/LuaIntf.h"
#include <vector>
#include <map>
#include <memory>

#include "NFITimerModule.h"
#include "NFServerDefine.h"

namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr);
}

typedef LuaIntf::LuaRef	NFLuaRef;
typedef LuaIntf::LuaTableRef NFLuaTableRef;

struct NFGUID;

class NFILuaScriptModule : public NFIModule
{
public:
	NFILuaScriptModule()
	{
	}
public:
	lua_State* GetLuaState()
	{
		return l.state();
	}
public:
	template <typename V = LuaIntf::LuaRef>
	V GetGlobal(const std::string& keyName) const
	{
		return l.getGlobal(keyName.c_str());
	}

	template<typename T>
	bool GetValue(const std::string& keyName, T& value) const
	{
		LuaIntf::LuaRef ref = GetGlobal(keyName);
		if (!ref.isValid())
		{
			return false;
		}

		try {
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
		try {
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
		try {
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
		try {
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
	bool TryRunGlobalScriptFunc(const std::string& strFuncName)
	{
		try {
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

	template<typename... Arg>
	bool TryRunGlobalScriptFunc(const std::string& strFuncName, Arg&&... args)
	{
		try {
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
	template<typename KEY, typename VALUE>
	bool GetLuaTableValue(const LuaIntf::LuaRef& table, const KEY& keyName, VALUE& value)
	{
		try {
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
protected:
	LuaIntf::LuaContext l;
};
