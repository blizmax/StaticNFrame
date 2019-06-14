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

class NFCHttpServer : public NFIHttpServer
{
public:
	NFCHttpServer(uint32_t serverType)
	{
		mEventBase = NULL;
		mPort = 0;
		mServerType = serverType;
		mIndex = 10000;
		certificate_chain_file_ = "server-certificate-chain.pem";
		private_key_file_ = "server-private-key.pem";
#if NF_ENABLE_SSL
		mEnableSSL = false;
		mSSLCtx = NULL;
#endif
	}

	virtual ~NFCHttpServer();

	NFIHttpHandle* AllocHttpRequest();

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