// -------------------------------------------------------------------------
//    @FileName         :    NFHttpServer.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIHttpServer.h"
#include "NFLibEvent.h"
#include <list>
#include <map>

class NFHttpHandle : public NFIHttpHandle
{
public:
	NFHttpHandle()
	{
		Reset();
	}

	virtual void Reset() override
	{
		requestId = 0;
		url.clear();
		path.clear();
		remoteHost.clear();
		httpServer = nullptr;
		timeOut = 0;
	}

	virtual  const std::string& GetUrl() const  override { return url; }
	virtual const std::string& GetPath() const  override { return path; }
	virtual const std::string& GetRemoteHost() const  override { return remoteHost; }
	virtual int GetType() const  override { return type; }
	virtual std::string GetBody() const  override  { return bodySlice.ToString(); }
	virtual uint64_t GetRequestId() const override { return requestId; }
	virtual uint64_t GetTimeOut() const override { return timeOut; }

	virtual void AddResponseHeader(const std::string& key, const std::string& value) const override
	{
		evhttp_add_header(evhttp_request_get_output_headers(req), key.data(), value.data());
	}

	virtual bool ResponseMsg(const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") const override
	{
		//create buffer
		struct evbuffer* eventBuffer = evbuffer_new();

		//send data
		evbuffer_add_printf(eventBuffer, strMsg.c_str());

		AddResponseHeader("Content-Type", "application/json");
		AddResponseHeader("Access-Control-Allow-Origin", "*");

		evhttp_send_reply(req, code, strReason.c_str(), eventBuffer);

		//free
		evbuffer_free(eventBuffer);

		return true;
	}

	evhttp_request* req;
	std::string url;
	std::string path;
	std::string remoteHost;
	NFHttpType type;
	NFSlice bodySlice;
	uint64_t requestId;
	NFCHttpServer* httpServer;
	uint64_t timeOut;
};

class NFCHttpServer : public NFIHttpServer
{
public:
	NFCHttpServer(uint32_t serverType)
	{
		mEventBase = NULL;
		mPort = 0;
		mServerType = serverType;
		mIndex = 10000;
#if NF_ENABLE_SSL
		certificate_chain_file_ = "server-certificate-chain.pem";
		private_key_file_ = "server-private-key.pem";
		mEnableSSL = false;
		mSSLCtx = NULL;
#endif
	}

	virtual ~NFCHttpServer();

	NFHttpHandle* AllocHttpRequest();

	virtual uint32_t GetServerType() const;

	virtual bool Init();
	virtual bool Execute();

	bool InitServerImpl(uint32_t nPort);
	virtual bool InitServer(uint32_t nPort);
	virtual bool InitServer(const std::vector<uint32_t>& listen_ports);
	virtual bool InitServer(const std::string& portStr);

	virtual bool ResponseMsg(const NFIHttpHandle& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");
	virtual bool ResponseMsg(uint64_t reqeustId, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");

#if NF_ENABLE_SSL
	bool EnableSSL() const;

	void SetEnableSSL(bool enable);

	const std::string& certificate_chain_file() const;

	const std::string& private_key_file() const;

	void set_certificate_chain_file(const std::string& filename);

	void set_private_key_file(const std::string& filename);

	/* berif 初始化SSL
	* param force_enable 强制启用SSL
	*/
	bool initSSL(bool force_enable = false);
#endif	
private:
	static void listener_cb(struct evhttp_request* req, void* arg);

	static void AddResponseHeader(const NFIHttpHandle& req, const std::string& key, const std::string& value);
private:
	uint32_t mPort;
	std::vector<uint32_t> mVecPort;
	struct event_base* mEventBase;
	std::vector<struct evhttp*> mEvhttpList;
	uint32_t mServerType;
	uint64_t mIndex;
	std::map<uint64_t, NFIHttpHandle*> mHttpRequestMap;
	std::list<NFIHttpHandle*> mListHttpRequestPool;

#if NF_ENABLE_SSL
	bool mEnableSSL;
	SSL_CTX* mSSLCtx;
	std::string certificate_chain_file_;
	std::string private_key_file_;
#endif
};