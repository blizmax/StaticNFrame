// -------------------------------------------------------------------------
//    @FileName         :    NetObject.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include <stdint.h>

#include "NFLibEvent.h"

#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFNetDefine.h"
#include "NFComm/NFCore/NFBuffer.h"

#include <vector>
#include <atomic>

struct stMsg;
struct bufferevent;
class NFThreadClient;
class NFClient;
class NFServer;

/**
* @brief ������󣬴���һ������
*/
class NetObject
{
public:
	/**
	* @brief libevent�����ݻص�
	*
	* @param pEv   libevent��д������
	* @param pArg  ����Ĳ���
	* @return
	*/
	static void conn_recvcb(struct bufferevent* pEv, void* pArg);

	/**
	* @brief libevent�����¼��ص�
	*
	* @param pEv		libevent��д������
	* @param events	�¼�
	* @param pArg		����Ĳ���
	* @return
	*/
	static void conn_eventcb(struct bufferevent* pEv, short events, void* pArg);

	/**
	* @brief libeventд���ݻص�
	*
	* @param pEv   libevent��д������
	* @param pArg  ����Ĳ���
	* @return
	*/
	static void conn_writecb(struct bufferevent* pEv, void* pArg);
public:
	/**
	 * @brief	���캯��
	 */
	NetObject();

	/**
	 * @brief	��������
	 */
	virtual ~NetObject();

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
	 *@brief  ���ý��ջص�.
	 */
	void SetRecvCB(const NET_RECEIVE_FUNCTOR& recvcb)
	{
		mRecvCB = recvcb;
	}

	/**
	 *@brief  ���������¼��ص�.
	 */
	void SetEventCB(const NET_EVENT_FUNCTOR& eventcb)
	{
		mEventCB = eventcb;
	}

	/**
	* @brief �����websocket�Ļ����ж��Ƿ�handleshark
	*
	* @return bool
	*/
	bool IsHandleShark() const;

	/**
	* @brief �����websocket�Ļ�����Ҫ���handleshark
	*
	* @return
	*/
	void SetHandleShark(bool b);

	/**
	* @brief �����websocket�Ļ�
	*
	* @return bool
	*/
	bool IsWebSocket() const;

	/**
	* @brief �����websocket�Ļ�
	*
	* @return 
	*/
	void SetWebSocket(bool b);

	/**
	 * @brief
	 *
	 * @return std::string
	 */
	std::string GetStrIp() const;

	/**
	 * @brief
	 *
	 * @param  val
	 * @return void
	 */
	void SetStrIp(std::string val);

	/**
	 * @brief
	 *
	 * @return uint32_t
	 */
	uint32_t GetPort() const;

	/**
	 * @brief
	 *
	 * @param  val
	 * @return void
	 */
	void SetPort(uint32_t val);

	/**
	 * @brief	��������
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual bool OnRecvData(bufferevent* pBufEv);

	/**
	 * @brief	ȡ�����ݣ������д���
	 *
	 * @return	< 0, ��ʾ��������������, = 0, ��ʾ�������ݼ�������> 0��ʾ���ݴ������
	 */
	virtual int Dismantle();

	/**
	 * @brief	����socket������socket�����Դ���, ������
	 *
	 * @return
	 */
	virtual void SetSocketId(SOCKET nSocket);

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	/**
	 * @brief	��ö�д���ݽṹ��
	 *
	 * @return
	 */
	struct bufferevent* GetBev() const;

	/**
	 * @brief	��ö�д���ݽṹ��
	 *
	 * @return
	 */
	void SetBev(struct bufferevent* bev);

	/**
	 * @brief	���ΨһID
	 *
	 * @return
	 */
	uint32_t GetLinkId() const;

	/**
	* @brief	���ΨһID
	*
	* @return
	*/
	void SetLinkId(uint32_t linkId);

	/**
	 * @brief	�Խ������������ݽ��д���
	 *
	 * @param type    �������ͣ���Ҫ��Ϊ�˺Ͷ��߳�ͳһ����, ��Ҫ�н������ݴ������ӳɹ������Ͽ����Ӵ���
	 * @param usLink  ���ͻ��˵�Ψһid
	 * @param pBuf    ����ָ��
	 * @param sz      ���ݴ�С
	 * @param nMsgId  ������������Ϣid
	 * @param nValue  ��ϢͷЯ����ֵ�����������ID��Ҳ�����ǶԷ��ͻ������ӵ�Ψһid
	 * @return
	 */
	virtual void OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue);

	/**
	* @brief	�Խ������������ݽ��д���
	*
	* @param type    �������ͣ���Ҫ��Ϊ�˺Ͷ��߳�ͳһ����, ��Ҫ�н������ݴ������ӳɹ������Ͽ����Ӵ���
	* @param usLink  ���ͻ��˵�Ψһid
	* @param strMsg  websocket �ַ������� ������ַ�������0��β����Ҫʹ��c_str����
	* @return
	*/
	virtual void OnHandleMsgPeer(eMsgType type, uint32_t usLink, const std::string& strMsg);

	/**
	 * @brief
	 *
	 * @return bool
	 */
	bool GetNeedRemove() const;

	/**
	 * @brief
	 *
	 * @param  val
	 * @return void
	 */
	void SetNeedRemove(bool val);

	/**
	 * @brief	�������ӳɹ�
	 *
	 * @param nSocket	ϵͳ�����socket
	 * @return
	 */
	virtual void OnHandleConnect(SOCKET nSocket);

	/**
	 * @brief	����Ͽ�����
	 *
	 * @return
	 */
	virtual void OnHandleDisConnect();

	/**
	 * @brief �رն����ֹ����Ķ�д����
	 *
	 * @return void
	 */
	virtual void CloseObject();

	/**
	* @brief ����shark info
	*
	* @return void
	*/
	virtual int HandleSharkInfo();

	/**
	* @brief ����shark info
	*
	* @return void
	*/
	virtual std::string HandleSharkReturn();

	/**
	* @brief �Ƿ��Ƿ�������
	*
	* @return void
	*/
	virtual void SetIsServer(bool b);

	virtual void SendHandleShark();

	virtual void SetOrigin(const std::string& str) { m_origin = str; }

	virtual void SetPacketParseType(uint32_t packetType) { mPacketParseType = packetType; }
protected:
	/**
	 * @brief	����������ݵĻص�
	 */
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	 * @brief	�����¼��ص�
	 */
	NET_EVENT_FUNCTOR mEventCB;

	/**
	 * @brief	libevent�������Ӷ�д�¼�����
	 */
	struct bufferevent* m_pBev;

	/**
	 * @brief	����������ӵ�socket
	 */
	SOCKET m_nSocketId;

	/**
	 * @brief	����ͻ������ӵ�ΨһID
	 */
	uint32_t m_usLinkId;

	/**
	 * @brief	�����������緢��������ʱ����
	 */
	NFBuffer m_buffer;

	/**
	* @brief	websocket��Ҫ
	*/
	std::string m_origin;

	/**
	 * @brief	���Ӵ���ĶԷ���IP
	 */
	std::string m_strIp;

	/**
	 * @brief	���Ӵ���ĶԷ��Ķ˿�
	 */
	uint32_t m_port;

	/**
	 * @brief �Ƿ���Ҫɾ��, ������Ӳ��������ã�������һ��ѭ���б�ɾ��
	 */
	bool mNeedRemove;

	/**
	* @brief �����websocket����Ҫ������
	*/
	bool mHandleShark;

	/**
	* @brief websocket
	*/
	bool mWebSocket;

	/**
	* @brief is server
	*/
	bool mIsServer; //�Ƿ��Ƿ�������

	/**
	* @brief ������Ϣ����
	*/
	uint32_t mPacketParseType;

	/**
	* @brief websocket header map
	*/
	std::map<std::string, std::string> mHeaderMap;

	std::string mCacheFrame;

	std::string mParseString;

	uint32_t mWSFrameType;
};

