// -------------------------------------------------------------------------
//    @FileName         :    NFCActorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_MANAGER_H
#define NFC_ACTOR_MANAGER_H

#include <map>
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFCore/NFQueue.hpp"

class NFCActorModule
	: public NFIActorModule
{
public:
	explicit NFCActorModule(NFIPluginManager* p);
	virtual ~NFCActorModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;

	virtual bool Execute() override;

	//////////////////////////////////////////////////////////////////////////////

	virtual int RequireActor() override;

	virtual bool SendMsgToActor(const int nActorIndex, uint64_t objectID, const int nEventID, const std::string& strArg) override;

	virtual bool HandlerEx(const NFIActorMessage& message, const int from) override;

	virtual bool ReleaseActor(const int nActorIndex) override;

	virtual NFIActor* GetActor(const int nActorIndex) override;

	///////////////////////////////////////////////////////////////////////////////////////////

	virtual bool AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR functorPtr) override;

	virtual bool AddComponent(const int nActorIndex, NFIComponent* pComponent) override;

	virtual bool ExecuteEvent();
private:

	Theron::Framework* m_pFramework;
	NFIActor* m_pMainActor;
	std::map<int, NFIActor*> mxActorMap;

	NFQueue<NFIActorMessage> mxQueue;
};

#endif

