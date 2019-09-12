// -------------------------------------------------------------------------
//    @FileName         :    NFIClient.h
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
#include <string>
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NetEvppObject.h"

#include "NFEvppServer.h"

/////////////////////////////////////////////////
/**
 *@file   NFIClient.h
 *@brief  ����ͻ��˷�װ��.
 *
 */
 /////////////////////////////////////////////////

 /**
 *@brief   ����ͻ��˷�װ��.
 */
class NFIClient : public NFIModule
{
public:
	/**
	 *@brief  ���캯��.
	 */
	NFIClient(uint32_t nId, const NFClientFlag& flag):m_usLinkId(nId), m_flag(flag)
	{
		mStatus = eConnectStatus_Disconnect;
		mLastActionTime = 0;
		mPacketParseType = 0;
	}

	/**
	 *@brief  ��������.
	 */
	virtual ~NFIClient()
	{
		
	}

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
	 * @brief	ɾ���رյ�����
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual void ExecuteClose() = 0;

	/**
	 * @brief	�ر�����
	 *
	 * @return	�Ƿ�ɹ�
	 */
	virtual void CloseServer() = 0;

	/**
	 * @brief	��÷���������
	 *
	 * @return
	 */
	const string& GetName() const { return m_strName; }

	/**
	 * @brief	���ؿͻ�����������
	 *
	 * @return
	 */
	const NFClientFlag& GetFlag() const { return m_flag; }

	/**
	 * @brief	���ΨһID
	 *
	 * @return
	 */
	uint32_t GetLinkId() const { return m_usLinkId; }

	/**
	* @brief	���ΨһID
	*
	* @return
	*/
	void SetLinkId(uint32_t linkId) { m_usLinkId = linkId; }

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize) = 0;

	/**
	 * @brief	�������� ����������ͷ
	 *
	 * @param pData		���͵�����,
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	/**
	 * @brief
	 */
	eConnectStatus GetStatus() const { return mStatus; }

	/**
	 * @brief
	 */
	void SetStatus(eConnectStatus val) { mStatus = val; }

	/**
	 * @brief
	 */
	bool IsNeedRemve() const { return mStatus == eConnectStatus_REMOVE; }

	/**
	 * @brief
	 *
	 * @return uint64_t
	 */
	uint64_t GetLastActionTime() const { return mLastActionTime; }

	/**
	 * @brief
	 *
	 * @param  time
	 * @return void
	 */
	void SetLastActionTime(uint64_t time) { mLastActionTime = time; }

	virtual bool IsWebSocket() const { return m_flag.bWebSocket; }

	void SetPacketParseType(uint32_t type) { mPacketParseType = type; }
	uint32_t GetPacketParseType() const { return mPacketParseType; }
protected:
	/**
	 * @brief	������������
	 */
	NFClientFlag m_flag;

	/**
	 * @brief	����������
	 */
	std::string m_strName;

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
	 * @brief ����״̬
	 */
	eConnectStatus mStatus;

	/**
	 * @brief ��һ�λʱ��
	 */
	uint64_t mLastActionTime;

	/**
	* @brief ������Ϣ����
	*/
	uint32_t mPacketParseType;
};
