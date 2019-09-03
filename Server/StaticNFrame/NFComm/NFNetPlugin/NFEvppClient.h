// -------------------------------------------------------------------------
//    @FileName         :    NFEvppClient.h
//    @Author           :    GaoYi
//    @Date             :    2019-06-04
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                               _(\_/) 
//                             ,((((^`\
//                            ((((  (6 \ 
//                          ,((((( ,    \
//      ,,,_              ,(((((  /"._  ,`,
//     ((((\\ ,...       ,((((   /    `-.-'
//     )))  ;'    `"'"'""((((   (      
//    (((  /            (((      \
//     )) |                      |
//    ((  |        .       '     |
//    ))  \     _ '      `t   ,.')
//    (   |   y;- -,-""'"-.\   \/  
//    )   / ./  ) /         `\  \
//       |./   ( (           / /'
//       ||     \\          //'|
//       ||      \\       _//'||
//       ||       ))     |_/  ||
//       \_\     |_/          ||
//       `'"                  \_\
//                            `'" 
// -------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include "NFNetDefine.h"
#include "NetEvppObject.h"

#include "evpp/event_loop_thread.h"
#include "evpp/tcp_client.h"
#include "NFEvppServer.h"
#include "NFIClient.h"

/////////////////////////////////////////////////
/**
 *@file   NFEvppClient.h
 *@brief  多线程evpp/libevent网络客户端封装类.
 *
 */
 /////////////////////////////////////////////////

 /**
 *@brief   多线程evpp/libevent网络客户端封装类.
 */
class NFEvppClient : public NFIClient
{
public:
	/**
	 *@brief  构造函数.
	 */
	NFEvppClient(evpp::EventLoopThread* eventLoop, uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFEvppClient();

	/**
	 * @brief	初始化
	 *
	 * @return 是否成功
	 */
	virtual bool Init() override;

	/**
	 * @brief	关闭客户端释放数据
	 *
	 * @return  是否成功
	 */
	virtual bool Shut() override;

	/**
	 * @brief 释放数据
	 *
	 * @return bool
	 */
	virtual bool Finalize() override;

	/**
	 * @brief	服务器每帧执行
	 *
	 * @return	是否成功
	 */
	virtual bool Execute() override;

	/**
	 * @brief	删除关闭的连接
	 *
	 * @return	是否成功
	 */
	virtual void ExecuteClose();

	/**
	 * @brief	关闭连接
	 *
	 * @return	是否成功
	 */
	virtual void CloseServer();

	/**
	 * @brief	连接服务端
	 *
	 * @return  连接成功与否
	 */
	virtual bool Connect();

	/**
	 * @brief	关闭客户端连接, 与Conenct对应
	 * 程序运行时，只能在OnExectue里调用，
	 * 退出时，在析构函数调用
	 * @return
	 */
	virtual void Close();

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	void ProcessMsgLogicThread();

	void Quit();
private:
	evpp::TCPClient* m_tcpClient;
protected:
	/**
	 * @brief	连接对象
	 */
	NetEvppObject* m_pObject;

	/**
	* @brief 需要消息队列
	*/
	NFQueueVector<MsgFromNetInfo*> mMsgQueue;
};


