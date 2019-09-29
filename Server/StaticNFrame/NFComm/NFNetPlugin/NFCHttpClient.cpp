// -------------------------------------------------------------------------
//    @FileName         :    NFHttpClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFCHttpClient.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include <openssl/x509v3.h>

bool NFCHttpClient::Execute()
{
	return true;
}

bool NFCHttpClient::Init()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return false;
	}
#endif

#if NF_ENABLE_SSL
	SSL_library_init();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	m_pSslCtx = SSL_CTX_new(SSLv23_method());
	if (!m_pSslCtx)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "SSL_CTX_new err.");
		return false;
	}
	X509_STORE* store = SSL_CTX_get_cert_store(m_pSslCtx);
	if (X509_STORE_set_default_paths(store) != 1) {
		NFLogError(NF_LOG_NET_PLUGIN, 0, "X509_STORE_set_default_paths failed");
		return false;
	}
#endif

	m_pBase = event_base_new();
	if (m_pBase == nullptr)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "event_base_new err.");
		return false;
	}
	return true;
}

bool NFCHttpClient::Final()
{
	if (m_pBase)
	{
		event_base_free(m_pBase);
		m_pBase = nullptr;
	}

#if NF_ENABLE_SSL
	if (m_pSslCtx)
	{
		SSL_CTX_free(m_pSslCtx);
		m_pSslCtx = nullptr;
	}

	ERR_free_strings();
	EVP_cleanup();
	ERR_remove_thread_state(nullptr);
	CRYPTO_cleanup_all_ex_data();
#endif
	return true;
}

bool NFCHttpClient::MakeRequest(const std::string& strUri,
	const std::string& strPostData,
	const std::map<std::string, std::string>& xHeaders,
	const NFHttpType eHttpType, int& respCode, std::string& strResp)
{
	respCode = 0;
	strResp = "";
	m_respCode = 0;
	m_strResp = "";
	struct evhttp_uri* http_uri = evhttp_uri_parse(strUri.c_str());
	if (http_uri == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_uri_parse err. ");
		return false;
	}

	const char*  scheme = evhttp_uri_get_scheme(http_uri);
	if (scheme == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "scheme == NULL err. ");
		return false;
	}

	std::string lowwerScheme(scheme);
	NFStringUtility::ToLower(lowwerScheme);
	if (lowwerScheme.compare("https") != 0 && lowwerScheme.compare("http") != 0)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "scheme == NULL err. ");
		return false;
	}

	bool isHttps = false;
	if (lowwerScheme.compare("https") == 0)
	{
		isHttps = true;
	}

	const char* host = evhttp_uri_get_host(http_uri);
	if (host == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "url must have a host err. ");
		return false;
	}

	int port = evhttp_uri_get_port(http_uri);
	if (port == -1)
	{
		port = isHttps ? 443 : 80;
	}

	const char* path = evhttp_uri_get_path(http_uri);
	if (path == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}

		NFLogError(NF_LOG_NET_PLUGIN, 0, "path == NUL err. ");
		return false;
	}

	std::string uri = path;
	if (strlen(path) == 0)
	{
		uri = "/";
	}

	const char* query = evhttp_uri_get_query(http_uri);
	if (query && strlen(query) > 0)
	{
		uri += "?";
		uri += query;
	}

	struct bufferevent* bev = NULL;

	if (!isHttps)
	{
		bev = bufferevent_socket_new(m_pBase, -1, BEV_OPT_CLOSE_ON_FREE);
	}
	else
	{
#if NF_ENABLE_SSL
		SSL* pSSL = SSL_new(m_pSslCtx);
		if (pSSL == NULL)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "SSL_new err.");
			return false;
		}
		SSL_set_tlsext_host_name(pSSL, host);
		X509_VERIFY_PARAM* param = SSL_get0_param(pSSL);
		X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
		X509_VERIFY_PARAM_set1_host(param, host, 0);
		SSL_set_verify(pSSL, SSL_VERIFY_PEER, nullptr);

		bev = bufferevent_openssl_socket_new(m_pBase, -1, pSSL,
			BUFFEREVENT_SSL_CONNECTING,
			BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
#endif
	}

	if (bev == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, " bev == NUL err. ");
		return false;
	}

#if NF_ENABLE_SSL
	if (isHttps)
	{
		bufferevent_openssl_set_allow_dirty_shutdown(bev, 1);
	}
#endif

	struct evhttp_connection* evcon = evhttp_connection_base_bufferevent_new(m_pBase, NULL, bev, host, port);
	if (evcon == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, " evcon == NUL err. ");
		return false;
	}

	if (m_nRetry > 0)
	{
		evhttp_connection_set_retries(evcon, m_nRetry);
	}

	if (m_nTimeOut >= 0)
	{
		evhttp_connection_set_timeout(evcon, m_nTimeOut);
	}

	// Fire off the request
	struct evhttp_request* req = evhttp_request_new(OnHttpReqDone, this);
	if (req == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_request_new() failed\n");
		return false;
	}

	struct evkeyvalq* output_headers = evhttp_request_get_output_headers(req);
	if (output_headers == NULL)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		evhttp_request_free(req);
		NFLogError(NF_LOG_NET_PLUGIN, 0, "output_headers == NULL err. ");
		return false;
	}

	if (evhttp_add_header(output_headers, "Host", host))
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		evhttp_request_free(req);
		NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_add_header host failed. ");
		return false;
	}

	if (evhttp_add_header(output_headers, "Connection", "close"))
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		evhttp_request_free(req);
		NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_add_header Connection failed. ");
		return false;
	}

	for (auto it = xHeaders.cbegin(); it != xHeaders.end(); it++)
	{
		if (evhttp_add_header(output_headers, it->first.c_str(), it->second.c_str()))
		{
			if (http_uri)
			{
				evhttp_uri_free(http_uri);
			}
			evhttp_request_free(req);
			NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_add_header failed. ");
			return false;
		}
	}

	size_t nLen = strPostData.length();
	if (nLen > 0)
	{
		struct evbuffer* output_buffer = evhttp_request_get_output_buffer(req);
		if (output_buffer == NULL)
		{
			if (http_uri)
			{
				evhttp_uri_free(http_uri);
			}
			evhttp_request_free(req);
			NFLogError(NF_LOG_NET_PLUGIN, 0, "output_buffer == NUL err. ");
			return false;
		}

		if (evbuffer_add(output_buffer, strPostData.c_str(), nLen)) 
		{
			if (http_uri)
			{
				evhttp_uri_free(http_uri);
			}
			evhttp_request_free(req);
			NFLogError(NF_LOG_NET_PLUGIN, 0, "evbuffer_add failed. ");
			return false;
		}
		char buf[256] = { 0 };
		evutil_snprintf(buf, sizeof(buf) - 1, "%lu", (unsigned long)nLen);
		if (evhttp_add_header(output_headers, "Content-Length", buf))
		{
			if (http_uri)
			{
				evhttp_uri_free(http_uri);
			}
			evhttp_request_free(req);
			NFLogError(NF_LOG_NET_PLUGIN, 0, "evbuffer_add failed. ");
			return false;
		}
	}

	int r_ = evhttp_make_request(evcon, req, (evhttp_cmd_type)eHttpType, uri.c_str());
	if (r_ != 0)
	{
		if (http_uri)
		{
			evhttp_uri_free(http_uri);
		}
		NFLogError(NF_LOG_NET_PLUGIN, 0, "evhttp_make_request() failed\n");
		return false;
	}

	if (m_pBase)
	{
		event_base_dispatch(m_pBase);
	}

	respCode = m_respCode;
	strResp = m_strResp;

	if (http_uri)
	{
		evhttp_uri_free(http_uri);
	}
	return true;
}

bool NFCHttpClient::PerformGet(const std::string& strUri,
	const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp)
{
	return MakeRequest(strUri, "", xHeaders, NFHttpType::NF_HTTP_REQ_GET, respCode, strResp);
}

bool NFCHttpClient::PerformPost(const std::string& strUri, const std::string& strPostData,
	const std::map<std::string, std::string>& xHeaders, int& respCode, std::string& strResp)
{
	return MakeRequest(strUri, strPostData, xHeaders, NFHttpType::NF_HTTP_REQ_POST, respCode, strResp);
}

void NFCHttpClient::OnHttpReqDone(struct evhttp_request* req, void* ctx)
{
	NFCHttpClient* pHttpClient = (NFCHttpClient*)(ctx);
	if (pHttpClient == NULL)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "pHttpClient ==NULL");
		return;
	}

	if (req == NULL)
	{
		/* If req is NULL, it means an error occurred, but
		* sadly we are mostly left guessing what the error
		* might have been.  We'll do our best... */
		struct bufferevent* bev = (struct bufferevent*) pHttpClient->m_pBase;
		unsigned long oslerr = 0;
		int printed_err = 0;
		int errcode = EVUTIL_SOCKET_ERROR();

		std::string strErrMsg = "";
		NFLogError(NF_LOG_NET_PLUGIN, 0, "some request failed - no idea which one though!\n");
		/* Print out the OpenSSL error queue that libevent
		* squirreled away for us, if any. */

		char buffer[1024] = { 0 };
		int nread = 0;

#if NF_ENABLE_SSL
		while ((oslerr = bufferevent_get_openssl_error(bev))) {
			ERR_error_string_n(oslerr, buffer, sizeof(buffer));
			NFLogError(NF_LOG_NET_PLUGIN, 0, buffer);
			//fprintf(stderr, "%s\n", buffer);
			strErrMsg += std::string(buffer);
			printed_err = 1;
		}
#endif
		/* If the OpenSSL error queue was empty, maybe it was a
		* socket error; let's try printing that. */
		if (!printed_err)
		{
			//char tmpBuf[1024] = { 0 };
			//snprintf(tmpBuf, 1024, "socket error = %s (%d)\n",
			//	evutil_socket_error_to_string(errcode),
			//	errcode);
			std::string errMsg = NF_FORMAT("socket error = {} ({})\n", evutil_socket_error_to_string(errcode), errcode);
			strErrMsg += errMsg;
		}
		pHttpClient->m_respCode = errcode;
		pHttpClient->m_strResp = "";
		NFLogError(NF_LOG_NET_PLUGIN, 0, strErrMsg.c_str());
		return;
	}

	pHttpClient->m_respCode = evhttp_request_get_response_code(req);
	struct evbuffer* evbuf = evhttp_request_get_input_buffer(req);
	size_t buffer_size = evbuffer_get_length(evbuf);
	if (buffer_size > 0) {
		pHttpClient->m_strResp = NFSlice((char*)evbuffer_pullup(evbuf, -1), buffer_size).ToString();
	}

	if (req->evcon)
	{
		evhttp_connection_free(req->evcon);
	}

	if (req->output_headers)
	{
		evhttp_clear_headers(req->output_headers);
	}
}