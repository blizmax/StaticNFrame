// -------------------------------------------------------------------------
//    @FileName         :    NFMemPolicy.h
//    @Author           :    GaoYi
//    @Date             :    2019-3-6
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"
#include "NFShm.h"

//////////////////////////////////////////////////////////////////////
// �洢����: �ڴ�, �����ڴ�, mmap(�ļ�)

/**
* �ڴ�洢
*/
template<typename T>
class NFMemStorePolicy
{
public:
	/**
	* ��ʼ��
	* @param pAddr: ָ����пռ��ָ��
	* @param iSize: �ռ��ָ��
	*/
	void create(void *pAddr, size_t iSize)
	{
		_t.create(pAddr, iSize);
	}

	/**
	* �����϶���
	* @param pAddr: ָ����пռ��ָ��
	* @param iSize: �ռ��ָ��
	*/
	void connect(void *pAddr, size_t iSize)
	{
		_t.connect(pAddr, iSize);
	}

protected:
	T   _t;
};

/**
* �����ڴ�洢
*/
template<typename T>
class ShmStorePolicy
{
public:
	/**
	* ��ʼ������洢
	* @param iShmKey
	* @param iSize
	*/
	void initStore(key_t iShmKey, size_t iSize)
	{
		_shm.init(iSize, iShmKey);
		if (_shm.iscreate())
		{
			_t.create(_shm.getPointer(), iSize);
		}
		else
		{
			_t.connect(_shm.getPointer(), iSize);
		}
	}

	/**
	* �ͷŹ����ڴ�
	*/
	void release()
	{
		_shm.del();
	}
protected:
	NFShm  _shm;
	T       _t;
};


