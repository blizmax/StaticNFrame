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
#include "NFNetBase.h"

extern char g_errstr[1024];
#define ERRNO   (NFSocket::GetSocketError())
#define ERRSTR  (NFSocket::GetSocketErrorMsg(ERRNO,  \
                 g_errstr,                   \
                 sizeof(g_errstr)))

class NFSocket
{
public:
	static int GetSocketError();
	static char* GetSocketErrorMsg(int err_no, char* buf, size_t len);
	static SOCKET Create(int nodelay_flag = 1);
	static bool Close(SOCKET sock);
	static bool Shutdown(SOCKET sock, int how);
	static bool SetReuse(SOCKET sock);
	static bool SetNonBlock(SOCKET sock);
	static bool SetNoDelay(SOCKET sock, int flag = 1);
	static int Accept(SOCKET listen_sock, SOCKET* sock);
	static bool Connect(SOCKET sock, const char* ip, uint16_t port);
	static bool Listen(SOCKET sock, const char* ip, uint16_t port);
	static int Send(SOCKET sock, const char* buf, uint32_t len);
	static int Recv(SOCKET sock, char* buf, uint32_t size);
	static bool GetPeerAddress(SOCKET sock, char* IPBuffer, uint32_t IPBufferSize, uint16_t* port);
	static int IpStrToInt(const char* ip);
	static int TpIntToStr(int ip, std::string* ip_str);
	static bool GetMac(char* mac);
protected:
	NFSocket();
	virtual ~NFSocket();
};
