// -------------------------------------------------------------------------
//    @FileName         :    NFEventObj.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include <string>
#include <stdint.h>
#include "NFComm/NFCore/NFPlatform.h"
#include "google/protobuf/message.h"

/* �¼�ʹ��ע������
 ȡ�������¼���ʱ����Ĳ���һ��Ҫ�Ͷ����¼�ʱ����Ĳ���һ�£�
 �¼��ڲ����Զ����¼�����Ĳ����������ص�ָ�룩���Ϊ�¼�key�ģ�
 ���ȡ���Ͷ���ʱ������һ�£� ȡ�����ľͻ�ʧ�ܣ��ͻᵼ�����¼�����������Ұָ�룩���´θ��¼�������ʱ�򴥷��ص��ͻ��쳣

 �¼�Ƕ�ײ�������̫�࣬������ԣ�������Ҫʹ���¼�Ƕ�ף���Ҫ��Ϊ�˱��������ѭ����Ŀǰ�¼����Ƕ�ײ���֧��5��
*/

/**
 *@brief �¼�ϵͳ����������ʹ���¼�ϵͳ�Ķ�����̳��������
 */
class _NFExport NFEventObj
{
public:
	/**
	 *@brief ���캯��
	 */
	NFEventObj();

	/**
	 *@brief ��������
	 */
	virtual ~NFEventObj();
public:
	/**
	* @brief �յ��¼�����, ���յ����¼����д���
	*
	* @param nEventID		�¼�ID
	* @param nSrcID			�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType		�¼�Դ���ͣ�������ͣ���������֮���
	* @param pEventContext	���������¼�����
	* @return
	*/
	/*
	* ��Fire�����⣬���µ�����:
	* ����1:�����OnExecute����� ���ñ�Ķ����UnSubscribe����, ���keyһ��(Ҳ����nEventID,nSrcID,bySrcType��һ��),
	*		���ܵ��½�Ҫִ�е��¼���ɾ�������������Ԥ�����Ʋ�һ��
	* ����2:�����OnExecute����� Fire�˱���¼����ᵼ�µ������⣬�¼�ϵͳ�Ѿ�������Ԥ���� ��ͬ���¼���������5�Σ�
	*       ���е�Fire�¼�������20��
	*/
	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) = 0;
public:
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
	void FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);

	/**
	* @brief �����¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @param desc		�¼����������ڴ�ӡ����ȡ��Ϣ���鿴BUG֮���
	* @return			�����¼��Ƿ�ɹ�
	*/
	bool Subscribe(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc);

	/**
	* @brief ȡ�������¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	bool UnSubscribe(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType);

	/**
	* @brief ȡ��NFEventObj���ж����¼�
	*
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	bool UnSubscribeAll();
};

