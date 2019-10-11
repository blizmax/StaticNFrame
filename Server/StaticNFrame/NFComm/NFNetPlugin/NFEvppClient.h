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
 *@brief  ���߳�evpp/libevent����ͻ��˷�װ��.
 *
 */
 /////////////////////////////////////////////////

 /**
 *@brief   ���߳�evpp/libevent����ͻ��˷�װ��.
 */
class NFEvppClient : public NFIClient
{
public:
	/**
	 *@brief  ���캯��.
	 */
	NFEvppClient(evpp::EventLoop* eventLoop, uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  ��������.
	 */
	virtual ~NFEvppClient();

	/**
	 * @brief	��ʼ��
	 *
	 * @return �Ƿ�ɹ�
	 */
	virtual bool Init() override;

	/**
	 * @brief	�رտͻ����ͷ�����
	 *
	 * @return  �Ƿ�ɹ�
	 */
	virtual bool Shut() override;

	/**
	 * @brief �ͷ�����
	 *
	 * @return bool
	 */
	virtual bool Finalize() override;

	/**
	 * @brief	������ÿִ֡��
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual bool Execute() override;

	/**
	 * @brief	�ر�����
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual void CloseServer();

	/**
	 * @brief	���ӷ����
	 *
	 * @return  ���ӳɹ����
	 */
	virtual bool Connect();

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	/**
	 * @brief	�������� ����������ͷ
	 *
	 * @param pData		���͵�����,
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	void ProcessMsgLogicThread();

	void SafeExit();
private:
	evpp::TCPClient* m_tcpClient;
protected:
	/**
	 * @brief	���Ӷ���
	 */
	NetEvppObject* m_pObject;

	/**
	* @brief ��Ҫ��Ϣ����
	*/
	NFQueueVector<MsgFromNetInfo*> mMsgQueue;

	/**
	 * @brief	���̰߳�ȫ�˳�
	 */
	std::atomic_bool m_safeExit;
};


