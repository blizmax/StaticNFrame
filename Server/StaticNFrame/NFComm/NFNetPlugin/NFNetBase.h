// -------------------------------------------------------------------------
//    @FileName         :    NFNetBase.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include <stdint.h>
#include <string>
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFSimpleBuffer.h"
#include "NFComm/NFCore/NFDataStream.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#include <mstcpip.h>
#include <iphlpapi.h>
#include <mswsock.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <pthread.h>
#endif

#if NF_PLATFORM == NF_PLATFORM_WIN
#define ArkGetLastError		    WSAGetLastError
typedef int Socklen_t;
#define WIN32_LEAN_AND_MEAN
#else
#define SOCKET				int
#define ArkGetLastError()	errno
#define closesocket			close
#define ioctlsocket			ioctl
typedef struct linger 		    LINGER;
#define SOCKET_ERROR		-1
#define INVALID_SOCKET		-1
#define SD_SEND				SHUT_WR
#endif

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <windows.h>
#include <mstcpip.h>
#include <iphlpapi.h>
#include <mswsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <pthread.h>
#endif  // OS

#if NF_PLATFORM == NF_PLATFORM_WIN
#define ERR_INTER       WSAEINTR
#define ERR_EAGAIN      WSAEWOULDBLOCK
#define ERR_INPROGRESS  WSAEINPROGRESS
#define SHUTDOWN_RECV   SD_RECEIVE
#define SHUTDOWN_SEND   SD_SEND
#define SHUTDOWN_BOTH   SD_BOTH
#else  // defined(OS_LINUX)

#include <sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h> 

#define ERR_INTER       EINTR
#define ERR_EAGAIN      EAGAIN
#define ERR_INPROGRESS  EINPROGRESS
#define SHUTDOWN_RECV   SHUT_RD
#define SHUTDOWN_SEND   SHUT_WR
#define SHUTDOWN_BOTH   SHUT_RDWR
#endif  // OS

enum PollType
{
	POLL_TYPE_NULL = 0,
	POLL_TYPE_READ = 1,
	POLL_TYPE_WRITE = 2,
};

enum SocketFlag
{
	SOCKET_LISTEN,
	SOCKET_ACCEPT,
	SOCKET_CONNECT,
};

enum SessionState
{
	SESSION_STATE_NULL = 0,
	SESSION_STATE_WORKING,    // working
	SESSION_STATE_SHUTDOWN,   // app lay shutdown(grace close socket)
};

enum EventFlag
{
	EVENT_NULL = 0,
	EVENT_READ = 1,
	EVENT_WRITE = 1 << 1,
	EVENT_RANDW = EVENT_READ | EVENT_WRITE,
};

struct EventData;

class EventHandle
{
	typedef std::function<void(EventData*)> EventHandler;
public:
	EventHandle()
	{
		mReadHandler = EventHandler();
		mWriteHandler = EventHandler();
		mErrorHandler = EventHandler();
	}
	virtual ~EventHandle() {}

	EventHandler	mReadHandler;
	EventHandler	mWriteHandler;
	EventHandler	mErrorHandler;
};

struct EventData
{
	EventHandle      handle;
	SocketFlag  sock_flag;
	EventFlag   event_flag;
	union
	{
		struct
		{
			SOCKET   sock;
			uint16_t port;
		};
		void*        session;
	};

	EventData()
	{
		sock_flag = SOCKET_ACCEPT;
		event_flag = EVENT_NULL;
		sock = INVALID_SOCKET;
		port = 0;
	}
};

struct EventDataEx : public EventData
{
	uint32_t  sendBufSize;
	uint32_t  recvBufSize;

	EventDataEx()
		: EventData()
		, sendBufSize(0)
		, recvBufSize(0)
	{}
};

