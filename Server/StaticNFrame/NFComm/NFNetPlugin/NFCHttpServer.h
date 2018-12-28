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
	NFCHttpServer()
	{
		mIndex = 10000;
		mPort = 0;
		mUnLinkId = 0;
		mEventBase = NULL;
#if NF_ENABLE_SSL
		mEnableSSL = true;
		mSSLCtx = NULL;
		certificate_chain_file_ = "server-certificate-chain.pem";
		private_key_file_ = "server-private-key.pem";
#endif
	}

	template<typename BaseType>
	NFCHttpServer(uint32_t unLinkId, BaseType* pBaseType, bool (BaseType::*handleRecieve)(uint32_t,const NFHttpHandle& req), NFWebStatus(BaseType::*handleFilter)(uint32_t, const NFHttpHandle& req))
	{
		mEventBase = NULL;
		mReceiveCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2);
		mFilter = std::bind(handleFilter, pBaseType, std::placeholders::_1, std::placeholders::_2);
		mPort = 0;
		mUnLinkId = unLinkId;
		mIndex = 10000;
		certificate_chain_file_ = "server-certificate-chain.pem";
		private_key_file_ = "server-private-key.pem";
#if NF_ENABLE_SSL
		mEnableSSL = false;
		mSSLCtx = NULL;
#endif
	}

	virtual ~NFCHttpServer();

	NFHttpHandle* AllocHttpRequest();

	virtual uint32_t GetLinkId() const;

	virtual bool Init();
	virtual bool Execute();

	int InitServerImpl(uint32_t nPort);
	virtual int InitServer(uint32_t nPort);
	virtual int InitServer(std::vector<uint32_t> nPorts);
	virtual int InitServer(const std::string& portStr);

	virtual bool ResponseMsg(const NFHttpHandle& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");
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

	static void AddResponseHeader(const NFHttpHandle& req, const std::string& key, const std::string& value);
private:
	uint32_t mPort;
	std::vector<uint32_t> mVecPort;
	struct event_base* mEventBase;
	std::vector<struct evhttp*> mEvhttpList;
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;
	uint32_t mUnLinkId;
	uint64_t mIndex;
	std::map<uint64_t, NFHttpHandle*> mHttpRequestMap;
	std::list<NFHttpHandle*> mListHttpRequestPool;

#if NF_ENABLE_SSL
	bool mEnableSSL;
	SSL_CTX* mSSLCtx;
	std::string certificate_chain_file_;
	std::string private_key_file_;
#endif
};