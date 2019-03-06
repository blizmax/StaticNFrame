#include "NFShm.h"

#include <cassert>
#include <errno.h>
#include "NFStringUtility.h"


NFShm::NFShm(size_t iShmSize, key_t iKey, bool bOwner)
{
	init(iShmSize, iKey, bOwner);
}

NFShm::~NFShm()
{
	if (_bOwner)
	{
		detach();
	}
}

void NFShm::init(size_t iShmSize, key_t iKey, bool bOwner)
{
	assert(_pshm == NULL);

	_bOwner = bOwner;

#if NF_PLATFORM == NF_PLATFORM_WIN
	std::string shmFileName = NF_FORMAT("shm_key_{}.bus", iKey);
	std::wstring wShmFileName = NFStringUtility::s2ws(shmFileName);
	_shemID = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, wShmFileName.c_str());
	
	if (_shemID == nullptr)
	{
		_bCreate = true;

		_shemID = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(iShmSize), wShmFileName.c_str());

		if (_shemID == nullptr)
		{
			throw NFShmException("[NFShm::init()] CreateFileMapping error", errno);
		}
	}
	else
	{
		_bCreate = false;
	}

	_pshm = (void*)MapViewOfFile(_shemID, FILE_MAP_ALL_ACCESS, 0, 0, iShmSize);
	if (_pshm == nullptr)
	{
		CloseHandle(_shemID);
		throw NFShmException("[NFShm::init()] MapViewOfFile error", errno);
	}

#else
	//注意_bCreate的赋值位置:保证多线程用一个对象的时候也不会有问题
	//试图创建
	if ((_shemID = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		_bCreate = false;
		//有可能是已经存在同样的key_shm,则试图连接
		if ((_shemID = shmget(iKey, iShmSize, 0666)) < 0)
		{
			throw NFShmException("[TC_Shm::init()] shmget error", errno);
		}
	}
	else
	{
		_bCreate = true;
	}

	//try to access shm
	if ((_pshm = shmat(_shemID, NULL, 0)) == (char *)-1)
	{
		throw NFShmException("[TC_Shm::init()] shmat error", errno);
	}
#endif
	_shmSize = iShmSize;
	_shmKey = iKey;
}

int NFShm::detach()
{
	int iRetCode = 0;
#if NF_PLATFORM == NF_PLATFORM_WIN
	if (_pshm != NULL)
	{
		UnmapViewOfFile(_pshm);
		CloseHandle(_shemID);

		_shemID = NULL;
		_pshm = NULL;
	}
#else
	if (_pshm != NULL)
	{
		iRetCode = shmdt(_pshm);

		_shemID = -1;
		_pshm = NULL;
	}
#endif

	return iRetCode;
}

int NFShm::del()
{
	int iRetCode = 0;
#if NF_PLATFORM == NF_PLATFORM_WIN
	if (_pshm != NULL)
	{
		UnmapViewOfFile(_pshm);
		CloseHandle(_shemID);

		_shemID = NULL;
		_pshm = NULL;
	}
#else
	if (_pshm != NULL)
	{
		iRetCode = shmctl(_shemID, IPC_RMID, 0);

		_pshm = NULL;
	}
#endif

	return iRetCode;
}