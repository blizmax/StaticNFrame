// -------------------------------------------------------------------------
//    @FileName         :    NFEvppServer.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once


#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"

#include <evpp/tcp_server.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>

class NFCNetServerModule;

class NFEvppServer : public NFIModule
{
	friend NFCNetServerModule;
public:
	/**
	 * @brief ���캯��
	 */
	NFEvppServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag);

	/**
	* @brief ��������
	*/
	virtual ~NFEvppServer();

	/**
	 *@brief  ���ý��ջص�.
	 */
	template <typename BaseType>
	void SetRecvCB(BaseType* pBaseType, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	}

	/**
	 *@brief  ���������¼��ص�.
	 */
	template <typename BaseType>
	void SetEventCB(BaseType* pBaseType, void (BaseType::*handleEvent)(const eMsgType nEvent, const uint32_t unLinkId))
	{
		mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2);
	}

	/**
	* @brief	���ΨһID
	*
	* @return
	*/
	uint32_t GetServerId() const;

	/**
	* @brief	��÷���������
	*
	* @return
	*/
	uint32_t GetServerType() const;

	/**
	* @brief	�Ƿ���web������
	*
	* @return
	*/
	bool IsWebSocket() const;

	/**
	* @brief	��ʼ��
	*
	* @return �Ƿ�ɹ�
	*/
	virtual bool Init() override;

	/**
	* @brief	�رտͻ���
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
private:
	evpp::EventLoop* m_eventLoop;
	evpp::TCPServer* m_tcpServer;
private:
	/**
	 * @brief	����������ݵĻص�
	 */
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	 * @brief	�����¼��ص�
	 */
	NET_EVENT_FUNCTOR mEventCB;

	/**
	* @brief ��������������
	*/
	NFServerFlag mFlag;

	/**
	* @brief ����������
	*/
	NF_SERVER_TYPES mServerType;

	/**
	* @brief ������Id��һ��һ��Ӧ�ó���һ�����������ͣ�һ��������ID
	*/
	uint32_t mServerId;

	/**
	* @brief �Ƿ���websocket
	*/
	bool mWebSocket;

	/**
	* @brief ������Ϣ����
	*/
	uint32_t mPacketParseType;
};
