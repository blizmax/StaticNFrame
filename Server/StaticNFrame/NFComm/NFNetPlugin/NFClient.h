// -------------------------------------------------------------------------
//    @FileName         :    NFClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-24
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NetObject.h"
#include "NFIClient.h"

/////////////////////////////////////////////////
/**
 *@file   NFClient.h
 *@brief  ���߳�libevent����ͻ��˷�װ��.
 *
 */
/////////////////////////////////////////////////

/**
*@brief  ���߳�libevent����ͻ��˷�װ��.
*/
class NFClient : public NFIClient
{
public:
	/**
	 *@brief  ���캯��.
	 */
	NFClient(uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  ��������.
	 */
	virtual ~NFClient();

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
	 * @brief	���libevent�����ݽṹ
	 * @return
	 */
	event_base* GetMainBase() const;

	/**
	 * @brief	��������
	 *
	 * @param pData		���͵�����, ����������Ѿ�����������ͷ
	 * @param unSize	���ݵĴ�С
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);
protected:
	/**
	 * @brief	libevent��react���ݽṹ
	 */
	event_base* m_pMainBase;;

	/**
	 * @brief	���Ӷ���
	 */
	NetObject* m_pObject;
};

