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

NFEvppClient::NFEvppClient(evpp::EventLoop* eventLoop, uint32_t nId, const NFClientFlag& flag):NFIClient(nId, flag)
{
	m_pObject = nullptr;
	m_tcpClient = nullptr;

	std::string strIpPort = NF_FORMAT("{}:{}", m_flag.strIP, m_flag.nPort);

	if (eventLoop)
	{
		m_tcpClient = NF_NEW evpp::TCPClient(eventLoop, strIpPort, "NFEvppClient");
	}
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
	if (m_tcpClient && m_tcpClient->conn())
	{
		m_tcpClient->Disconnect();
	}
	return true;
}

void NFEvppClient::Quit()
{
	if (m_tcpClient)
	{
		NF_SAFE_DELETE(m_tcpClient);
	}
}

bool NFEvppClient::Finalize()
{
	if (m_pObject)
	{
		NF_SAFE_DELETE(m_pObject);
	}

	m_tcpClient->loop()->QueueInLoop(std::bind(&NFEvppClient::Quit, this));

	m_tcpClient = nullptr;
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

void NFEvppClient::CloseServer()
{
	if (m_pObject)
	{
		if (m_pObject->GetNeedRemove() == false)
		{
			m_pObject->SetNeedRemove(true);
			m_pObject->CloseObject();
		}
	}

	if (m_tcpClient && m_tcpClient->conn())
	{
		m_tcpClient->Disconnect();
	}
}

bool NFEvppClient::Connect()
{
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
			if (!conn->context().IsEmpty())
			{
				xMsgBuff = msg->NextAll();

				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(conn->context());
				if (pObject)
				{
					pObject->m_buffer.PushData(xMsgBuff.data(), xMsgBuff.size());

					while (true)
					{
						char* outData = nullptr;
						uint32_t outLen = 0;
						uint32_t allLen = 0;
						uint32_t nMsgId = 0;
						uint64_t nValue = 0;
						int ret = NFIPacketParse::DeCode(pObject->mPacketParseType, pObject->m_buffer.ReadAddr(), pObject->m_buffer.ReadableSize(), outData, outLen, allLen, nMsgId, nValue);
						if (ret < 0)
						{
							pObject->m_buffer.Consume(pObject->m_buffer.ReadableSize());
							break;
						}
						else if (ret > 0)
						{
							break;
						}
						else
						{
							//OnHandleMsgPeer(eMsgType_RECIVEDATA, m_usLinkId, outData, outLen, nMsgId, nValue);
							MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
							pMsg->nType = eMsgType_RECIVEDATA;
							pMsg->strMsg = std::string(outData, outLen);
							pMsg->nMsgId = nMsgId;
							pMsg->nValue = nValue;
							mMsgQueue.Push(pMsg);

							pObject->m_buffer.Consume(allLen);
							continue;
						}
					}
				}
			}
		}
	});
	m_tcpClient->set_auto_reconnect(false);

	if (!m_pObject)
	{
		m_pObject = new NetEvppObject(nullptr);
	}

	m_pObject->SetPacketParseType(GetPacketParseType());
	m_pObject->SetIsServer(false);
	m_pObject->SetLinkId(m_usLinkId);
	m_pObject->SetStrIp(m_flag.strIP);
	m_pObject->SetPort(m_flag.nPort);

	m_pObject->SetRecvCB(mRecvCB);
	m_pObject->SetEventCB(mEventCB);

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
			if (m_pObject)
			{
				m_pObject->SetConnPtr(pMsg->mTCPConPtr);
				pMsg->mTCPConPtr->set_context(evpp::Any(m_pObject));
				m_pObject->OnHandleConnect();
			}
		}
		else if (pMsg->nType == eMsgType_DISCONNECTED)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject && pObject == m_pObject)
				{
					m_pObject->OnHandleDisConnect();
				}
				else
				{
					NFLogError(NF_LOG_SYSTEMLOG, 0, "pObject != m_pObject, thread client wrong!!");
				}
				pMsg->mTCPConPtr->set_context(evpp::Any(nullptr));
			}
			else
			{
				if (m_pObject)
				{
					m_pObject->OnHandleDisConnect();
				}
			}
		}
		else if (pMsg->nType == eMsgType_RECIVEDATA)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject)
				{
					pObject->OnHandleMsgPeer(eMsgType_RECIVEDATA, pObject->m_usLinkId, (char*)pMsg->strMsg.data(), pMsg->strMsg.length(), pMsg->nMsgId, pMsg->nValue);
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
	if (m_tcpClient && m_tcpClient->conn())
	{
		m_tcpClient->Disconnect();
	}
	
	if (m_pObject)
	{
		NF_SAFE_DELETE(m_pObject);
	}
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

