// -------------------------------------------------------------------------
//    @FileName         :    NFHttpServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFCHttpServer.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "common/rapidjson/rapidjson.h"
#include "common/rapidjson/document.h"
#include "common/rapidjson/stringbuffer.h"
#include "common/rapidjson/writer.h"
#include "NFComm/NFCore/NFCommon.h"

NFCHttpServer::~NFCHttpServer()
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

	for (size_t i = 0; i < mEvhttpList.size(); i++)
	{
		if (mEvhttpList[i])
		{
			evhttp_free(mEvhttpList[i]);
		}
	}
	mEvhttpList.clear();

	if (mEventBase)
	{
		event_base_free(mEventBase);
		mEventBase = NULL;
	}
#if NF_ENABLE_SSL
	if (mSSLCtx) {
		SSL_CTX_free(mSSLCtx);
	}
#endif
}

#if NF_ENABLE_SSL
bool NFCHttpServer::initSSL(bool force_enable) {
	if (force_enable) {
		if (mSSLCtx) { SSL_CTX_free(mSSLCtx); }
		mSSLCtx = nullptr;
		mEnableSSL = true;
	}
	if (!mEnableSSL) {
		return true;
	}
	if (mSSLCtx) { return true; };

	/* 初始化SSL协议环境 */
	// SSL_library_int();
	/* 创建SSL上下文 */
	SSL_CTX *ctx = SSL_CTX_new(SSLv23_server_method());
	if (ctx == NULL) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "SSL_CTX_new failed");
		return false;
	}
	/* 设置SSL选项 https://linux.die.net/man/3/ssl_ctx_set_options */
	SSL_CTX_set_options(ctx,
		SSL_OP_SINGLE_DH_USE |
		SSL_OP_SINGLE_ECDH_USE |
		SSL_OP_NO_SSLv2 /*禁用SSLv2*/ |
		SSL_OP_NO_TLSv1 /*禁用TLSv1*/);
	/* 是否校验对方证书(这里是服务端，使用SSL_VERIFY_NONE参数表示不校验) */
	SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
	/* 创建椭圆曲线加密key */
	EC_KEY *ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if (ecdh == NULL) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "EC_KEY_new_by_curve_name failed");
		ERR_print_errors_fp(stderr);
		return false;
	}
	/* 设置ECDH临时公钥 */
	if (1 != SSL_CTX_set_tmp_ecdh(ctx, ecdh)) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "SSL_CTX_set_tmp_ecdh failed");
		return false;
	}
	/* 加载证书链文件(文件编码必须为PEM格式，使用Base64编码) */
	/* 此处也可使用SSL_CTX_use_certificate_file仅加载公钥证书 */
	if (1 != SSL_CTX_use_certificate_chain_file(
		ctx, certificate_chain_file_.c_str())) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Load certificate chain file({})failed", certificate_chain_file_);
		ERR_print_errors_fp(stderr);
		return false;
	}
	/* 加载私钥文件 */
	if (1 != SSL_CTX_use_PrivateKey_file(
		ctx, private_key_file_.c_str(), SSL_FILETYPE_PEM)) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Load private key file({})failed", private_key_file_);
		ERR_print_errors_fp(stderr);
		return false;
	}
	/* 校验私钥与证书是否匹配 */
	if (1 != SSL_CTX_check_private_key(ctx)) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "EC_KEY_new_by_curve_name failed");
		ERR_print_errors_fp(stderr);
		return false;
	}
	auto bevcb = [](struct event_base *base, void *arg)
		-> struct bufferevent* {
		struct bufferevent* r;
		SSL_CTX *sslctx = (SSL_CTX *)arg;
		r = bufferevent_openssl_socket_new(base,
			-1,
			SSL_new(sslctx),
			BUFFEREVENT_SSL_ACCEPTING,
			BEV_OPT_CLOSE_ON_FREE);
		return r;
	};

	for (size_t i = 0; i < mEvhttpList.size(); i++)
	{
		evhttp_set_bevcb(mEvhttpList[i], bevcb, ctx);
	}
	
	mSSLCtx = ctx;
	return true;
}
#endif

uint32_t NFCHttpServer::GetServerType() const
{
	return mServerType;
}

NFHttpHandle* NFCHttpServer::AllocHttpRequest()
{
	if (mListHttpRequestPool.size() <= 0)
	{
		for (int i = 0; i < 1024; i++)
		{
			mListHttpRequestPool.push_back(NF_NEW NFHttpHandle());
		}
	}

	NFHttpHandle* pRequest = mListHttpRequestPool.front();
	mListHttpRequestPool.pop_front();
	pRequest->Reset();

	pRequest->requestId = ++mIndex;

	return pRequest;
}

bool NFCHttpServer::Execute()
{
	if (mEventBase)
	{
		event_base_loop(mEventBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}

	std::vector<NFHttpHandle*> vec;
	for (auto iter = mHttpRequestMap.begin(); iter != mHttpRequestMap.end();iter++)
	{
		NFHttpHandle* pRequest = iter->second;
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

bool NFCHttpServer::InitServer(const std::vector<uint32_t>& nPorts)
{
	Init();
	for (size_t i = 0; i < nPorts.size(); i++)
	{
		int ret = InitServerImpl(nPorts[i]);
		if (ret != 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Listen Port:{} Failed!", nPorts[i]);
			return false;
		}
	}

#if NF_ENABLE_SSL
	if (mEnableSSL) {
		initSSL();
	}
#endif
	return true;
}

bool NFCHttpServer::InitServer(const std::string& portStr)
{
	Init();

	std::vector<std::string> result;
	std::string str = portStr;
	NFStringUtility::Trim(str);
	NFStringUtility::Split(str, ",", &result);
	for (size_t i = 0; i < result.size(); i++)
	{
		uint32_t port = NFCommon::strto<uint32_t>(result[i]);
		int ret = InitServerImpl(port);
		if (ret != 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Listen Port:{} from Port String:{} Failed!", port, portStr);
			return false;
		}
	}

#if NF_ENABLE_SSL
	if (mEnableSSL) {
		initSSL();
	}
#endif
	return true;
}

bool NFCHttpServer::Init()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		return false;
	}
#endif

	mEventBase = event_base_new();
	if (!mEventBase)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "create event_base fail");
		return false;
	}

	return true;
}

bool NFCHttpServer::InitServer(uint32_t port)
{
	Init();

	bool ret = InitServerImpl(port);

#if NF_ENABLE_SSL
	if (mEnableSSL) {
		initSSL();
	}
#endif

	return ret;
}

bool NFCHttpServer::InitServerImpl(uint32_t port)
{
	mPort = port;
	mVecPort.push_back(port);
	//struct event_base *base;
	struct evhttp* http;
	struct evhttp_bound_socket* handle;

	http = evhttp_new(mEventBase);
	if (!http)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "create evhttp fail");
		return 1;
	}

	mEvhttpList.push_back(http);

	/* 创建SSL上下文环境 ，可以理解为 SSL句柄 */
	/*
	SSL_CTX *ctx = SSL_CTX_new (SSLv23_server_method ());
	SSL_CTX_set_options (ctx,
	SSL_OP_SINGLE_DH_USE |
	SSL_OP_SINGLE_ECDH_USE |
	SSL_OP_NO_SSLv2);
	*/
	/* Cheesily pick an elliptic curve to use with elliptic curve ciphersuites.
	* We just hardcode a single curve which is reasonably decent.
	* See http://www.mail-archive.com/openssl-dev@openssl.org/msg30957.html */
	/*
	EC_KEY *ecdh = EC_KEY_new_by_curve_name (NID_X9_62_prime256v1);
	if (! ecdh)
	die_most_horribly_from_openssl_error ("EC_KEY_new_by_curve_name");
	if (1 != SSL_CTX_set_tmp_ecdh (ctx, ecdh))
	die_most_horribly_from_openssl_error ("SSL_CTX_set_tmp_ecdh");
	*/
	/* 选择服务器证书 和 服务器私钥. */
	/*
	const char *certificate_chain = "server-certificate-chain.pem";
	const char *private_key = "server-private-key.pem";
	*/
	/* 设置服务器证书 和 服务器私钥 到
	OPENSSL ctx上下文句柄中 */
	//server_setup_certs (ctx, certificate_chain, private_key);

	/*
	使我们创建好的evhttp句柄 支持 SSL加密
	实际上，加密的动作和解密的动作都已经帮
	我们自动完成，我们拿到的数据就已经解密之后的
	*/

	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", mPort);
	if (!handle)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "http server bind port :{} fail!", mPort);
		return false;
	}

	evhttp_set_gencb(http, listener_cb, (void*) this);

	return true;
}

void NFCHttpServer::listener_cb(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "req == NULL");
		return;
	}

	if (req->evcon == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "req->evcon == NULL");
		return;
	}

	NFCHttpServer* pNet = (NFCHttpServer*)arg;
	if (pNet == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "pNet ==NULL");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	NFHttpHandle* pRequest = pNet->AllocHttpRequest();
	if (pRequest == nullptr)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "pRequest ==NULL");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	pRequest->req = req;
	pRequest->httpServer = pNet;
	pRequest->timeOut = NFGetSecondTime();

	//uri
	const char* uri = evhttp_request_get_uri(req);
	if (uri == NULL)
	{
		pNet->mListHttpRequestPool.push_back(pRequest);
		NFLogError(NF_LOG_NET_PLUGIN, 0, "uri ==NULL");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	pRequest->url = uri;

	const char* hostname = evhttp_request_get_host(req);
	if (hostname == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "hostname ==NULL");
	}
	else
	{
		pRequest->remoteHost = hostname;
	}

	if (req->uri_elems == NULL)
	{
		//get decodeUri
		struct evhttp_uri* decoded = evhttp_uri_parse(uri);
		if (decoded == NULL)
		{
			pNet->mListHttpRequestPool.push_back(pRequest);
			NFLogError(NF_LOG_NET_PLUGIN, 0, "bad request ");
			evhttp_send_error(req, HTTP_BADREQUEST, 0);
			return;
		}

		//path
		const char* urlPath = evhttp_uri_get_path(decoded);
		if (urlPath != NULL)
		{
			pRequest->path = urlPath;
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "urlPath ==NULL");
		}
		evhttp_uri_free(decoded);
	}
	else
	{
		const char* urlPath = evhttp_uri_get_path(req->uri_elems);
		if (urlPath != NULL)
		{
			pRequest->path = urlPath;
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "urlPath ==NULL");
		}
	}
	
	pRequest->type = (NFHttpType)evhttp_request_get_command(req);

	struct evbuffer *in_evb = evhttp_request_get_input_buffer(req);
	size_t len = evbuffer_get_length(in_evb);
	if (len > 0)
	{
		pRequest->bodySlice = NFSlice((char*)evbuffer_pullup(in_evb, -1), len);
	}

	pNet->mHttpRequestMap.emplace(pRequest->requestId, pRequest);

	if (pNet->mFilter)
	{
		//return 401
		try
		{
			NFWebStatus xWebStatus = pNet->mFilter(pNet->mServerType, *pRequest);
			if (xWebStatus != NFWebStatus::WEB_OK)
			{
				//401
				pNet->ResponseMsg(*pRequest, "Filter error", xWebStatus);
				return;
			}
		}
		catch (std::exception& e)
		{
			pNet->ResponseMsg(*pRequest, e.what(), NFWebStatus::WEB_ERROR);
			return;
		}
		catch (...)
		{
			pNet->ResponseMsg(*pRequest, "UNKNOW ERROR", NFWebStatus::WEB_ERROR);
			return;
		}

	}

	// call cb
	try
	{
		if (pNet->mReceiveCB)
		{
			pNet->mReceiveCB(pNet->mServerType, *pRequest);
			return;
		}
		else
		{
			pNet->ResponseMsg(*pRequest, "NO PROCESSER", NFWebStatus::WEB_ERROR);
			return;
		}
	}
	catch (std::exception& e)
	{
		pNet->ResponseMsg(*pRequest, e.what(), NFWebStatus::WEB_ERROR);
		return;
	}
	catch (...)
	{
		pNet->ResponseMsg(*pRequest, "UNKNOW ERROR", NFWebStatus::WEB_ERROR);
		return;
	}
}

void NFCHttpServer::AddResponseHeader(const NFHttpHandle& req, const std::string& key, const std::string& value)
{
	evhttp_request* pHttpReq = (evhttp_request*)req.req;
	evhttp_add_header(evhttp_request_get_output_headers(pHttpReq), key.data(), value.data());
}

bool NFCHttpServer::ResponseMsg(const NFHttpHandle& req, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	evhttp_request* pHttpReq = (evhttp_request*)req.req;
	//create buffer
	struct evbuffer* eventBuffer = evbuffer_new();

	//send data
	evbuffer_add_printf(eventBuffer, strMsg.c_str());

	AddResponseHeader(req, "Content-Type", "application/json");
	AddResponseHeader(req, "Access-Control-Allow-Origin", "*");

	evhttp_send_reply(pHttpReq, code, strReason.c_str(), eventBuffer);

	//free
	evbuffer_free(eventBuffer);

	auto it = mHttpRequestMap.find(req.requestId);
	if (it != mHttpRequestMap.end())
	{
		it->second->Reset();
		mListHttpRequestPool.push_back(it->second);
		mHttpRequestMap.erase(it);
	}
	return true;
}

bool NFCHttpServer::ResponseMsg(uint64_t requestId, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	NFHttpHandle* req = nullptr;
	auto it = mHttpRequestMap.find(requestId);
	if (it == mHttpRequestMap.end())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "Response Msg Timeout........ requestId:{}, strMsg:{}", requestId, strMsg);
		return false;
	}

	req = it->second;

	evhttp_request* pHttpReq = (evhttp_request*)req->req;
	//create buffer
	struct evbuffer* eventBuffer = evbuffer_new();

	//send data
	evbuffer_add_printf(eventBuffer, strMsg.c_str());

	AddResponseHeader(*req, "Content-Type", "application/json");
	AddResponseHeader(*req, "Access-Control-Allow-Origin", "*");

	evhttp_send_reply(pHttpReq, code, strReason.c_str(), eventBuffer);

	//free
	evbuffer_free(eventBuffer);

	req->Reset();
	mListHttpRequestPool.push_back(req);
	mHttpRequestMap.erase(it);
	return true;
}

#if NF_ENABLE_SSL
bool NFCHttpServer::EnableSSL() const
{
	return mEnableSSL;
}

void NFCHttpServer::SetEnableSSL(bool enable)
{ 
	mEnableSSL = enable; 
}

const std::string& NFCHttpServer::certificate_chain_file() const
{
	return certificate_chain_file_;
}
const std::string& NFCHttpServer::private_key_file() const
{
	return private_key_file_;
}
void NFCHttpServer::set_certificate_chain_file(const std::string& filename)
{
	certificate_chain_file_ = filename;
}
void NFCHttpServer::set_private_key_file(const std::string& filename)
{
	private_key_file_ = filename;
}
#endif