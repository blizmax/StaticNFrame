// -------------------------------------------------------------------------
//    @FileName         :    NFNetDefine.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                    _ooOoo_
//                   o8888888o
//                   88" . "88
//                   (| -_- |)
//                    O\ = /O
//                ____/`---'\____
//              .   ' \\| |// `.
//               / \\||| : |||// \
//             / _||||| -:- |||||- \
//               | | \\\ - /// | |
//            | \_| ''\---/'' | |
//              \ .-\__ `-` ___/-. /
//           ___`. .' /--.--\ `. . __
//        ."" '< `.___\_<|>_/___.' >'"".
//       | | : `- \`.;`\ _ /`;.`/ - ` : | |
//         \ \ `-. \_ __\ /__ _/ .-` / /
// ======`-.____`-.___\_____/___.-`____.-'======
//                    `=---='
//
// .............................................
//          ���汣��             ����BUG
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
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/tcp.h>
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

#define MAX_SEND_BUFFER_SIZE (1024 * 100)
#define MAX_RECV_BUFFER_SIZE (1024 * 100)

enum eConnectStatus
{
	eConnectStatus_Disconnect, //�Ͽ�����
	eConnectStatus_Connecting, //������
	eConnectStatus_ConnectOk, //����OK
	eConnectStatus_RECONNECT, //�Ͽ�����
	eConnectStatus_REMOVE, //ɾ��
};

enum eWebSocketFrameType {
	ERROR_FRAME = 0xff,
	CONTINUATION_FRAME = 0x00,
	TEXT_FRAME = 0x01,
	BINARY_FRAME = 0x02,
	CLOSE_FRAME = 0x08,
	PING_FRAME = 0x09,
	PONG_FRAME = 0x0A
};

struct stMsgFlag
{
	uint32_t nInitRcvBufSZ; //������Ϣ�Ļ�������ʼֵ
	//uint32		nMaxRcvBufSZ;			//������Ϣ�Ļ��������ֵ
	uint32_t nMaxRcvMsgSZ; //������Ϣ�������ֵ
	uint32_t nInitSendBufSZ; //������Ϣ�Ļ�������ʼֵ
	uint32_t nMaxCacheSize; //��󻺴��С

	stMsgFlag()
	{
		nInitRcvBufSZ = 1024 * 4;
		nInitSendBufSZ = 1024 * 100;
		nMaxRcvMsgSZ = 1024 * 100;
		nMaxCacheSize = 0;
	}
};

struct NFServerFlag
{
	uint32_t nMaxConnectNum; //���������
	uint32_t nInitConnectNum; //��ʼ���Ӹ���
	uint32_t nWorkThreadNum; //�����̸߳���
	uint16_t nPort; //�˿�
	bool bEncrypt; //�Ƿ����
	bool bWebSocket; //�Ƿ���websocket
	uint32_t nMaxMsgNumMinPer; //ÿһ����������Ϣ����
	uint32_t nTimeoutDisconnect; //��ʱ�Ͽ����ӶϿ�����
	uint32_t mPacketParseType; //������Ϣ����
	NFServerFlag()
	{
		nInitConnectNum = 10;
		nWorkThreadNum = 1;
		nMaxConnectNum = 10000;
		bEncrypt = false;
		nPort = 0;
		nMaxMsgNumMinPer = 800;
		nTimeoutDisconnect = 0;
		mPacketParseType = 0;
		bWebSocket = false;
	}
};

struct NFClientFlag
{
	bool bAutoConnect; //�Զ�����
	bool bEncrypt;
	bool bWebSocket;
	uint16_t nPort;
	std::string strIP;
	uint16_t sSuspend;
	std::string origin;

	NFClientFlag()
	{
		nPort = 0;
		bAutoConnect = false;
		bEncrypt = false;
		bWebSocket = false;
		sSuspend = 0;
	}
};

class NFThreadNetMsg
{
public:
	NFThreadNetMsg()
	{
		usLink = 0;
		eType = eMsgType_Num;
		nMsgId = 0;
		nValue = 0;
	}

	uint32_t usLink; //������ײ�ͨ�ŵĽӿ�
	uint32_t nMsgId;
	uint64_t nValue;
	eMsgType eType; //��Ϣ�Ǵ���������
	NFSimpleBuffer nBuffer;
};

