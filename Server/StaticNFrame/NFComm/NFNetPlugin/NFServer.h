// -------------------------------------------------------------------------
//    @FileName         :    NFServer.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"

struct evconnlistener;
struct event_base;
class NetObject;
class NFCNetServerModule;

class NFServer : public NFIModule
{
	friend NFCNetServerModule;
public:
	/**
	 * @brief ���캯��
	 */
	NFServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag);

	/**
	* @brief ��������
	*/
	virtual ~NFServer();

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
	 * @brief ����������
	 *
	 * @param  fd
	 * @param  sa
	 * @return bool
	 */
	bool AddNetObject(SOCKET fd, sockaddr* sa);

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize);

	/**
	 * @brief �������IP
	 *
	 * @param  usLinkId
	 * @return std::string 
	 */
	std::string GetLinkIp(uint32_t usLinkId);

	/**
	* @brief �ر�����
	*
	* @param  usLinkId
	* @return 
	*/
	void CloseLinkId(uint32_t usLinkId);

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	bool SendAll(const void* pData, uint32_t unSize);

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

	/**
	 * @brief ���һ�����õ�ID
	 *
	 * @return uint32_t
	 */
	virtual uint32_t GetFreeUnLinkId();

	/**
	* @brief ��õ�ǰ����������
	*
	* @return uint32_t
	*/
	virtual uint32_t GetNetObjectCount() const;

	/**
	* @brief ��õ�ǰ������Ӹ���
	*
	* @return uint32_t
	*/
	virtual uint32_t GetMaxConnectNum() const;

	/**
	 * @brief
	 *
	 * @return event_base*
	 */
	virtual event_base* GetEventBase() const;
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
	* @brief libevent
	*/
	struct event_base* mBase;

	/**
	* @brief libevent������
	*/
	struct evconnlistener* mListener;

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
	* @brief ���Ӷ�������
	*/
	std::vector<NetObject*> mNetObjectArray;

	/**
	* @brief ���Ӷ�����ʵ��Ŀ
	*/
	uint32_t mNetObjectCount;

	/**
	* @brief ��Ҫɾ�������Ӷ���
	*/
	std::vector<uint32_t> mvRemoveObject;

	/**
	* @brief �Ƿ���websocket
	*/
	bool mWebSocket;

	/**
	* @brief ������Ϣ����
	*/
	uint32_t mPacketParseType;
};

