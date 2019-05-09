// -------------------------------------------------------------------------
//    @FileName         :    NFCEventModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_EVENT_MODULE_H
#define NFC_EVENT_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIEventModule.h"

#include "NFComm/NFPluginModule/NFEventTemplate.h"
#include "NFComm/NFPluginModule/NFEventObj.h"

/* �¼�ʹ��ע������
 ȡ�������¼���ʱ����Ĳ���һ��Ҫ�Ͷ����¼�ʱ����Ĳ���һ�£�
 �¼��ڲ����Զ����¼�����Ĳ����������ص�ָ�룩���Ϊ�¼�key�ģ�
 ���ȡ���Ͷ���ʱ������һ�£� ȡ�����ľͻ�ʧ�ܣ��ͻᵼ�����¼�����������Ұָ�룩���´θ��¼�������ʱ�򴥷��ص��ͻ��쳣

 �¼�Ƕ�ײ�������̫�࣬������ԣ�������Ҫʹ���¼�Ƕ�ף���Ҫ��Ϊ�˱��������ѭ����Ŀǰ�¼����Ƕ�ײ���֧��5��
*/

//�¼�ִ�ж���
struct OnEventExecuteObj
{
	bool operator()(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) const
	{
		if (nullptr == pSink) return false;

		pSink->OnExecute(nEventID, nSrcID, bySrcType, message);
		return true;
	}
};

class NFIKernelModule;

class NFCEventModule
	: public NFIEventModule
{
public:
	explicit NFCEventModule(NFIPluginManager* p);

	virtual ~NFCEventModule();

	virtual bool Init() override;
	virtual bool AfterInit() override;
	virtual bool BeforeShut() override;
	virtual bool Shut() override;
	virtual bool Execute() override;
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
	virtual void FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) override;

	/**
	* @brief �����¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @param desc		�¼����������ڴ�ӡ����ȡ��Ϣ���鿴BUG֮���
	* @return			�����¼��Ƿ�ɹ�
	*/
	virtual bool Subscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc) override;

	/**
	* @brief ȡ�������¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	virtual bool UnSubscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType) override;

	/**
	* @brief ȡ��NFEventObj���ж����¼�
	*
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	virtual bool UnSubscribeAll(NFEventObj* pSink) override;
private:
	NFEventTemplate<NFEventObj, OnEventExecuteObj> m_ExecuteCenter;
};

#endif

