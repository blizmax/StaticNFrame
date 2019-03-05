// -------------------------------------------------------------------------
//    @FileName         :    NFBuffer.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"

/**
* @brief �߳�����װ,�ο�google protobuf
*
*/
class _NFExport NFMutex
{
public:
	/**
	* @brief ���캯��.
	*
	*/
	NFMutex();

	/**
	* @brief ��������.
	*
	*/
	virtual ~NFMutex();

	/**
	* @brief ��ס�̣߳� ����б�Ҫֱ������������Ȼ��һֱ����
	* Block if necessary until this Mutex is free, then acquire it exclusively.
	*/
	void Lock();

	/**
	* @brief ���� Release this Mutex.  Caller must hold it exclusively.
	*
	*/
	void Unlock();

	/**
	* @brief ����̲߳�����ȫ������������ᵼ�±���
	* Crash if this Mutex is not held exclusively by this thread.
	* May fail to crash when it should; will never crash when it should not
	*
	*/
	void AssertHeld();
private:
	/**
	* @brief �ڲ��ṹ
	*
	*/
	struct NFInternal;
	NFInternal* mInternal;

	/**
	* @brief ��ֹ����
	*
	*/
	NF_DISALLOW_EVIL_CONSTRUCTORS(NFMutex);
};

/**
* @brief �Զ��ͷŵ��߳�����װ,�ο�google protobuf
*
*/
class NFMutexLock
{
public:
	/**
	* @brief ���캯��.
	*
	*/
	explicit NFMutexLock(NFMutex* mu) : mu_(mu)
	{
		this->mu_->Lock();
	}

	/**
	* @brief ��������.
	*
	*/
	~NFMutexLock()
	{
		this->mu_->Unlock();
	}

private:
	NFMutex* const mu_;
	/**
	* @brief ��ֹ����
	*
	*/
	NF_DISALLOW_EVIL_CONSTRUCTORS(NFMutexLock);
};

