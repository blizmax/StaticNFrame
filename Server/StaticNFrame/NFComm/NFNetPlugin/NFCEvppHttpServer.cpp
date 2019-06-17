// -------------------------------------------------------------------------
//    @FileName         :    NFEvppHttpServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-6-9
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                    _ooOoo_
//                   o8888888o
//                   88" . "88
//                   (| -_- |)
//                    O\ = /O
//                ____/`---'\____
//              .   ' \\| |// `.
//               / \\||| : |||// \
//             / _||||| -:- |||||- \
//               | | \\\ - /// | |
//            | \_| ''\---/'' | |
//              \ .-\__ `-` ___/-. /
//           ___`. .' /--.--\ `. . __
//        ."" '< `.___\_<|>_/___.' >'"".
//       | | : `- \`.;`\ _ /`;.`/ - ` : | |
//         \ \ `-. \_ __\ /__ _/ .-` / /
// ======`-.____`-.___\_____/___.-`____.-'======
//                    `=---='
//
// .............................................
//          ·ð×æ±£ÓÓ             ÓÀÎÞBUG
//
// -------------------------------------------------------------------------

#include "NFCEvppHttpServer.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFCommon.h"

#include "evpp/http/context.h"
#include "NFCHttpServer.h"

NFCHttpEvppServer::NFCHttpEvppServer(uint32_t serverType)
{
	mServerType = serverType;
	mPort = 0;
	m_pHttpServer = new evpp::http::Server(1);

	m_pHttpServer->RegisterDefaultHandler([this](evpp::EventLoop* loop,
		const evpp::http::ContextPtr& ctx,
		const evpp::http::HTTPSendResponseCallback& respcb) {
		NFEvppHttMsg* pMsg = NF_NEW NFEvppHttMsg();
		pMsg->mCtx = ctx;
		pMsg->mResponseCb = respcb;
		mMsgQueue.Push(pMsg);
	});
}

NFCHttpEvppServer::~NFCHttpEvppServer()
{
	for (auto iter = mHttpRequestMap.begin(); iter != mHttpRequestMap.end(); iter++)
	{
		NF_SAFE_DELETE(iter->second);
	}
	mHttpRequestMap.clear();

	for (auto iter = mListHttpRequestPool.begin(); iter != mListHttpRequestPool.end(); iter++)
	{
		NF_SAFE_DELETE(*iter);
	}
	mListHttpRequestPool.clear();

	if (m_pHttpServer)
	{
		m_pHttpServer->Stop();
		NF_SAFE_DELETE(m_pHttpServer);
	}
}

bool NFCHttpEvppServer::Execute()
{
	ProcessMsgLogicThread();
	std::vector<NFHttpHandle*> vec;
	for (auto iter = mHttpRequestMap.begin(); iter != mHttpRequestMap.end(); iter++)
	{
		NFHttpHandle* pRequest = dynamic_cast<NFHttpHandle*>(iter->second);
		if (pRequest->timeOut + 10 <= (uint64_t)NFGetSecondTime())
		{
			vec.push_back(pRequest);
		}
	}

	for (int i = 0; i < (int)vec.size(); i++)
	{
		NFHttpHandle* pRequest = vec[i];
		ResponseMsg(*pRequest, "TimeOut Error", NFWebStatus::WEB_TIMEOUT);
	}

	return true;
}

uint32_t NFCHttpEvppServer::GetServerType() const
{
	return mServerType;
}

bool NFCHttpEvppServer::InitServer(uint32_t listen_port)
{
	if (m_pHttpServer->Init(listen_port))
	{
		if (m_pHttpServer->Start())
		{
			return true;
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Start Listen Port:{} Failed!", listen_port);
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "Init Listen Port:{} Failed!", listen_port);
	return false;
}

bool NFCHttpEvppServer::InitServer(const std::vector<uint32_t>& listen_ports)
{
	if (m_pHttpServer->Init((const std::vector<int>&)listen_ports))
	{
		if (m_pHttpServer->Start())
		{
			return true;
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Start Listen Port:{} Failed!", NFCommon::tostr(listen_ports));
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "Init Listen Port:{} Failed!", NFCommon::tostr(listen_ports));
}

bool NFCHttpEvppServer::InitServer(const std::string& listen_ports/*like "80,8080,443"*/)
{
	if (m_pHttpServer->Init(listen_ports))
	{
		if (m_pHttpServer->Start())
		{
			return true;
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Start Listen Port:{} Failed!", listen_ports);
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "Init Listen Port:{} Failed!", listen_ports);
}

void NFCHttpEvppServer::ProcessMsgLogicThread()
{
	std::vector<NFEvppHttMsg*> vecMsg;
	mMsgQueue.Pop(vecMsg);
	for (size_t i = 0; i < vecMsg.size(); i++)
	{
		NFEvppHttMsg* pMsg = vecMsg[i];
		if (pMsg == nullptr) continue;

		NFEvppHttpHandle* pRequest = AllocHttpRequest();
		pRequest->mCtx = pMsg->mCtx;
		pRequest->mResponseCb = pMsg->mResponseCb;
		pRequest->type = (NFHttpType)pMsg->mCtx->req()->type;
		pRequest->timeOut = NFGetSecondTime();;

		mHttpRequestMap.emplace(pRequest->requestId, pRequest);

		if (mFilter)
		{
			//return 401
			try
			{
				NFWebStatus xWebStatus = mFilter(mServerType, *pRequest);
				if (xWebStatus != NFWebStatus::WEB_OK)
				{
					//401
					ResponseMsg(*pRequest, "Filter error", xWebStatus);
					return;
				}
			}
			catch (std::exception& e)
			{
				ResponseMsg(*pRequest, e.what(), NFWebStatus::WEB_ERROR);
				return;
			}
			catch (...)
			{
				ResponseMsg(*pRequest, "UNKNOW ERROR", NFWebStatus::WEB_ERROR);
				return;
			}

		}

		// call cb
		try
		{
			if (mReceiveCB)
			{
				mReceiveCB(mServerType, *pRequest);
				return;
			}
			else
			{
				ResponseMsg(*pRequest, "NO PROCESSER", NFWebStatus::WEB_ERROR);
				return;
			}
		}
		catch (std::exception& e)
		{
			ResponseMsg(*pRequest, e.what(), NFWebStatus::WEB_ERROR);
			return;
		}
		catch (...)
		{
			ResponseMsg(*pRequest, "UNKNOW ERROR", NFWebStatus::WEB_ERROR);
			return;
		}

		NF_SAFE_DELETE(pMsg);
	}
}

NFEvppHttpHandle* NFCHttpEvppServer::AllocHttpRequest()
{
	if (mListHttpRequestPool.size() <= 0)
	{
		for (int i = 0; i < 1024; i++)
		{
			mListHttpRequestPool.push_back(NF_NEW NFEvppHttpHandle());
		}
	}

	NFEvppHttpHandle* pRequest = dynamic_cast<NFEvppHttpHandle*>(mListHttpRequestPool.front());
	mListHttpRequestPool.pop_front();

	pRequest->Reset();

	pRequest->requestId = ++mIndex;

	return pRequest;
}

bool NFCHttpEvppServer::ResponseMsg(const NFIHttpHandle& req, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	req.ResponseMsg(strMsg, code, strReason);

	auto it = mHttpRequestMap.find(req.GetRequestId());
	if (it != mHttpRequestMap.end())
	{
		it->second->Reset();
		mListHttpRequestPool.push_back(it->second);
		mHttpRequestMap.erase(it);
	}
	return true;
}

bool NFCHttpEvppServer::ResponseMsg(uint64_t requestId, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	NFEvppHttpHandle* req = nullptr;
	auto it = mHttpRequestMap.find(requestId);
	if (it == mHttpRequestMap.end())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Response Msg Timeout........ requestId:{}, strMsg:{}", requestId, strMsg);
		return false;
	}

	req = it->second;

	bool ret = req->ResponseMsg(strMsg, code, strReason);
	if (!ret)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Response Msg error........ requestId:{}, strMsg:{}", requestId, strMsg);
	}

	req->Reset();
	mListHttpRequestPool.push_back(req);
	mHttpRequestMap.erase(it);
	return true;
}
