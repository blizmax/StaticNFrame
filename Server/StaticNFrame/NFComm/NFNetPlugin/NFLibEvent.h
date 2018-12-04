// -------------------------------------------------------------------------
//    @FileName         :    NFLibEvent.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/queue.h>
#endif

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/http.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <event2/event_compat.h>
#include <event2/dns.h>
#include <event2/dns_compat.h>
#include <event2/dns_struct.h>
#include <event2/listener.h>
#include "event2/bufferevent_ssl.h"
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/bufferevent_struct.h>

#if NF_ENABLE_SSL

#ifdef  __cplusplus
extern "C" {
#endif
#include <openssl/ssl.h>
#include <openssl/err.h>
#ifdef  __cplusplus
}
#endif

#endif

