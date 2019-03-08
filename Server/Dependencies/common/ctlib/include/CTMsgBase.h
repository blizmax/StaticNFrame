/**
* 消息基本定义
*/

#ifndef __CTLIB_MSGBASE_H__
#define __CTLIB_MSGBASE_H__

#include "CTCodeConvert.h"

namespace CTLib
{
//编解码方式
enum CODE_TYPE
{
    CODE_BIN = 1,       
    CODE_STRING = 2,
    CODE_JSON = 3,
    CODE_MYSQL = 4,
    CODE_ENCRYPTED = 5, //加密协议，用于经过ptkey加密的数据包
    CODE_PROTOBUF = 6, //protocol buff协议
    CODE_HTTP   = 7,   //HTTP协议
};

enum enumMsgType
{
    EMT_REQUEST = 1,     ///< 请求消息
    EMT_ANSWER  = 2,     ///< 响应消息
    EMT_NOTIFY  = 3,     ///< 无需响应的通知消息
    EMT_ERRANS  = 4,     ///< 简单错误返回 
};

/**
* 消息头部
*/
class CTMsgHead : public CTCodeObject
{
public:
    enum
    {
        EMAX_MSGLEN = 0x7FFF,   ///< 消息的最大长度
    };

    short shMsgID;      ///< 用来解析具体的消息
    short shMsgType;    ///< 消息类型, 参见enumMsgType
    int   iCmdSeq;      ///< 消息Seq
    int64 llMsgAct;     ///< ACT字段, 需要原封不动的回传

    CTMsgHead() :
    shMsgID(0), shMsgType(0), iCmdSeq(0),llMsgAct(0)
    {
    }

    bool IsValid() const
    {
        return shMsgID > 0 && 
            (shMsgType == EMT_REQUEST ||
            shMsgType == EMT_ANSWER ||
            shMsgType == EMT_NOTIFY ||
            shMsgType == EMT_ERRANS);
    }
};

/** enp消息包格式定义 */
typedef enum enp_package
{
    enp_package_binary = 0,		///< 二进制协议, 头2字节标识消息长度
    enp_package_string,			///< 字符串协议, 以\n结尾
    enp_package_flash,          ///< flash xmlsocket协议, 以\0结尾
    enp_package_encrypted,      ///< 字符串加密协议，包为二进制形式
    enp_package_protobuf,		///< protocol buff协议, 头2字节标识消息长度
    enp_package_http,           ///< http协议
}
EEnpPackage;

/** enp socket状态定义*/
enum enp_sock
{
    enp_sock_free = 0,		///< 空闲socket
    enp_sock_accepted,		///< 已连接的被动socket
    enp_sock_max			///< 必须是最后一个
};

/**
* Handle的状态标志定义.
* 低8位用于enp向上层返回状态
* 高8位用于上层向enp发送控制
*/
enum enp_flag
{
    enp_flag_error			= 0x1,		///< (enp ->) 其他未知错误
    enp_flag_remoteclose	= 0x2,		///< (enp ->) 远程主机关闭连接
    enp_flag_idleclose		= 0x4,		///< (enp ->) 因为发呆而被enp关闭

    enp_flag_askclose		= 0x100,	///< (-> enp) Handle在发送完成后关闭
    enp_flag_busy   		= 0x200,	///< (-> enp) 上层繁忙
    enp_flag_msg_too_big    = 0x400,    ///< 消息太大
};

/**
* ENP头部
*/
class CEnpNetHead
{
public:
    CEnpNetHead():
        uiRemoteIP(0),
        unRemotePort(0),
		uiLocalIP(0),
		unLocalPort(0),
        uiHandle(0),
        uiHandleSeq(0),
        uiCreateTime(0),
        uiLastTime(0),
        uiAct1(0),
        uiAct2(0),
        unLiveFlag(0),
        ucEnodeMethod(0),
        ucDeodeMethod(0),
        unDataLength(0)
    {
    }

public:
    unsigned int		uiRemoteIP;     ///< 远程客户端的IP
    unsigned short		unRemotePort;   ///< 远程客户端的端口
	unsigned int		uiLocalIP;		///< 服务器端IP
	unsigned short		unLocalPort;	///< 服务器端端口
    unsigned int		uiHandle;		///< 本连接对应的ENP Handle(上层程序不要修改)
    unsigned int		uiHandleSeq;	///< 序列号(上层程序不要修改)
    unsigned int		uiCreateTime;	///< socket创建时间
    unsigned int		uiLastTime;		///< socket最后活跃时间
    unsigned int		uiAct1;			///< ACT值
    unsigned int		uiAct2;			///< ACT值
    unsigned short		unLiveFlag;		///< 参见 enum enp_flag
    unsigned char       ucEnodeMethod;  ///< 消息体编码方式
    unsigned char       ucDeodeMethod;  ///< 消息体解码方式
    unsigned short		unDataLength;   ///< 后续的数据长度
    char szPtKey[20];  ///< 用于加密协议的解密

    CEnpNetHead& operator=(const CEnpNetHead& s)
    {
        uiRemoteIP = s.uiRemoteIP;
        unRemotePort = s.unRemotePort;
        uiLocalIP = s.uiLocalIP;
        unLocalPort = s.unLocalPort;
        uiHandle = s.uiHandle;
        uiHandleSeq = s.uiHandleSeq;
        uiCreateTime = s.uiCreateTime;
        uiLastTime = s.uiLastTime;
        uiAct1 = s.uiAct1;
        uiAct2 = s.uiAct2;
        unLiveFlag = s.unLiveFlag;
        ucEnodeMethod = s.ucEnodeMethod;
        ucDeodeMethod = s.ucDeodeMethod;
        unDataLength = s.unDataLength;
        memcpy(szPtKey, s.szPtKey, sizeof(szPtKey));

        return *this;
    }

};

} // namespace CTLib

BEGIN_CODE_CONVERT(CTMsgHead)
CODE_CONVERT(shMsgID)
CODE_CONVERT(shMsgType)	
CODE_CONVERT(llMsgAct)
CODE_CONVERT(iCmdSeq)
END_CODE_CONVERT(CTMsgHead)

#endif //__CTLIB_MSGBASE_H__
