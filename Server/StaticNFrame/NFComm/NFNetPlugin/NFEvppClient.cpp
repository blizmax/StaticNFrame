// -------------------------------------------------------------------------
//    @FileName         :    NFEvppClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-06-04
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                               _(\_/) 
//                             ,((((^`\
//                            ((((  (6 \ 
//                          ,((((( ,    \
//      ,,,_              ,(((((  /"._  ,`,
//     ((((\\ ,...       ,((((   /    `-.-'
//     )))  ;'    `"'"'""((((   (      
//    (((  /            (((      \
//     )) |                      |
//    ((  |        .       '     |
//    ))  \     _ '      `t   ,.')
//    (   |   y;- -,-""'"-.\   \/  
//    )   / ./  ) /         `\  \
//       |./   ( (           / /'
//       ||     \\          //'|
//       ||      \\       _//'||
//       ||       ))     |_/  ||
//       \_\     |_/          ||
//       `'"                  \_\
//                            `'" 
// -------------------------------------------------------------------------

#include "NFEvppClient.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFEvppServer.h"

NFEvppClient::NFEvppClient(uint32_t nId, const NFClientFlag& flag):NFIClient(nId, flag)
{
	m_pObject = nullptr;
	m_eventLoop = nullptr;
	m_tcpClient = nullptr;

	m_eventLoop = NF_NEW evpp::EventLoopThread();
	m_eventLoop->set_name("NFEvppClientThread");

	std::string strIpPort = NF_FORMAT("{}:{}", m_flag.strIP, m_flag.nPort);
	m_tcpClient = NF_NEW evpp::TCPClient(m_eventLoop->loop(), strIpPort, "NFEvppClient");
}

NFEvppClient::~NFEvppClient()
{

}

bool NFEvppClient::Init()
{
	if (!Connect())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "NetError | error: connected failed!");
		return false;
	}
	return true;
}

bool NFEvppClient::Shut()
{
	if (m_pObject)
	{
		if (m_pObject->GetNeedRemove() == false)
		{
			m_pObject->SetNeedRemove(true);
			m_pObject->CloseObject();
		}
	}
	NFEvppClient::Close();
	return true;
}

bool NFEvppClient::Finalize()
{
	if (m_pObject)
	{
		NF_SAFE_DELETE(m_pObject);
	}
	if (m_tcpClient)
	{
		NF_SAFE_DELETE(m_tcpClient);
	}
	if (m_eventLoop)
	{
		NF_SAFE_DELETE(m_eventLoop);
	}
	m_tcpClient = nullptr;
	m_eventLoop = nullptr;
	return true;
}

bool NFEvppClient::Execute()
{
	ExecuteClose();
	ProcessMsgLogicThread();
	return true;
}

void NFEvppClient::ExecuteClose()
{
	if (m_pObject)
	{
		if (m_pObject->GetNeedRemove())
		{
			Close();
		}
	}
}

bool NFEvppClient::Connect()
{
	m_eventLoop->Start();

	//消息回调是在别的线程里运行的
	m_tcpClient->SetConnectionCallback([this](const evpp::TCPConnPtr& conn)
	{
		if (conn->IsConnected())
		{
			conn->SetTCPNoDelay(true);
			MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
			pMsg->nType = eMsgType_CONNECTED;
			mMsgQueue.Push(pMsg);
		}
		else
		{
			MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
			pMsg->nType = eMsgType_DISCONNECTED;
			mMsgQueue.Push(pMsg);
		}
	});

	//消息回调是在别的线程里运行的
	m_tcpClient->SetMessageCallback([this](const evpp::TCPConnPtr& conn,
		evpp::Buffer* msg)
	{
		evpp::Slice xMsgBuff;
		if (msg)
		{
			xMsgBuff = msg->NextAll();
			MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
			pMsg->nType = eMsgType_RECIVEDATA;
			pMsg->strMsg = std::string(xMsgBuff.data(), xMsgBuff.size());
			mMsgQueue.Push(pMsg);
		}
	});
	m_tcpClient->set_auto_reconnect(m_flag.bAutoConnect);
	m_tcpClient->Connect();

	return true;
}

void NFEvppClient::ProcessMsgLogicThread()
{
	std::vector<MsgFromNetInfo*> vecMsg;
	mMsgQueue.Pop(vecMsg);
	for (size_t i = 0; i < vecMsg.size(); i++)
	{
		MsgFromNetInfo* pMsg = vecMsg[i];
		if (pMsg == nullptr) continue;

		if (pMsg->nType == eMsgType_CONNECTED)
		{
			m_pObject = new NetEvppObject(pMsg->mTCPConPtr);

			m_pObject->SetIsServer(false);
			m_pObject->SetLinkId(m_usLinkId);
			m_pObject->SetStrIp(m_flag.strIP);
			m_pObject->SetPort(m_flag.nPort);

			m_pObject->SetRecvCB(mRecvCB);
			m_pObject->SetEventCB(mEventCB);

			pMsg->mTCPConPtr->set_context(evpp::Any(m_pObject));
			m_pObject->OnHandleConnect();
		}
		else if (pMsg->nType == eMsgType_DISCONNECTED)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject && pObject == m_pObject)
				{
					pObject->OnHandleDisConnect();
				}
				else
				{
					NFLogError(NF_LOG_SYSTEMLOG, 0, "pObject != m_pObject, thread client wrong!!");
				}
				pMsg->mTCPConPtr->set_context(evpp::Any(nullptr));
			}
			else
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "connect failed!");
			}
		}
		else if (pMsg->nType == eMsgType_RECIVEDATA)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject && pObject == m_pObject)
				{
					pObject->OnRecvData(pMsg->strMsg.data(), pMsg->strMsg.length());
				}
				else
				{
					NFLogError(NF_LOG_SYSTEMLOG, 0, "pObject != m_pObject, thread client wrong!!");
				}
			}
		}
		else
		{
			//
		}

		NF_SAFE_DELETE(pMsg);
	}
}

void NFEvppClient::Close()
{
	m_tcpClient->Disconnect();
	m_eventLoop->Stop(true);
	m_pObject = nullptr;
}

bool NFEvppClient::Send(const void* pData, uint32_t unSize)
{
	if (m_pObject)
	{
		return m_pObject->Send(pData, unSize);
	}
	return false;
}

