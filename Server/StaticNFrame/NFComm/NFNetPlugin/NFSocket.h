// -------------------------------------------------------------------------
//    @FileName         :    NFSocket.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFNetDefine.h"

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

extern char g_errstr[1024];
#define ERRNO   (Socket::GetSocketError())
#define ERRSTR  (Socket::GetSocketErrorMsg(ERRNO,  \
                 g_errstr,                   \
                 sizeof(g_errstr)))

class Socket
{
public:
	static int    GetSocketError();
	static char*  GetSocketErrorMsg(int err_no, char *buf, size_t len);
	static SOCKET Create(int nodelay_flag = 1);
	static bool   Close(SOCKET sock);
	static bool   Shutdown(SOCKET sock, int how);
	static bool   SetReuse(SOCKET sock);
	static bool   SetNonBlock(SOCKET sock);
	static bool   SetNoDelay(SOCKET sock, int flag = 1);
	static int    Accept(SOCKET listen_sock, SOCKET* sock);
	static bool   Connect(SOCKET sock, const char* ip, uint16_t port);
	static bool   Listen(SOCKET sock, const char* ip, uint16_t port);
	static int    Send(SOCKET sock, const char* buf, uint32_t len);
	static int    Recv(SOCKET sock, char* buf, uint32_t size);
	static bool   GetPeerAddress(SOCKET sock, char* IPBuffer, uint32_t IPBufferSize, uint16_t* port);
	static int    IpStrToInt(const char* ip);
	static int    TpIntToStr(int ip, std::string* ip_str);
	static bool   GetMac(char* mac);
protected:
	Socket();
	virtual ~Socket();
};