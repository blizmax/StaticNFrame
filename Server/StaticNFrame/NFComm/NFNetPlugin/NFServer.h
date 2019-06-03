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
#include "NFIServer.h"

struct evconnlistener;
struct event_base;
class NetObject;
class NFCNetServerModule;

class NFServer : public NFIServer
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
	virtual bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize) override;

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
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool SendAll(const void* pData, uint32_t unSize) override;

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
	* @brief libevent
	*/
	struct event_base* mBase;

	/**
	* @brief libevent������
	*/
	struct evconnlistener* mListener;

	/**
	* @brief ���Ӷ�������
	*/
	std::vector<NetObject*> mNetObjectArray;

	/**
	* @brief ��Ҫɾ�������Ӷ���
	*/
	std::vector<uint32_t> mvRemoveObject;
};

