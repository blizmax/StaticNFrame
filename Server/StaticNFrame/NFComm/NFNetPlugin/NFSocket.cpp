// -------------------------------------------------------------------------
//    @FileName         :    NFSocket.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFSocket.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#if NF_PLATFORM == NF_PLATFORM_WIN

#include <errno.h>
#include <stdio.h>

#pragma warning(disable : 4127)
#pragma warning(disable : 4018)
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

static class WinSocketSetup
{
public:
	WinSocketSetup(unsigned char minor = 2, unsigned char major = 2)
	{
		WSADATA  data;
		uint16_t version = MAKEWORD(minor, major);

		if (WSAStartup(version, &data) != 0)
		{
			assert(false);
			NFLogError("[Net] WSAStartup failed");
			exit(0);
		}
	}
	~WinSocketSetup()
	{
		WSACleanup();
	}
}s_socketSetup;

int NFSocket::GetSocketError()
{
	return WSAGetLastError();
}

char* NFSocket::GetSocketErrorMsg(int err_no, char *buf, size_t len)
{
	assert(buf);
	LPVOID msg_buf = NULL;
	int ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		(DWORD)err_no,
		//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // log output format is UTF8, use English not need covert
		(LPTSTR)&msg_buf,
		0, NULL);

	if (ret > 0 && msg_buf != NULL && len > 0)
	{
		strncpy(buf, reinterpret_cast<const char*>(msg_buf), len);
		buf[len - 1] = '\0';
		LocalFree(msg_buf);
	}
	return buf;
}

SOCKET NFSocket::Create(int nodelay_flag /*= 1*/)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock != INVALID_SOCKET)
	{
		if (!SetNoDelay(sock, nodelay_flag) || !SetNonBlock(sock))
		{
			Close(sock);
			sock = INVALID_SOCKET;
		}
	}
	return sock;
}

bool NFSocket::Close(SOCKET sock)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
	}
	return true;
}

bool NFSocket::Shutdown(SOCKET sock, int how)
{
	assert(sock != INVALID_SOCKET);
	if (sock != INVALID_SOCKET)
	{
		shutdown(sock, how);
	}
	return true;
}

bool NFSocket::SetReuse(SOCKET sock)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		int flag = 1;
		int ret = setsockopt(sock,
			SOL_SOCKET,
			SO_REUSEADDR,
			reinterpret_cast<char*>(&flag),
			sizeof(flag));
		if (ret == SOCKET_ERROR)
		{
			NFLogError("[Net] set socket reuse flag(%d) error(%d):%s",
				flag,
				ERRNO,
				ERRSTR);
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool NFSocket::SetNonBlock(SOCKET sock)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		unsigned long flag = 1;
		int ret = ioctlsocket(sock, FIONBIO, &flag);
		if (ret == SOCKET_ERROR)
		{
			NFLogError("[Net] set socket noblock flag(%ul) error(%d):%s",
				flag,
				ERRNO,
				ERRSTR);
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool NFSocket::SetNoDelay(SOCKET sock, int flag /*= 1*/)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		int ret = setsockopt(sock,
			IPPROTO_TCP,
			TCP_NODELAY,
			reinterpret_cast<char*>(&flag),
			sizeof(flag));
		if (ret == SOCKET_ERROR)
		{
			NFLogError("[Net] set socket delay flag(%d) error(%d):%s",
				flag,
				ERRNO,
				ERRSTR);
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

int NFSocket::Accept(SOCKET listen_sock, SOCKET* sock)
{
	assert(listen_sock);

	int len = sizeof(sockaddr_in);
	struct sockaddr_in addr;
	*sock = accept(listen_sock, reinterpret_cast<sockaddr*>(&addr), &len);
	if (*sock == INVALID_SOCKET)
	{
		if (ERRNO == ERR_EAGAIN)
		{
			return 1;
		}
		NFLogError("[Net] Accept error(%d):%s",
			ERRNO,
			ERRSTR);
		return -1;
	}
	else
	{
		return 0;
	}
}

bool NFSocket::Connect(SOCKET sock, const char* ip, uint16_t port)
{
	assert(sock != INVALID_SOCKET);
	assert(ip);

	//if (!SetReuse(sock)) {
	//  return false;
	//}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	int retCode = connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

	if ((retCode < 0) && (ERRNO != ERR_EAGAIN && ERRNO != ERR_INPROGRESS))
	{
		NFLogError("[Net] connect addr(%s:%u) error(%d):%s", ip, port, ERRNO, ERRSTR);
		return false;
	}
	return true;
}

bool NFSocket::Listen(SOCKET sock, const char* ip, uint16_t port)
{
	assert(sock != INVALID_SOCKET);
	assert(ip);

	//if (!SetReuse(sock)) {
	//  return false;
	//}

	//TODO 修改返回值
	if (!SetNonBlock(sock))
	{
		return false;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	int ret = ::bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		NFLogError("[Net] bind socket error(%d):%s", ERRNO, ERRSTR);
		return false;
	}

	ret = listen(sock, 2048);
	if (ret == SOCKET_ERROR)
	{
		NFLogError("[Net] bind socket error(%d):%s", ERRNO, ERRSTR);
		return false;
	}
	return true;
}

//返回值有意义：字长
int NFSocket::Send(SOCKET sock, const char* buf, uint32_t len)
{
	do
	{
		int ret = send(sock, buf, len, 0);
		if (ret != SOCKET_ERROR)
			return ret;

		if (ERRNO == ERR_INTER)
		{
			continue;
		}
		else
		{
			if (ERRNO == ERR_EAGAIN || ERRNO == ERR_INPROGRESS)
			{
				return 0;
			}
			else
			{
				NFLogError("[Net] socket send error(%d):%s", ERRNO, ERRSTR);
				return -1;
			}
		}
	} while (true);
}

int NFSocket::Recv(SOCKET sock, char* buf, uint32_t size)
{
	do
	{
		int ret = recv(sock, buf, size, 0);
		if (ret != SOCKET_ERROR)
			return ret;

		if (ERRNO == ERR_INTER)
		{
			continue;
		}
		else
		{
			if (ERRNO == ERR_EAGAIN || ERRNO == ERR_INPROGRESS)
			{
				return -2;
			}
			else
			{
				NFLogError("[Net] socket recv error(%d):%s", ERRNO, ERRSTR);
				return -1;
			}
		}
	} while (true);
}

bool NFSocket::GetPeerAddress(SOCKET sock, char* IPBuffer, uint32_t IPBufferSize, uint16_t* port)
{
	bool result = false;
	int retCode = -1;
	struct sockaddr_in addr;
	int length = sizeof(addr);
	char* IP = nullptr;

	retCode = getpeername(sock, reinterpret_cast<sockaddr*>(&addr), &length);
	if (retCode == SOCKET_ERROR)
	{
		NFLogError("[Net] get socket addr error(%d):%s", ERRNO, ERRSTR);
		goto Exit0;
	}

	IP = inet_ntoa(addr.sin_addr);
	length = (int)strlen(IP);
	assert(IPBufferSize > length);

	memcpy(IPBuffer, IP, length + 1);
	*port = ntohs(addr.sin_port);

	result = true;
Exit0:
	return result;
}


#else

#include <errno.h>
#include <string.h>
#include <net/if.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int NFSocket::GetSocketError()
{
	return errno;     // errno is thread-local
}

char* NFSocket::GetSocketErrorMsg(int err_no, char *buf, size_t len)
{
	assert(buf);
	assert(len > 0);

	strerror_r(err_no, buf, len);
	return buf;
}

SOCKET NFSocket::Create(int nodelay_flag /*= 1*/)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock != INVALID_SOCKET)
	{
		if (!SetNoDelay(sock, nodelay_flag) || !SetNonBlock(sock))
		{
			Close(sock);
			sock = INVALID_SOCKET;
		}
	}
	return sock;
}

bool NFSocket::Close(SOCKET sock)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		close(sock);
	}
	return true;
}

bool NFSocket::Shutdown(SOCKET sock, int how)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		int ret = shutdown(sock, how);
		if (ret < 0)
		{
			NFLogError(
				"shutdown socket error(%d):%s", ERRNO, ERRSTR
			);
			return false;
		}
	}
	return true;
}

bool NFSocket::SetReuse(SOCKET sock)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		int flag = 1;
		int ret = setsockopt(sock,
			SOL_SOCKET,
			SO_REUSEADDR,
			(char*)&flag,
			sizeof(flag));
		if (ret < 0)
		{
			NFLogError("[Net] set socket reuse flag(%d) error(%d):%s",
				flag,
				ERRNO,
				ERRSTR);
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool NFSocket::SetNonBlock(SOCKET sock)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		int flag = fcntl(sock, F_GETFL, 0);
		int ret = fcntl(sock, F_SETFL, flag | O_NONBLOCK);
		if (ret < 0)
		{
			NFLogError("[Net] set socket noblock flag(%d) error(%d):%s",
				flag,
				ERRNO,
				ERRSTR);
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool NFSocket::SetNoDelay(SOCKET sock, int flag /*= 1*/)
{
	assert(sock != INVALID_SOCKET);

	if (sock != INVALID_SOCKET)
	{
		int ret = setsockopt(sock,
			IPPROTO_TCP,
			TCP_NODELAY,
			(char*)&flag,
			sizeof(flag));
		if (ret < 0)
		{
			NFLogError("[Net] set socket delay flag(%d) error(%d):%s",
				flag,
				ERRNO,
				ERRSTR);
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

//返回值有意义：三种类型不适合做bool
int NFSocket::Accept(SOCKET listen_sock, SOCKET* sock)
{
	assert(listen_sock);

	int len = 0;
	struct sockaddr_in addr;
	*sock = accept(listen_sock,
		reinterpret_cast<sockaddr*>(&addr),
		reinterpret_cast<socklen_t *>(&len));
	if (*sock == INVALID_SOCKET)
	{
		if (ERRNO == ERR_EAGAIN)
		{
			return 1;
		}
		NFLogError("[Net] Accept error(%d):%s",
			ERRNO,
			ERRSTR);
		return -1;
	}
	else
	{
		return 0;
	}
}

bool NFSocket::Connect(SOCKET sock, const char* ip, uint16_t port)
{
	assert(sock != INVALID_SOCKET);
	assert(ip);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	int retCode = connect(sock,
		reinterpret_cast<sockaddr*>(&addr),
		sizeof(addr));

	if ((retCode < 0) && (ERRNO != ERR_EAGAIN && ERRNO != ERR_INPROGRESS))
	{
		NFLogError("[Net] connect addr(%s:%d) error(%d):%s",
			ip,
			port,
			ERRNO,
			ERRSTR);
		return false;
	}
	return true;
}

bool NFSocket::Listen(SOCKET sock, const char* ip, uint16_t port)
{
	assert(sock != INVALID_SOCKET);
	assert(ip);

	if (!SetReuse(sock))
	{
		return false;
	}

	//TODO 修改listen
	if (!SetNonBlock(sock))
	{
		return false;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	//MMOTODO 修改返回值
	int ret = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (ret < 0)
	{
		NFLogError("[Net] bind socket error(%d):%s",
			ERRNO,
			ERRSTR);
		return false;
	}

	ret = listen(sock, 2048);
	if (ret < 0)
	{
		NFLogError("[Net] listen error(%d):%s",
			ERRNO,
			ERRSTR);
		return false;
	}
	return true;
}

//返回值有意义：字长
int NFSocket::Send(SOCKET sock, const char* buf, uint32_t len)
{
	do
	{
		int ret = send(sock, buf, len, MSG_NOSIGNAL | MSG_DONTWAIT);
		if (ret >= 0)
			return ret;

		if (ERRNO == ERR_INTER)
		{
			continue;
		}
		else
		{
			if (ERRNO == ERR_EAGAIN || ERRNO == ERR_INPROGRESS)
			{
				return 0;
			}
			else
			{
				NFLogError("[Net] socket send error(%d):%s",
					ERRNO,
					ERRSTR);
				return -1;
			}
		}
	} while (true);
}

int NFSocket::Recv(SOCKET sock, char* buf, uint32_t size)
{
	do
	{
		int ret = recv(sock, buf, size, MSG_DONTWAIT);
		if (ret >= 0)
			return ret;

		if (ERRNO == ERR_INTER)
		{
			continue;
		}
		else
		{
			if (ERRNO == ERR_EAGAIN || ERRNO == ERR_INPROGRESS)
			{
				return -2;
			}
			else
			{
				NFLogError("[Net] socket recv error(%d):%s",
					ERRNO,
					ERRSTR);
				return -1;
			}
		}
	} while (true);
}

bool NFSocket::GetPeerAddress(SOCKET sock, char* IPBuffer, uint32_t IPBufferSize, uint16_t* port)
{
	bool result = false;
	int retCode = -1;
	struct sockaddr_in addr;
	socklen_t length = sizeof(addr);
	char* IP = nullptr;

	assert(sock != INVALID_SOCKET);
	assert(IPBuffer);

	retCode = getpeername(sock, reinterpret_cast<sockaddr*>(&addr), &length);
	if (retCode == SOCKET_ERROR)
	{
		NFLogError("[Net] get socket addr error(%d):%s", ERRNO, ERRSTR);
		goto Exit0;
	}

	IP = inet_ntoa(addr.sin_addr);
	length = strlen(IP);
	assert(IPBufferSize > length);

	memcpy(IPBuffer, IP, length + 1);
	*port = ntohs(addr.sin_port);

	result = true;
Exit0:
	return result;
}


#endif