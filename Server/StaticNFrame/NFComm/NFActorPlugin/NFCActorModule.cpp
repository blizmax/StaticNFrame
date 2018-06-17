// -------------------------------------------------------------------------
//    @FileName         :    NFCActorModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorModule
//
// -------------------------------------------------------------------------

#include "NFCActorModule.h"
#include "NFComm/NFCore/NFProfiler.h"
#include "NFCActor.h"

NFCActorModule::NFCActorModule(NFIPluginManager* p)
{
	pPluginManager = p;

	srand(static_cast<unsigned>(time(nullptr)));

	m_pFramework = NF_NEW Theron::Framework(std::thread::hardware_concurrency());

	m_pMainActor = NF_NEW NFCActor(*m_pFramework, this);
}

NFCActorModule::~NFCActorModule()
{
	delete m_pMainActor;
	m_pMainActor = nullptr;
	delete m_pFramework;
	m_pFramework = nullptr;
}

bool NFCActorModule::Init()
{
	return true;
}

bool NFCActorModule::AfterInit()
{
	return true;
}

bool NFCActorModule::BeforeShut()
{
	return true;
}

bool NFCActorModule::Shut()
{
	return true;
}

bool NFCActorModule::Finalize()
{
	for (auto it = mxActorMap.begin(); it != mxActorMap.end(); ++it)
	{
		NF_SAFE_DELETE(it->second);
	}
	mxActorMap.clear();
	return true;
}

bool NFCActorModule::Execute()
{
	return ExecuteEvent();
}

int NFCActorModule::RequireActor()
{
	if (m_pFramework)
	{
		NFIActor* pActor(NF_NEW NFCActor(*m_pFramework, this));
		mxActorMap.emplace(pActor->GetAddress().AsInteger(), pActor);
		return pActor->GetAddress().AsInteger();
	}
	return 0;
}

NFIActor* NFCActorModule::GetActor(const int nActorIndex)
{
	auto it = mxActorMap.find(nActorIndex);
	if (it == mxActorMap.end())
	{
		return nullptr;
	}
	return it->second;
}

bool NFCActorModule::HandlerEx(const NFIActorMessage& message, const int from)
{
	if (message.xEndFuncptr == nullptr)
	{
		return true; //如果返回执行函数不存在，就不继续了
	}

	if (message.msgType != NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT)
	{
		return mxQueue.Push(message);
	}

	return false;
}

bool NFCActorModule::ExecuteEvent()
{
	std::list<NFIActorMessage> xListMsg;
	bool bRet;
	bRet = mxQueue.Pop(xListMsg);
	if (bRet)
	{
		for (auto it = xListMsg.begin(); it != xListMsg.end(); ++it)
		{
			NFIActorMessage& xMsg = *it;
			if (xMsg.msgType != NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT && xMsg.xEndFuncptr != nullptr)
			{
				xMsg.xEndFuncptr(xMsg.self, xMsg.nFromActor, xMsg.nMsgID, xMsg.data);
			}
		}
	}

	return true;
}

bool NFCActorModule::SendMsgToActor(const int nActorIndex, uint64_t objectID, const int nEventID, const std::string& strArg)
{
	NFIActor* pActor = GetActor(nActorIndex);
	if (nullptr != pActor)
	{
		NFIActorMessage xMessage;

		xMessage.msgType = NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT;
		xMessage.data = strArg;
		xMessage.nMsgID = nEventID;
		xMessage.self = objectID;

		if (m_pFramework && m_pMainActor)
		{
			xMessage.nFromActor = m_pMainActor->GetAddress().AsInteger();
			return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
		}
	}

	return false;
}

bool NFCActorModule::AddComponent(const int nActorIndex, NFIComponent* pComponent)
{
	NFIActor* pActor = GetActor(nActorIndex);
	if (nullptr != pActor)
	{
		pActor->AddComponent(pComponent);

		return true;
	}

	return false;
}

bool NFCActorModule::ReleaseActor(const int nActorIndex)
{
	auto it = mxActorMap.find(nActorIndex);
	if (it != mxActorMap.end())
	{
		NF_SAFE_DELETE(it->second);
		mxActorMap.erase(it);
		return true;
	}
	return false;
}

bool NFCActorModule::AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR functorPtr)
{
	NFIActor* pActor = GetActor(nActorIndex);
	if (nullptr != pActor)
	{
		return pActor->AddEndFunc(nSubMsgID, functorPtr);
	}

	return false;
}

