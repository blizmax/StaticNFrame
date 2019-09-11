// -------------------------------------------------------------------------
//    @FileName         :    NetEvppObject.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/3
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


#include <cstdint>

#include "NFLibEvent.h"

#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFNetDefine.h"
#include "NFComm/NFCore/NFBuffer.h"
#include "evpp/tcp_conn.h"


class NFEvppServer;
class NFEvppClient;

/**
* @brief ������󣬴���һ������
*/
class NetEvppObject
{
public:
	friend NFEvppServer;
	friend NFEvppClient;
	/**
	 * @brief	���캯��
	 */
	NetEvppObject(const evpp::TCPConnPtr& conn);

	/**
	 * @brief	��������
	 */
	virtual ~NetEvppObject();

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
	 * @brief	ȡ�����ݣ������д���
	 *
	 * @return	< 0, ��ʾ��������������, = 0, ��ʾ�������ݼ�������> 0��ʾ���ݴ������
	 */
	virtual int Dismantle();

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

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
	 * @brief	��������
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual bool OnRecvData(const char* data, size_t length);

	/**
	 * @brief	�������ӳɹ�
	 *
	 * @param nSocket	ϵͳ�����socket
	 * @return
	 */
	virtual void OnHandleConnect();

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
	* @brief �Ƿ��Ƿ�������
	*
	* @return void
	*/
	virtual void SetIsServer(bool b);

	/**
	* @brief ���ý�������
	*
	* @return void
	*/
	virtual void SetPacketParseType(uint32_t packetType) { mPacketParseType = packetType; }

	virtual void SetConnPtr(const evpp::TCPConnPtr conn) { mConnPtr = conn; }
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
	 * @brief	����ͻ������ӵ�ΨһID
	 */
	uint32_t m_usLinkId;

	/**
	 * @brief	�����������緢��������ʱ����
	 */
	NFBuffer m_buffer;

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
	* @brief is server
	*/
	bool mIsServer; //�Ƿ��Ƿ�������

	/**
	* @brief ������Ϣ����
	*/
	uint32_t mPacketParseType;

	/**
	* @brief ����EVPP�����Ӵ���
	*/
	evpp::TCPConnPtr mConnPtr;
};

