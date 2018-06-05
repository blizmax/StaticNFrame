// -------------------------------------------------------------------------
//    @FileName         :    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_H
#define NFC_ACTOR_H

#include <map>
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCActor
	: public NFIActor
{
public:
	NFCActor(Theron::Framework& framework, NFIActorModule* pModule);
	virtual ~NFCActor();

	virtual void AddComponent(NFIComponent* pComponent) override;
	virtual NFIComponent* FindComponent(const std::string& strComponentName) override;

	virtual bool AddBeginFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR xBeginFunctor) override;
	virtual bool AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR xEndFunctor) override;

	virtual bool SendMsg(const Theron::Address& address, const NFIActorMessage& message) override;

protected:
	//handler in component
	virtual void Handler(const NFIActorMessage& message, const Theron::Address& from) override;

	//the purpose is to push message to main thread
	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address& from) override;

private:
	std::map<std::string, NFIComponent*> mxComponent;

	std::map<int, ACTOR_PROCESS_FUNCTOR> mxProcessFuntor;
	std::map<int, ACTOR_PROCESS_FUNCTOR> mxEndProcessFuntor;

	ACTOR_PROCESS_FUNCTOR mxDefaultEndProcessFuntor;
};
#endif
