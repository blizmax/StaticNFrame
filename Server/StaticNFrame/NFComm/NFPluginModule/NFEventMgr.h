// -------------------------------------------------------------------------
//    @FileName         :    NFEventMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once
#include "NFEventTemplate.h"
#include "NFEventObj.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFIEventModule.h"

/* �¼�ʹ��ע������
 ȡ�������¼���ʱ����Ĳ���һ��Ҫ�Ͷ����¼�ʱ����Ĳ���һ�£�
 �¼��ڲ����Զ����¼�����Ĳ����������ص�ָ�룩���Ϊ�¼�key�ģ�
 ���ȡ���Ͷ���ʱ������һ�£� ȡ�����ľͻ�ʧ�ܣ��ͻᵼ�����¼�����������Ұָ�룩���´θ��¼�������ʱ�򴥷��ص��ͻ��쳣

 �¼�Ƕ�ײ�������̫�࣬������ԣ�������Ҫʹ���¼�Ƕ�ף���Ҫ��Ϊ�˱��������ѭ����Ŀǰ�¼����Ƕ�ײ���֧��5��
*/

class NFIEventModule;

/**
 *@brief �¼�ϵͳ������
 */
class _NFExport NFEventMgr : public NFSingleton<NFEventMgr>
{
public:
	/**
	 *@brief ���캯��
	 */
	NFEventMgr();

	/**
	 *@brief ��������
	 */
	virtual ~NFEventMgr();

	/**
	 *@brief ��ʼ��
	 */
	bool Init(NFIEventModule* pEventModule);

	/**
	 *@brief �ͷ���Դ
	 */
	void UnInit();

	/**
	* @brief �����¼�,��ִ���յ��¼��Ķ���Ķ�Ӧ����
	*
	* @param nEventID		�¼�ID
	* @param nSrcID			�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType		�¼�Դ���ͣ�������ͣ���������֮���
	* @param pEventContext	�¼����������
	* @return				ִ���Ƿ�ɹ�
	*/
	/*
	* ������в�����ܵ�������, �����ᵼ�±���, ��������Ԥ��Ĳ�һ��:
	* ����1:��������Fire�¼����ͬ��key��ɾ����ͬ��pSink,
	*		���ܵ��½�Ҫִ�е��¼���ɾ�������������Ԥ�����Ʋ�һ��
	* ����2:��������Fire�¼����ͬ��key��ɾ����ͬ��pSink, �����¼�ϵͳ����SubscribeInfo��Add,Sub���ü�������Ԥ����
	*       ��������������ɾ�������ᵼ��std::list������ʧЧ�� ����ɾ�����ᵼ������
	* ����3:��������Fire�¼�� Fire�˱���¼����ᵼ�µ������⣬�¼�ϵͳ�Ѿ�������Ԥ���� ��ͬ���¼���������5�Σ�
	*       ���е�Fire�¼�������20��
	*/
	void FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) const;

	/**
	* @brief �����¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @param desc		�¼����������ڴ�ӡ����ȡ��Ϣ���鿴BUG֮���
	* @return			�����¼��Ƿ�ɹ�
	*/
	bool Subscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc) const;

	/**
	* @brief ȡ�������¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	bool UnSubscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType) const;

	/**
	* @brief ȡ��NFEventObj���ж����¼�
	*
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	bool UnSubscribeAll(NFEventObj* pSink) const;
private:
	NFIEventModule* m_pEventModule;
};

