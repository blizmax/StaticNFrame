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
	NFEvppClient(evpp::EventLoopThread* eventLoop, uint32_t nId, const NFClientFlag& flag);

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
	 * @brief	ɾ���رյ�����
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual void ExecuteClose();

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
	 * @brief	�رտͻ�������, ��Conenct��Ӧ
	 * ��������ʱ��ֻ����OnExectue����ã�
	 * �˳�ʱ����������������
	 * @return
	 */
	virtual void Close();

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	void ProcessMsgLogicThread();

	void Quit();
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
};


