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
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFCore/NFStringUtility.h"

NFCHttpServerModule::NFCHttpServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mServerArray.resize(NF_ST_MAX);
	for (int i = 0; i < NF_SERVER_TYPES::NF_ST_MAX; ++i)
	{
		mServerArray[i] = nullptr;
	}
	m_pLuaScriptModule = NULL;
	mxCallBack.resize(NF_ST_MAX);
}

NFCHttpServerModule::~NFCHttpServerModule()
{
	for (size_t i = 0; i < mServerArray.size(); i++)
	{
		if (mServerArray[i] != nullptr)
		{
			NF_SAFE_DELETE(mServerArray[i]);
		}
	}
	mServerArray.clear();
}

bool NFCHttpServerModule::Awake()
{
	//可以允许Lua Module不存在
	m_pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(m_pPluginManager->FindModule(typeid(NFILuaScriptModule).name()));
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
	for (size_t i = 0; i < mServerArray.size(); i++)
	{
		if (mServerArray[i] != nullptr)
		{
			mServerArray[i]->Execute();
		}
	}
	return true;
}

bool NFCHttpServerModule::BeforeShut()
{
	mxCallBack.clear();
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

int NFCHttpServerModule::InitServer(NF_SERVER_TYPES serverType, const std::string& portStr)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		if (mServerArray[serverType] != nullptr)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the serverType:{} has existing! Add Server Failed!", GetServerName(serverType));
			return 0;
		}

		NFCHttpServer* pHttpServer = new NFCHttpServer(serverType, this, &NFCHttpServerModule::OnReceiveNetPack, &NFCHttpServerModule::OnFilterPack);

		if (pHttpServer->InitServer(portStr) == 0)
		{
			mServerArray[serverType] = pHttpServer;
			return serverType;
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "Http Server Listen Port Failed, serverType:{}, port:{}", GetServerName(serverType), portStr);
		NF_SAFE_DELETE(pHttpServer);
		return 0;
	}
	return 0;
}

int NFCHttpServerModule::InitServer(NF_SERVER_TYPES serverType, std::vector<uint32_t> nPorts)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		if (mServerArray[serverType] != nullptr)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the serverType:{} has existing! Add Server Failed!", GetServerName(serverType));
			return 0;
		}

		NFCHttpServer* pHttpServer = new NFCHttpServer(serverType, this, &NFCHttpServerModule::OnReceiveNetPack, &NFCHttpServerModule::OnFilterPack);

		if (pHttpServer->InitServer(nPorts) == 0)
		{
			mServerArray[serverType] = pHttpServer;
			return serverType;
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "Http Server Listen Port Failed, serverType:{}, port:{}", GetServerName(serverType), NFCommon::tostr(nPorts));
		NF_SAFE_DELETE(pHttpServer);
		return 0;
	}
	return 0;
}

int NFCHttpServerModule::InitServer(NF_SERVER_TYPES serverType, uint32_t nPort)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		if (mServerArray[serverType] != nullptr)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the serverType:{} has existing! Add Server Failed!", GetServerName(serverType));
			return 0;
		}

		NFCHttpServer* pHttpServer = new NFCHttpServer(serverType, this, &NFCHttpServerModule::OnReceiveNetPack, &NFCHttpServerModule::OnFilterPack);

		if (pHttpServer->InitServer(nPort) == 0)
		{
			mServerArray[serverType] = pHttpServer;
			return serverType;
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "Http Server Listen Port Failed, serverType:{}, port:{}", GetServerName(serverType), nPort);
		NF_SAFE_DELETE(pHttpServer);
		return 0;
	}
	return 0;
}

bool NFCHttpServerModule::OnReceiveNetPack(uint32_t unlinkId, const NFHttpHandle& req)
{
	uint32_t serverType = unlinkId;
	if (serverType <= NF_ST_NONE || serverType >= NF_ST_MAX) return false;

	auto iter = mxCallBack[serverType].mMsgCBMap.find(req.type);
	if (iter != mxCallBack[serverType].mMsgCBMap.end())
	{
		std::string lowerPath = NFStringUtility::ToLower(req.path);
		auto itPath = iter->second.find(lowerPath);
		if (itPath != iter->second.end())
		{
			HTTP_RECEIVE_FUNCTOR& pFunPtr = itPath->second;
			try
			{
				pFunPtr(serverType,req);
			}
			catch (const std::exception&)
			{
				ResponseMsg((NF_SERVER_TYPES)serverType, req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
			}
			return true;
		}
		else
		{
			for (int i = 0; i < mxCallBack[serverType].mOtherMsgCBMap[req.type].size(); ++i)
			{
				HTTP_RECEIVE_FUNCTOR& pFunPtr = mxCallBack[serverType].mOtherMsgCBMap[req.type][i];
				try
				{
					pFunPtr(serverType, req);
				}
				catch (const std::exception&)
				{
					ResponseMsg((NF_SERVER_TYPES)serverType, req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
				}
				return true;
			}
		}
	}

	auto luaiter = mxCallBack[serverType].mMsgLuaCBMap.find(req.type);
	if (luaiter != mxCallBack[serverType].mMsgLuaCBMap.end())
	{
		std::string lowerPath = NFStringUtility::ToLower(req.path);
		auto luaitPath = luaiter->second.find(lowerPath);
		if (luaitPath != luaiter->second.end())
		{
			std::string& luaFunc = luaitPath->second;
			try
			{
				if (m_pLuaScriptModule)
				{
					m_pLuaScriptModule->RunHttpServerLuaFunc(luaFunc, serverType, req);
				}
			}
			catch (const std::exception&)
			{
				ResponseMsg((NF_SERVER_TYPES)serverType, req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
			}
			return true;
		}
		else
		{
			for (int i = 0; i < mxCallBack[serverType].mOtherMsgLuaCBMap[req.type].size(); ++i)
			{
				std::string& luaFunc = mxCallBack[serverType].mOtherMsgLuaCBMap[req.type][i];
				try
				{
					if (m_pLuaScriptModule)
					{
						m_pLuaScriptModule->RunHttpServerLuaFunc(luaFunc, serverType, req);
					}
				}
				catch (const std::exception&)
				{
					ResponseMsg((NF_SERVER_TYPES)serverType, req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
				}
			}
		}
	}

	return ResponseMsg((NF_SERVER_TYPES)serverType, req, "", NFWebStatus::WEB_ERROR);
}

NFWebStatus NFCHttpServerModule::OnFilterPack(uint32_t unlinkId, const NFHttpHandle & req)
{
	uint32_t serverType = unlinkId;
	if (serverType <= NF_ST_NONE || serverType >= NF_ST_MAX) return NFWebStatus::WEB_ERROR;

	std::string lowerPath = NFStringUtility::ToLower(req.path);
	auto itPath = mxCallBack[serverType].mMsgFliterMap.find(lowerPath);
	if (itPath != mxCallBack[serverType].mMsgFliterMap.end())
	{
		HTTP_FILTER_FUNCTOR& pFunPtr = itPath->second;
		return pFunPtr(serverType, req);
	}

	return NFWebStatus::WEB_OK;
}

bool NFCHttpServerModule::LuaAddMsgCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const NFHttpType eRequestType, const std::string& luaFunc)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		std::string lowerCmd = NFStringUtility::ToLower(strCommand);
		mxCallBack[serverType].mMsgLuaCBMap[eRequestType].emplace(lowerCmd, luaFunc);
	}

	return false;
}

bool NFCHttpServerModule::LuaAddOtherMsgCB(NF_SERVER_TYPES serverType, const NFHttpType eRequestType, const std::string& luaFunc)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		mxCallBack[serverType].mOtherMsgLuaCBMap[eRequestType].push_back(luaFunc);
	}

	return false;
}

bool NFCHttpServerModule::AddMsgCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		std::string lowerCmd = NFStringUtility::ToLower(strCommand);
		mxCallBack[serverType].mMsgCBMap[eRequestType].emplace(lowerCmd, cb);
	}

	return false;
}

bool NFCHttpServerModule::AddOtherMsgCB(NF_SERVER_TYPES serverType, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR& cb)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		mxCallBack[serverType].mOtherMsgCBMap[eRequestType].push_back(cb);
	}

	return false;
}

bool NFCHttpServerModule::AddFilterCB(NF_SERVER_TYPES serverType, const std::string& strCommand, const HTTP_FILTER_FUNCTOR& cb)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		std::string lowerCmd = NFStringUtility::ToLower(strCommand);
		mxCallBack[serverType].mMsgFliterMap.emplace(lowerCmd, cb);
	}

	return true;
}

bool NFCHttpServerModule::ResponseMsg(NF_SERVER_TYPES serverType, const NFHttpHandle& req, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		if (mServerArray[serverType] != nullptr)
		{
			return mServerArray[serverType]->ResponseMsg(req, strMsg, code, strReason);
		}
	}
	return false;
}

bool NFCHttpServerModule::ResponseMsg(NF_SERVER_TYPES serverType, uint64_t requestId, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	if (serverType > NF_ST_NONE && serverType < NF_ST_MAX)
	{
		if (mServerArray[serverType] != nullptr)
		{
			return mServerArray[serverType]->ResponseMsg(requestId, strMsg, code, strReason);
		}
	}
	return false;
}