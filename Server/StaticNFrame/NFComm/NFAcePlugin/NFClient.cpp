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
#include "NFComm/NFNetPlugin/NFIPacketParse.h"
#include "NFComm/NFCore/NFCommon.h"

NFClient::NFClient(uint32_t nId, const NFClientFlag& flag) : m_pMainBase(nullptr), m_pObject(nullptr), m_usLinkId(nId), mLastActionTime(0)
{
	m_usLinkId = nId;
	m_flag = flag;
	mStatus = eConnectStatus_Disconnect;
}

NFClient::~NFClient()
{
	/**
	*@brief  必须先析构m_pBev， 然后析构m_pMainBase
	*/
	NFClient::Close();
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
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);

	msg_queue()->high_water_mark(MAX_MSG_MASK);
	msg_queue()->low_water_mark(MAX_MSG_MASK);

	//设置链接为非阻塞模式
	if (this->peer().enable(ACE_NONBLOCK) == -1)
	{
		NFLogError("this->peer().enable(ACE_NONBLOCK) error");
		return false;
	}

	//获得远程链接地址和端口
	if (this->peer().get_remote_addr(m_addrRemote) == -1)
	{
		NFLogError("this->peer().get_remote_addr(m_addrRemote) error");
		return false;
	}

	int nRet = m_AddrServer.set(m_flag.nPort, m_flag.strIP.c_str());

	if (-1 == nRet)
	{
		NFLogError("adrClient({}:{}) error.\n", m_flag.strIP, m_flag.nPort);
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

