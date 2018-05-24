// -------------------------------------------------------------------------
//    @FileName         :    NFCActor.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFCActor
//
// -------------------------------------------------------------------------

#include "NFCActor.h"

NFCActor::NFCActor(Theron::Framework& framework, NFIActorModule* pModule)
	: NFIActor(framework)
{
	m_pActorModule = pModule;
}

NFCActor::~NFCActor()
{
	NFIComponent* pCompnent;

	for (auto it = mxComponent.begin(); it != mxComponent.end(); ++it)
	{
		pCompnent = it->second;
		if (pCompnent)
		{
			pCompnent->SetEnable(false);
			pCompnent->BeforeShut();
			pCompnent->Shut();
			pCompnent->Finalize();
			delete pCompnent;
		}
	}

	mxComponent.clear();
	mxProcessFuntor.clear();
	mxEndProcessFuntor.clear();
}

void NFCActor::HandlerEx(const NFIActorMessage& message, const Theron::Address& from)
{
	if (m_pActorModule)
	{
		m_pActorModule->HandlerEx(message, from.AsInteger());
	}
}

void NFCActor::AddComponent(NFIComponent* pComponent)
{
	if (pComponent == nullptr) return;

	mxComponent.emplace(pComponent->GetComponentName(), pComponent);
	pComponent->SetActor(this);

	pComponent->Awake();
	pComponent->Init();
	pComponent->AfterInit();
	pComponent->ReadyExecute();
}

NFIComponent* NFCActor::FindComponent(const std::string& strComponentName)
{
	auto it = mxComponent.find(strComponentName);
	if (it == mxComponent.end())
	{
		return nullptr;
	}
	return it->second;
}

bool NFCActor::AddBeginFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR xBeginFunctor)
{
	if (mxProcessFuntor.find(nSubMsgID) != mxProcessFuntor.end())
	{
		return false;
	}

	mxProcessFuntor.emplace(nSubMsgID, xBeginFunctor);
	return true;
}

bool NFCActor::AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR xEndFunctor)
{
	if (mxEndProcessFuntor.find(nSubMsgID) != mxEndProcessFuntor.end())
	{
		return false;
	}

	if (nSubMsgID >= 0)
	{
		mxEndProcessFuntor.emplace(nSubMsgID, xEndFunctor);
	}
	else
	{
		mxDefaultEndProcessFuntor = xEndFunctor;
	}

	return true;
}

void NFCActor::Handler(const NFIActorMessage& message, const Theron::Address& from)
{
	std::string strData = message.data;

	auto it = mxProcessFuntor.find(message.nMsgID);
	if (it != mxProcessFuntor.end())
	{
		it->second(message.self, message.nFormActor, message.nMsgID, strData);
	}
	else
	{
		for (auto iterator = mxComponent.begin(); iterator != mxComponent.end(); ++iterator)
		{
			NFIComponent* pComponent = iterator->second;
			if (pComponent && pComponent->Enable())
			{
				pComponent->OnASyncEvent(message.self, message.nFormActor, message.nMsgID, strData);
			}
		}
	}

	////////////////////////////////////////////////////////
	// must return message
	NFIActorMessage xReturnMessage;

	auto iter = mxEndProcessFuntor.find(message.nMsgID);
	if (iter != mxEndProcessFuntor.end())
	{
		xReturnMessage.xEndFuncptr = iter->second;
	}
	else
	{
		//default end function
		if (mxDefaultEndProcessFuntor != nullptr)
		{
			xReturnMessage.xEndFuncptr = mxDefaultEndProcessFuntor;
		}
	}

	//如果返回函数不存在，就不再发消息给Main Actor, 减少后面的COPY
	if (xReturnMessage.xEndFuncptr == nullptr)
	{
		return;
	}

	xReturnMessage.msgType = NFIActorMessage::ACTOR_MSG_TYPE_END_FUNC;
	xReturnMessage.nMsgID = message.nMsgID;
	xReturnMessage.data = std::move(strData); //减少COPY
	xReturnMessage.self = message.self;
	xReturnMessage.nFormActor = this->GetAddress().AsInteger();

	Send(xReturnMessage, from);
}

bool NFCActor::SendMsg(const Theron::Address& address, const NFIActorMessage& message)
{
	return Send(message, address);
}

