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

NFCHttpServerModule::NFCHttpServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pHttpServer = NULL;
}

NFCHttpServerModule::~NFCHttpServerModule()
{

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
	mComMsgCBList.clear();
	return true;
}

bool NFCHttpServerModule::Shut()
{
	if (m_pHttpServer)
	{
		m_pHttpServer->Final();
		delete m_pHttpServer;
		m_pHttpServer = NULL;
	}
	return true;
}

bool NFCHttpServerModule::Finalize()
{
	return true;
}

int NFCHttpServerModule::InitServer(const unsigned short nPort)
{
	m_pHttpServer = new NFCHttpServer(this, &NFCHttpServerModule::OnReceiveNetPack);
	std::cout << "Open http port:" << nPort << std::endl;
	return m_pHttpServer->InitServer(nPort);
}

void NFCHttpServerModule::OnReceiveNetPack(const NFHttpRequest& req, const std::string& strCommand,
	const std::string& strUrl)
{
	auto it = mMsgCBMap.find(strCommand);
	if (mMsgCBMap.end() != it)
	{
		HTTP_RECEIVE_FUNCTOR& pFunPtr = it->second;
		pFunPtr(req, strCommand, strUrl);
	}
	else
	{
		if (mComMsgCBList.size() > 0)
		{
			for (std::list<HTTP_RECEIVE_FUNCTOR>::iterator it = mComMsgCBList.begin();
			it != mComMsgCBList.end(); ++it)
			{
				HTTP_RECEIVE_FUNCTOR& pFunPtr = *it;
				pFunPtr(req, strCommand, strUrl);
			}
		}
		else
		{
			std::cout << "no http receiver:" << strCommand << std::endl;
		}
	}
}

bool NFCHttpServerModule::AddMsgCB(const std::string& strCommand, const HTTP_RECEIVE_FUNCTOR& cb)
{
	if (mMsgCBMap.find(strCommand) == mMsgCBMap.end())
	{
		mMsgCBMap.insert(std::map<std::string, HTTP_RECEIVE_FUNCTOR>::value_type(strCommand, cb));
		return true;
	}
	return false;
}

bool NFCHttpServerModule::AddComMsgCB(const HTTP_RECEIVE_FUNCTOR& cb)
{
	mComMsgCBList.push_back(cb);
	return true;
}

bool NFCHttpServerModule::ResponseMsg(const NFHttpRequest& req, const int nCommand, const std::string& strMsg)
{
	return false;
}

bool NFCHttpServerModule::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	return m_pHttpServer->ResponseMsg(req, strMsg, code, strReason);
}

bool
NFCHttpServerModule::ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName)
{
	return m_pHttpServer->ResponseFile(req, strPath, strFileName);
}