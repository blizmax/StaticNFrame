// -------------------------------------------------------------------------
//    @FileName         :    NFHttpServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFCHttpServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFCHttpServer.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

NFCHttpServerModule::NFCHttpServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pHttpServer = NULL;
	m_pLuaScriptModule = NULL;
}

NFCHttpServerModule::~NFCHttpServerModule()
{
	if (m_pHttpServer)
	{
		delete m_pHttpServer;
		m_pHttpServer = NULL;
	}
}

bool NFCHttpServerModule::Awake()
{
	//可以允许Lua Module不存在
	m_pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pPluginManager->FindModule(typeid(NFILuaScriptModule).name()));
	return true;
}

bool NFCHttpServerModule::Init()
{
	return true;
}

bool NFCHttpServerModule::AfterInit()
{
	return true;
}



bool NFCHttpServerModule::Execute()
{
	if (m_pHttpServer)
	{
		m_pHttpServer->Execute();
	}

	return true;
}

bool NFCHttpServerModule::BeforeShut()
{
	mMsgCBMap.clear();
	mMsgFliterMap.clear();
	return true;
}

bool NFCHttpServerModule::Shut()
{
	return true;
}

bool NFCHttpServerModule::Finalize()
{
	return true;
}

int NFCHttpServerModule::InitServer(const unsigned short nPort)
{
	m_pHttpServer = new NFCHttpServer(this, &NFCHttpServerModule::OnReceiveNetPack, &NFCHttpServerModule::OnFilterPack);
	//NFLogDebug("Http Server Open http port:{}", nPort);
	return m_pHttpServer->InitServer(nPort);
}

bool NFCHttpServerModule::OnReceiveNetPack(const NFHttpRequest& req)
{
	auto iter = mMsgCBMap.find(req.type);
	if (iter != mMsgCBMap.end())
	{
		auto itPath = iter->second.find(req.path);
		if (itPath != iter->second.end())
		{
			HTTP_RECEIVE_FUNCTOR& pFunPtr = itPath->second;
			try
			{
				pFunPtr(req);
			}
			catch (const std::exception&)
			{
				ResponseMsg(req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
			}
			return true;
		}
	}

	auto luaiter = mMsgLuaCBMap.find(req.type);
	if (luaiter != mMsgLuaCBMap.end())
	{
		auto luaitPath = luaiter->second.find(req.path);
		if (luaitPath != luaiter->second.end())
		{
			std::string& luaFunc = luaitPath->second;
			try
			{
				if (m_pLuaScriptModule)
				{
					m_pLuaScriptModule->RunHttpServerLuaFunc(luaFunc, req);
				}
			}
			catch (const std::exception&)
			{
				ResponseMsg(req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
			}
			return true;
		}
	}

	return ResponseMsg(req, "", NFWebStatus::WEB_ERROR);
}

NFWebStatus NFCHttpServerModule::OnFilterPack(const NFHttpRequest & req)
{
	auto itPath = mMsgFliterMap.find(req.path);
	if (itPath != mMsgFliterMap.end())
	{
		HTTP_FILTER_FUNCTOR& pFunPtr = itPath->second;
		return pFunPtr(req);
	}

	return NFWebStatus::WEB_OK;
}

bool NFCHttpServerModule::LuaAddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const std::string& luaFunc)
{
	auto it = mMsgLuaCBMap.find(eRequestType);
	if (it == mMsgLuaCBMap.end())
	{
		mMsgLuaCBMap[eRequestType].emplace(strCommand, luaFunc);
	}

	return false;
}

bool NFCHttpServerModule::AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb)
{
	auto it = mMsgCBMap.find(eRequestType);
	if (it == mMsgCBMap.end())
	{
		mMsgCBMap[eRequestType].emplace(strCommand, cb);
	}

	return false;
}

bool NFCHttpServerModule::AddFilterCB(const std::string& strCommand, const HTTP_FILTER_FUNCTOR& cb)
{
	auto it = mMsgFliterMap.find(strCommand);
	if (it == mMsgFliterMap.end())
	{
		mMsgFliterMap.emplace(strCommand, cb);
	}

	return true;
}

bool NFCHttpServerModule::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	return m_pHttpServer->ResponseMsg(req, strMsg, code, strReason);
}