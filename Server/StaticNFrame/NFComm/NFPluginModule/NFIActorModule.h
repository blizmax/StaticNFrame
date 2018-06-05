// -------------------------------------------------------------------------
//    @FileName         :    NFIActorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActorModule
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_MODULE_H
#define NFI_ACTOR_MODULE_H

#include "NFIModule.h"
#include "NFIActor.h"
#include "NFIComponent.h"

///////////////////////////////////////////////////

class NFIComponent;

class NFIActorModule : public NFIModule
{
public:
	template <typename TypeComponent>
	int RequireActor()
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return 0;
		}

		int nActorID = RequireActor();
		if (nActorID >= 0)
		{
			AddComponent<TypeComponent>(nActorID);

			return nActorID;
		}

		return -1;
	}

	template <typename TypeComponent, typename BaseType>
	int RequireActor(BaseType* pBase, int (BaseType::*handler_end)(uint64_t, const int, const int, const std::string&))
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return 0;
		}

		int nActorID = RequireActor();
		if (nActorID >= 0)
		{
			AddComponent<TypeComponent>(nActorID);
			AddDefaultEndFunc<BaseType>(nActorID, pBase, handler_end);

			return nActorID;
		}

		return -1;
	}

	template <typename TypeComponent>
	bool AddComponent(const int nActorIndex)
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return false;
		}

		NFIActor* pActor = GetActor(nActorIndex);
		if (pActor)
		{
			//use CreateNewInstance to replace this line to create a new component script
			TypeComponent* pComponent = NF_NEW TypeComponent();
			return AddComponent(nActorIndex, pComponent);
		}

		return false;
	}

	template <typename BaseType>
	int AddEndFunc(const int nActorIndex, const int nSubMessageID,
		BaseType* pBase, int (BaseType::*handler_end)(uint64_t, const int, const int, const std::string&))
	{
		if (pBase == nullptr || handler_end == nullptr)
		{
			return 0;
		}

		ACTOR_PROCESS_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		return AddEndFunc(nActorIndex, nSubMessageID, functor_end);
	}

	template <typename BaseType>
	int AddDefaultEndFunc(const int nActorIndex,
		BaseType* pBase, int (BaseType::*handler_end)(uint64_t, const int, const int, const std::string&))
	{
		if (pBase == nullptr || handler_end == nullptr)
		{
			return 0;
		}

		ACTOR_PROCESS_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		return AddEndFunc(nActorIndex, -1, functor_end);
	}

	virtual int RequireActor() = 0;
	virtual bool SendMsgToActor(const int nActorIndex, uint64_t objectID, const int nEventID, const std::string& strArg) = 0;
	virtual bool HandlerEx(const NFIActorMessage& message, const int from) = 0;
	virtual bool ReleaseActor(const int nActorIndex) = 0;
	virtual NFIActor* GetActor(const int nActorIndex) = 0;

public:
	virtual bool AddComponent(const int nActorIndex, NFIComponent* pComponent) = 0;

	virtual bool AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR functorPtr_end) = 0;
};

#endif
