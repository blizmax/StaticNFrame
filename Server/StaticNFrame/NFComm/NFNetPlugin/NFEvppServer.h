// -------------------------------------------------------------------------
//    @FileName         :    NFEvppServer.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//
//                    .::::.
//                  .::::::::.
//                 :::::::::::  FUCK YOU
//             ..:::::::::::'
//           '::::::::::::'
//             .::::::::::
//        '::::::::::::::..
//             ..::::::::::::.
//           ``::::::::::::::::
//            ::::``:::::::::'        .:::.
//           ::::'   ':::::'       .::::::::.
//         .::::'      ::::     .:::::::'::::.
//        .:::'       :::::  .:::::::::' ':::::.
//       .::'        :::::.:::::::::'      ':::::.
//      .::'         ::::::::::::::'         ``::::.
//  ...:::           ::::::::::::'              ``::.
// ```` ':.          ':::::::::'                  ::::..
//                    '.:::::'                    ':'````..
//
// -------------------------------------------------------------------------
#pragma once


#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"
#include "NetEvppObject.h"
#include "NFIServer.h"

#include <evpp/tcp_server.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>
#include "NFComm/NFCore/NFQueue.hpp"

struct MsgFromNetInfo
{
	MsgFromNetInfo(const evpp::TCPConnPtr TCPConPtr) : mTCPConPtr(TCPConPtr)
	{
		nType = eMsgType_Num;
	}

	eMsgType nType;
	evpp::TCPConnPtr mTCPConPtr;
	std::string strMsg;
};

class NFCNetServerModule;

class NFEvppServer : public NFIServer
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
	 * @brief ����������
	 *
	 * @param  fd
	 * @param  sa
	 * @return bool
	 */
	NetEvppObject* AddNetObject(const evpp::TCPConnPtr& conn);

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

	/**
	 * @brief �������IP
	 *
	 * @param  usLinkId
	 * @return std::string
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) override;

	/**
	* @brief �ر�����
	*
	* @param  usLinkId
	* @return
	*/
	virtual void CloseLinkId(uint32_t usLinkId) override;

	/**
	 * @brief ���һ�����õ�ID
	 *
	 * @return uint32_t
	 */
	virtual uint32_t GetFreeUnLinkId();

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool SendAll(const void* pData, uint32_t unSize) override;

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize) override;

	NetEvppObject* GetNetObject(uint32_t uslinkId);
protected:
	/**
	 * @brief
	 *
	 * @return void
	 */
	virtual void ExecuteClose();

	/**
	 * @brief ����������ݰ��ص�
	 *
	 * @param  unLinkId
	 * @param  playerId
	 * @param  nMsgId
	 * @param  msg
	 * @param  nLen
	 * @return void
	 */
	virtual void OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	 * @brief �����¼������ӺͶϿ����Ӵ���
	 *
	 * @param  nEvent
	 * @param  unLinkId
	 * @return void
	 */
	virtual void OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId);

	/**
	 * @brief ���̴߳�����Ϣ����
	 */
	virtual void ProcessMsgLogicThread();
private:
	evpp::EventLoopThread* m_eventLoop;
	evpp::TCPServer* m_tcpServer;
private:
	/**
	* @brief ���Ӷ�������
	*/
	std::unordered_map<uint32_t, NetEvppObject*> mNetObjectArray;

	/**
	* @brief ��Ҫɾ�������Ӷ���
	*/
	std::vector<uint32_t> mvRemoveObject;

	/**
	* @brief ��Ҫ��Ϣ����
	*/
	NFQueueVector<MsgFromNetInfo*> mMsgQueue;

	/**
	* @brief �˳���ʱ������ȷ��
	*/
	atomic_bool mExit;
};
