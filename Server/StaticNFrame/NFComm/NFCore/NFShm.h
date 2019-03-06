// -------------------------------------------------------------------------
//    @FileName         :    NFShm.h
//    @Author           :    GaoYi
//    @Date             :    2019-3-6
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"
#include "NFException.hpp"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <Windows.h>
typedef long key_t;
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

/////////////////////////////////////////////////
/**
* @file  NFShm.h
* @brief  �����ڴ��װ��.
*
*/
/////////////////////////////////////////////////

/**
* @brief �����ڴ��쳣��.
*/
struct NFShmException : public NFException
{
	NFShmException(const std::string &buffer) : NFException(buffer) {};
	NFShmException(const std::string &buffer, int err) : NFException(buffer, err) {};
	~NFShmException() throw() {};
};

/**
* @brief  �����ڴ������࣬˵��:
* 1 �������ӹ����ڴ�, �����ڴ��Ȩ���� 0666
* 2 _bOwner=false: ����ʱ��detach�����ڴ�
* 3 _bOwner=true: ����ʱdetach�����ڴ�
*/

class _NFExport NFShm
{
public:

	/**
	* @brief ���캯��.
	*
	* @param bOwner  �Ƿ�ӵ�й����ڴ�,Ĭ��Ϊfalse
	*/
#if NF_PLATFORM == NF_PLATFORM_WIN
	NFShm(bool bOwner = false) : _bOwner(bOwner), _shmSize(0), _shmKey(0), _bCreate(true), _pshm(nullptr), _shemID(nullptr) {}
#else
	NFShm(bool bOwner = false) : _bOwner(bOwner), _shmSize(0), _shmKey(0), _bCreate(true), _pshm(nullptr), _shemID(-1) {}
#endif

	/**
	* @brief ���캯��.
	*
	* @param iShmSize �����ڴ��С
	* @param iKey     �����ڴ�Key
	* @throws         TC_Shm_Exception
	*/
	NFShm(size_t iShmSize, key_t iKey, bool bOwner = false);

	/**
	* @brief ��������.
	*/
	~NFShm();

	/**
	* @brief ��ʼ��.
	*
	* @param iShmSize   �����ڴ��С
	* @param iKey       �����ڴ�Key
	* @param bOwner     �Ƿ�ӵ�й����ڴ�
	* @throws           TC_Shm_Exception
	* @return ��
	*/
	void init(size_t iShmSize, key_t iKey, bool bOwner = false);

	/**
	* @brief �жϹ����ڴ�����ͣ����ɵĹ����ڴ�,���������ϵĹ����ڴ�
	* ��������ɵĹ����ڴ�,��ʱ���Ը�����Ҫ����ʼ��
	*
	* @return  true,���ɹ����ڴ�; false, �����ϵĹ����ڴ�
	*/
	bool iscreate() { return _bCreate; }

	/**
	* @brief  ��ȡ�����ڴ��ָ��.
	*
	* @return   void* �����ڴ�ָ��
	*/
	void *getPointer() { return _pshm; }

	/**
	* @brief  ��ȡ�����ڴ�Key.
	*
	* @return key_t* ,�����ڴ�key
	*/
	key_t getkey() { return _shmKey; }

	/**
	* @brief  ��ȡ�����ڴ�ID.
	*
	* @return int ,�����ڴ�Id
	*/
#if NF_PLATFORM == NF_PLATFORM_WIN
	HANDLE getid() { return _shemID; }
#else
	int getid() { return _shemID; }
#endif

	/**
	*  @brief  ��ȡ�����ڴ��С.
	*
	* return size_t,�����ڴ��С
	*/
	size_t size() { return _shmSize; }

	/**
	*  @brief ��������ڴ棬�ڵ�ǰ�����н�������ڴ�
	* �����ڴ��ڵ�ǰ��������Ч
	* @return int
	*/
	int detach();

	/**
	*  @brief ɾ�������ڴ�.
	*
	* ��ȫɾ�������ڴ�
	*/
	int del();

protected:

	/**
	* �Ƿ�ӵ�й����ڴ�
	*/
	bool            _bOwner;

	/**
	* �����ڴ��С
	*/
	size_t          _shmSize;

	/**
	* �����ڴ�key
	*/
	key_t           _shmKey;

	/**
	* �Ƿ������ɵĹ����ڴ�
	*/
	bool            _bCreate;

	/**
	* �����ڴ�
	*/
	void            *_pshm;

	/**
	* �����ڴ�id
	*/
#if NF_PLATFORM == NF_PLATFORM_WIN
	HANDLE			_shemID;
#else
	int             _shemID;
#endif
};



