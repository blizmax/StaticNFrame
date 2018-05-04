#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include <stdint.h>

#include "event2/bufferevent.h"
#include "event2/event.h"
#include "event2/util.h"
#include "event2/buffer.h"
#include <event2/http.h>
#include <event2/http_struct.h>  
#include <event2/keyvalq_struct.h>
#include <event2/listener.h>
#include <event2/event_compat.h>

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFNetDefine.h"
#include "NFComm/NFCore/NFDataStream.h"
#include "NFComm/NFCore/NFBuffer.h"

#include <vector>



struct stMsg;
struct bufferevent;
class NFClient;

class NetObject : public NFIModule
{
	friend NFClient;
public:
	NetObject();
	virtual ~NetObject();
public:
	virtual bool Init() override;
public:
	virtual bool		OnRecvData(bufferevent* pBufEv);
	virtual int			Dismantle();
	virtual void		FlushData();
	virtual uint32_t	GetConn() const { return m_usConn; }
	virtual void		SetConn(uint32_t val) { m_usConn = val; }
	virtual bool		Ping(const stMsg* pMsg) { return false; };
	virtual uint64_t	GetPingTime() const { return 0; }
	virtual void		Reset();
public:
	struct bufferevent* GetBev() const { return m_pBev; }
	uint32_t			GetLinkId() const { return m_usLinkId; }
public:
	void				OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue);
protected:
	struct bufferevent* m_pBev;			//读写事件对象
	uint32_t			m_usLinkId;					//分配的Id
	stMsgFlag			m_flag;
	SOCKET				m_nSocket;
	uint32_t			m_usConn;
	NFBuffer			m_buffer;
protected:
	NFQueueVector<NFThreadNetMsg*> m_threadNetMsgs;
};
