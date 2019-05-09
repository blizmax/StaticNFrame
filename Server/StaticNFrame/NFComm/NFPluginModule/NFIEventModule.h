// -------------------------------------------------------------------------
//    @FileName         :    NFIEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEventModule
//
// -------------------------------------------------------------------------

#ifndef NFI_EVENT_MODULE_H
#define NFI_EVENT_MODULE_H

#include <iostream>
#include "NFIModule.h"

#include "NFComm/NFPluginModule/NFEventObj.h"

class NFIEventModule
	: public NFIModule
{
public:
	/**
	* @brief �����¼�,��ִ���յ��¼��Ķ���Ķ�Ӧ����
	*
	* @param nEventID		�¼�ID
	* @param nSrcID			�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType		�¼�Դ���ͣ�������ͣ���������֮���
	* @param message	�¼����������
	* @return				ִ���Ƿ�ɹ�
	*/
	virtual void FireExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message) = 0;

	/**
	* @brief �����¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @param desc		�¼����������ڴ�ӡ����ȡ��Ϣ���鿴BUG֮���
	* @return			�����¼��Ƿ�ɹ�
	*/
	virtual bool Subscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc) = 0;

	/**
	* @brief ȡ�������¼�
	*
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	virtual bool UnSubscribe(NFEventObj* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType) = 0;

	/**
	* @brief ȡ��NFEventObj���ж����¼�
	*
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	virtual bool UnSubscribeAll(NFEventObj* pSink) = 0;
};

#endif

