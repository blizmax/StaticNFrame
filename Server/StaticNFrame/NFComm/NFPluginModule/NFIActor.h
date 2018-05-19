// -------------------------------------------------------------------------
//    @FileName         :    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_H
#define NFI_ACTOR_H

#define THERON_USE_STD_THREADS 1

#include <map>
#include <string>
#include <Theron/Theron.h>
#include <functional>
#include "NFIModule.h"

class NFIComponent;
class NFIActorModule;

typedef std::function<int(uint64_t, const int, const int, std::string&)> ACTOR_PROCESS_FUNCTOR;

class NFIActorMessage
{
public:
    enum MessageType
    {
        ACTOR_MSG_TYPE_COMPONENT,
        ACTOR_MSG_TYPE_END_FUNC,
        ACTOR_MSG_TYPE_NET_MSG,
    };

    NFIActorMessage()
    {
        nMsgID = 0;
        nFormActor = 0;
        msgType = ACTOR_MSG_TYPE_COMPONENT;
		self = 0;
    }

    int nMsgID;
    int nFormActor;
    std::string data;
    MessageType msgType;
    ////////////////////event/////////////////////////////////////////////////
    uint64_t self;
    //////////////////////////////////////////////////////////////////////////
    ACTOR_PROCESS_FUNCTOR xEndFuncptr;
protected:
private:
};

class NFIActor : public Theron::Actor
{
public:
    NFIActor(Theron::Framework& framework) : Theron::Actor(framework)
    {
        RegisterHandler(this, &NFIActor::DefaultHandler);
		m_pActorModule = nullptr;
    }

    virtual ~NFIActor() {}

    virtual void AddComponent(NFIComponent* pComponent) = 0;

    template<typename T>
    T* FindComponent(const std::string& strName)
    {
        if (!TIsDerived<T, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return NF_SHARE_PTR<T>();
        }

        NFIComponent* pComponent = FindComponent(strName);
        T* pT = dynamic_cast<T*>(pComponent);
        return pT;
    }
    virtual NFIComponent* FindComponent(const std::string& strComponentName) = 0;

    virtual bool AddBeginFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR xBeginFunctor) = 0;
    virtual bool AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR xEndFunctor) = 0;

    virtual bool SendMsg(const Theron::Address& address, const NFIActorMessage& message) = 0;

protected:

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address& from) {};
    virtual void Handler(const NFIActorMessage& message, const Theron::Address& from) {};

private:
    void DefaultHandler(const NFIActorMessage& message, const Theron::Address from) //-V801
    {
        if (message.msgType == NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT)
        {
            Handler(message, from);
        }
        else
        {
            HandlerEx(message, from);
        }
    }

protected:
    NFIActorModule* m_pActorModule;
};

#endif
