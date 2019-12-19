// -------------------------------------------------------------------------
//    @FileName         :    NFIServer.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/03
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
#include "NFComm/NFCore/NFMutex.h"
#include "NFComm/NFCore/NFLock.h"


class NFIServer : public NFIModule
{
	friend class NFCNetServerModule;
public:
	/**
 * @brief ���캯��
 */
	NFIServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag) : mFlag(flag), mServerType(serverType), mServerId(serverId), mNetObjectCount(0)
	{
		mWebSocket = flag.bWebSocket;
		mPacketParseType = flag.mPacketParseType;
		mNetObjectMaxIndex = 0;
		assert(serverType > NF_ST_NONE && serverType < NF_ST_MAX);
	}

	/**
	* @brief ��������
	*/
	virtual ~NFIServer()
	{
		
	}
	/**
	 *@brief  ���ý��ջص�.
	 */
	template <typename BaseType>
	void SetRecvCB(BaseType* pBaseType, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t opreateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
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
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize) = 0;

	/**
 * @brief	�������� ����������ͷ
 *
 * @param pData		���͵�����,
 * @param unSize	���ݵĴ�С
 * @return
 */
	virtual bool Send(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	/**
	 * @brief �������IP
	 *
	 * @param  usLinkId
	 * @return std::string
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) = 0;

	/**
	* @brief �ر�����
	*
	* @param  usLinkId
	* @return
	*/
	virtual void CloseLinkId(uint32_t usLinkId) = 0;

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool SendAll(const void* pData, uint32_t unSize) = 0;

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool SendAll(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	/**
	* @brief	���ΨһID
	*
	* @return
	*/
	virtual uint32_t GetServerId() const { return mServerId; }

	/**
	* @brief	��÷���������
	*
	* @return
	*/
	virtual uint32_t GetServerType() const { return mServerType; }

	/**
	* @brief	�Ƿ���web������
	*
	* @return
	*/
	virtual bool IsWebSocket() const { return mWebSocket; }

	/**
	* @brief ��õ�ǰ����������
	*
	* @return uint32_t
	*/
	virtual uint32_t GetNetObjectCount() const { return mNetObjectCount; }

	/**
	* @brief ��õ�ǰ������Ӹ���
	*
	* @return uint32_t
	*/
	virtual uint32_t GetMaxConnectNum() const { return mFlag.nMaxConnectNum; }
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
	* @brief ���Ӷ�����ʵ��Ŀ
	*/
	uint32_t mNetObjectCount;

	/**
	* @brief ��ǰ���Ӷ����������
	*/
	uint32_t mNetObjectMaxIndex;

	/**
	* @brief �Ƿ���websocket
	*/
	bool mWebSocket;

	/**
	* @brief ������Ϣ����
	*/
	uint32_t mPacketParseType;

	/**
	* @brief ������Ϣ����, ����������IP�ռ�
	*/
	std::map<std::string, uint32_t> mAttackIp;

	/**
	* @brief ������Ϣ����, ����������IP�ռ�
	*/
	NFMutex mAttackIpLock;
};


