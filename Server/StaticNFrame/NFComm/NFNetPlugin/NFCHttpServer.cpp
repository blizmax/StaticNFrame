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
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

NFCHttpServer::~NFCHttpServer()
{
	if (base)
	{
		event_base_free(base);
		base = NULL;
	}
}

bool NFCHttpServer::Execute()
{
	if (base)
	{
		event_base_loop(base, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}

	return true;
}


int NFCHttpServer::InitServer(const unsigned short port)
{
	mPort = port;
	//struct event_base *base;
	struct evhttp* http;
	struct evhttp_bound_socket* handle;

#if NF_PLATFORM == NF_PLATFORM_WIN
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		return (1);
	}
#endif

	base = event_base_new();
	if (!base)
	{
		NFLogError("create event_base fail");
		return 1;
	}

	http = evhttp_new(base);
	if (!http)
	{
		NFLogError("create evhttp fail");
		return 1;
	}

	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", mPort);
	if (!handle)
	{
		NFLogError("http server bind port :{} fail!", mPort);
		return 1;
	}

	evhttp_set_gencb(http, listener_cb, (void*) this);
	return 0;
}


void NFCHttpServer::listener_cb(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		NFLogError("req ==NULL");
		return;
	}

	NFCHttpServer* pNet = (NFCHttpServer*)arg;
	if (pNet == NULL)
	{
		NFLogError("pNet ==NULL");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	NFHttpRequest request;
	request.req = req;

	//headers

	struct evkeyvalq * header = evhttp_request_get_input_headers(req);
	if (header == NULL)
	{
		NFLogError("header ==NULL");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	rapidjson::Document headerDoc;
	headerDoc.SetObject();

	struct evkeyval* kv = header->tqh_first;
	while (kv)
	{
		request.headers.emplace(kv->key, kv->value);
		headerDoc.AddMember(rapidjson::StringRef(kv->key), rapidjson::StringRef(kv->value), headerDoc.GetAllocator());
		kv = kv->next.tqe_next;
	}

	rapidjson::StringBuffer headerbuffer;
	rapidjson::Writer<rapidjson::StringBuffer> headerWriter(headerbuffer);
	headerDoc.Accept(headerWriter);
	request.json_params = headerbuffer.GetString();

	//uri
	const char* uri = evhttp_request_get_uri(req);
	if (uri == NULL)
	{
		NFLogError("uri ==NULL");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	request.url = uri;
	const char* hostname = evhttp_request_get_host(req);
	if (hostname == NULL)
	{
		NFLogError("hostname ==NULL");
	}
	else
	{
		request.remoteHost = hostname;
	}

	request.type = (NFHttpType)evhttp_request_get_command(req);

	//get decodeUri
	struct evhttp_uri* decoded = evhttp_uri_parse(uri);
	if (decoded == NULL)
	{
		NFLogError("bad request ");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	//path
	const char* urlPath = evhttp_uri_get_path(decoded);
	if (urlPath != NULL)
	{
		request.path = urlPath;
	}
	else
	{
		NFLogError("urlPath ==NULL");
	}

	evhttp_uri_free(decoded);

	//std::cout << "Got a GET request:" << uri << std::endl;
	if (evhttp_request_get_command(req) == evhttp_cmd_type::EVHTTP_REQ_GET)
	{
		//OnGetProcess(request, );
		NFLogDebug("EVHTTP_REQ_GET");

		rapidjson::Document paramDoc;
		paramDoc.SetObject();

		struct evkeyvalq params;
		evhttp_parse_query(uri, &params);
		struct evkeyval* kv = params.tqh_first;
		while (kv)
		{
			request.params.emplace(kv->key, kv->value);
			paramDoc.AddMember(rapidjson::StringRef(kv->key), rapidjson::StringRef(kv->value), paramDoc.GetAllocator());
			kv = kv->next.tqe_next;
		}

		rapidjson::StringBuffer parambuffer;
		rapidjson::Writer<rapidjson::StringBuffer> paramWriter(parambuffer);
		paramDoc.Accept(paramWriter);
		request.json_params = parambuffer.GetString();
	}

	struct evbuffer *in_evb = evhttp_request_get_input_buffer(req);
	if (in_evb == NULL)
	{
		NFLogError("urlPath ==NULL");
		return;
	}

	size_t len = evbuffer_get_length(in_evb);
	if (len > 0)
	{
		unsigned char *pData = evbuffer_pullup(in_evb, len);
		request.body.clear();

		if (pData != NULL)
		{
			request.body.append((const char *)pData, len);
		}
	}

	if (pNet->mFilter)
	{
		//return 401
		try
		{
			NFWebStatus xWebStatus = pNet->mFilter(request);
			if (xWebStatus != NFWebStatus::WEB_OK)
			{
				//401
				pNet->ResponseMsg(request, "Filter error", xWebStatus);
				return;
			}
		}
		catch (std::exception& e)
		{
			pNet->ResponseMsg(request, e.what(), NFWebStatus::WEB_ERROR);
		}
		catch (...)
		{
			pNet->ResponseMsg(request, "UNKNOW ERROR", NFWebStatus::WEB_ERROR);
		}

	}

	// call cb
	try
	{
		if (pNet->mReceiveCB)
		{
			pNet->mReceiveCB(request);
		}
		else
		{
			pNet->ResponseMsg(request, "NO PROCESSER", NFWebStatus::WEB_ERROR);
		}
	}
	catch (std::exception& e)
	{
		pNet->ResponseMsg(request, e.what(), NFWebStatus::WEB_ERROR);
	}
	catch (...)
	{
		pNet->ResponseMsg(request, "UNKNOW ERROR", NFWebStatus::WEB_ERROR);
	}
}

bool NFCHttpServer::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
	const std::string& strReason)
{
	evhttp_request* pHttpReq = (evhttp_request*)req.req;
	//create buffer
	struct evbuffer* eventBuffer = evbuffer_new();

	//send data
	evbuffer_add_printf(eventBuffer, strMsg.c_str());

	evhttp_add_header(evhttp_request_get_output_headers(pHttpReq), "Content-Type", "application/json");

	evhttp_send_reply(pHttpReq, code, strReason.c_str(), eventBuffer);

	//free
	evbuffer_free(eventBuffer);

	return true;
}