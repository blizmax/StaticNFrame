// -------------------------------------------------------------------------
//    @FileName         :    NFEvppHttpServer.h
//    @Author           :    GaoYi
//    @Date             :    2019-6-9
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//                    .::::.
//                  .::::::::.
//                 :::::::::::  FUCK YOU
//             ..:::::::::::'
//           '::::::::::::'
//             .::::::::::
//        '::::::::::::::..
//             ..::::::::::::.
//           ``::::::::::::::::
//            ::::``:::::::::'        .:::.
//           ::::'   ':::::'       .::::::::.
//         .::::'      ::::     .:::::::'::::.
//        .:::'       :::::  .:::::::::' ':::::.
//       .::'        :::::.:::::::::'      ':::::.
//      .::'         ::::::::::::::'         ``::::.
//  ...:::           ::::::::::::'              ``::.
// ```` ':.          ':::::::::'                  ::::..
//                    '.:::::'                    ':'````..
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIHttpServer.h"
#include "evpp/http/http_server.h"
#include <list>
#include <map>
#include "NFComm/NFCore/NFQueue.hpp"
#include <experimental/filesystem>

class NFEvppHttpHandle : public NFIHttpHandle
{
public:
	NFEvppHttpHandle()
	{

	}

	virtual void Reset() override
	{
		requestId = 0;
		timeOut = 0;
		mCtx = nullptr;
		mResponseCb = nullptr;
	}

	virtual  const std::string& GetUrl() const  override { return mCtx->original_uri(); }
	virtual const std::string& GetPath() const  override { return mCtx->uri(); }
	virtual const std::string& GetRemoteHost() const  override { return mCtx->remote_ip(); }
	virtual int GetType() const  override { return type; }
	virtual std::string GetBody() const  override { return mCtx->body().ToString(); }
	virtual uint64_t GetRequestId() const override { return requestId; }
	virtual uint64_t GetTimeOut() const override { return timeOut; }

	virtual void AddResponseHeader(const std::string& key, const std::string& value) const override
	{
		mCtx->AddResponseHeader(key, value);
	}

	virtual bool ResponseMsg(const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") const override
	{
		AddResponseHeader("Content-Type", "application/json");
		AddResponseHeader("Access-Control-Allow-Origin", "*");

		mCtx->set_response_http_code(code);
		mResponseCb(strMsg);
		return true;
	}

	NFHttpType type;
	uint64_t requestId;
	uint64_t timeOut;
	evpp::http::ContextPtr mCtx;
	evpp::http::HTTPSendResponseCallback mResponseCb;
};

class NFEvppHttMsg
{
public:
	evpp::http::ContextPtr mCtx;
	evpp::http::HTTPSendResponseCallback mResponseCb;
};

class NFCHttpEvppServer : public NFIHttpServer
{
public:
	NFCHttpEvppServer(uint32_t serverType);

	virtual ~NFCHttpEvppServer();

	virtual bool Execute() override;

	virtual uint32_t GetServerType() const override;

	virtual bool InitServer(uint32_t listen_port) override;
	virtual bool InitServer(const std::vector<uint32_t>& listen_ports) override;
	virtual bool InitServer(const std::string& listen_ports/*like "80,8080,443"*/) override;

	/**
	 * @brief 主线程处理消息队列
	 */
	virtual void ProcessMsgLogicThread();

	NFEvppHttpHandle* AllocHttpRequest();

	virtual bool ResponseMsg(const NFIHttpHandle& req, const std::string& strMsg, NFWebStatus code,
		const std::string& strReason = "OK") override;

	virtual bool ResponseMsg(uint64_t reqeustId, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") override;
private:
	evpp::http::Server* m_pHttpServer;
private:
	uint32_t mPort;
	uint32_t mServerType;
	std::vector<uint32_t> mVecPort;

	/**
	* @brief 需要消息队列
	*/
	NFQueueVector<NFEvppHttMsg*> mMsgQueue;

	uint64_t mIndex;
	std::map<uint64_t, NFEvppHttpHandle*> mHttpRequestMap;
	std::list<NFEvppHttpHandle*> mListHttpRequestPool;
};