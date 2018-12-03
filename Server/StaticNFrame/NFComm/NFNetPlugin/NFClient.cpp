// -------------------------------------------------------------------------
//    @FileName         :    NFClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-05-24
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFClient.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NFComm/NFCore/NFCommon.h"

NFClient::NFClient(uint32_t nId, const NFClientFlag& flag) : m_pMainBase(nullptr), m_pObject(nullptr), m_usLinkId(nId), mLastActionTime(0)
{
#ifdef _MSC_VER
	WSADATA wsaData;
	int nResult = WSAStartup(0x0201, &wsaData);
	if (nResult)
	{
		std::cout << "WSAStartup failed with error code:" << nResult << std::endl;
		return;
	}
#endif
	m_usLinkId = nId;
	m_flag = flag;
	mStatus = eConnectStatus_Disconnect;

	m_pMainBase = event_base_new();
	if (m_pMainBase == nullptr)
	{
		NFLogError("NetError | error: client event_base_new failed!");
		assert(false);
	}
}

NFClient::~NFClient()
{
	/**
	 *@brief  必须先析构m_pBev， 然后析构m_pMainBase
	 */
	NFClient::Close();
	if (m_pMainBase)
	{
		event_base_free(m_pMainBase);
	}
	m_pMainBase = nullptr;
}

bool NFClient::Init()
{
	if (!Connect())
	{
		NFLogError("NetError | error: connected failed!");
		return false;
	}
	return true;
}

bool NFClient::Shut()
{
	if (m_pObject)
	{
		if (m_pObject->GetNeedRemove() == false)
		{
			m_pObject->SetNeedRemove(true);
			m_pObject->CloseObject();
		}
	}
	return true;
}

bool NFClient::Finalize()
{
	NFClient::Close();
	return true;
}

bool NFClient::Execute()
{
	ExecuteClose();
	if (m_pMainBase)
	{
		event_base_loop(m_pMainBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}
	return true;
}

void NFClient::ExecuteClose()
{
	if (m_pObject)
	{
		if (m_pObject->GetNeedRemove())
		{
			Close();
		}
	}
}

const string& NFClient::GetName() const
{
	return m_strName;
}

const NFClientFlag& NFClient::GetFlag() const
{
	return m_flag;
}

bool NFClient::Connect()
{
	m_pObject = new NetObject();

	struct bufferevent* pBev = bufferevent_socket_new(m_pMainBase, -1, BEV_OPT_CLOSE_ON_FREE);

	m_pObject->SetBev(pBev);
	m_pObject->SetLinkId(m_usLinkId);
	m_pObject->SetStrIp(m_flag.strIP);
	m_pObject->SetPort(m_flag.nPort);

	m_pObject->SetRecvCB(mRecvCB);
	m_pObject->SetEventCB(mEventCB);

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET ;
	inet_pton(AF_INET, m_flag.strIP.c_str(), (void*)&sin.sin_addr.s_addr);
	sin.sin_port = htons(m_flag.nPort);
	bufferevent_setcb(pBev, NetObject::conn_recvcb, NetObject::conn_writecb, NetObject::conn_eventcb, m_pObject);
	//设置水位为0
	bufferevent_setwatermark(pBev, EV_READ, 0, 0);
	bufferevent_setwatermark(pBev, EV_WRITE, 0, 0);

	bufferevent_enable(pBev, EV_WRITE | EV_READ);

	if (bufferevent_socket_connect(pBev, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) < 0)
	{
		NFLogError("NetError | connect failed! IP: {}, port:{}", m_flag.strIP,m_flag.nPort);
		return false;
	}

	return true;
}

void NFClient::Close()
{
	if (m_pObject)
	{
		NF_SAFE_DELETE(m_pObject);
	}
	m_pObject = nullptr;
}

event_base* NFClient::GetMainBase() const
{
	return m_pMainBase;
}

uint32_t NFClient::GetLinkId() const
{
	return m_usLinkId;
}

void NFClient::SetLinkId(uint32_t linkId)
{
	m_usLinkId = linkId;
}

bool NFClient::Send(const void* pData, uint32_t unSize)
{
	if (m_pObject)
	{
		return m_pObject->Send(pData, unSize);
	}
	return false;
}

eConnectStatus NFClient::GetStatus() const
{
	return mStatus;
}

void NFClient::SetStatus(eConnectStatus val)
{
	mStatus = val;
}

bool NFClient::IsNeedRemve() const
{
	return mStatus == eConnectStatus_REMOVE;
}

uint64_t NFClient::GetLastActionTime() const
{
	return mLastActionTime;
}

void NFClient::SetLastActionTime(uint64_t time)
{
	mLastActionTime = time;
}

