/**
* 共享内存
*/

#ifndef __CTLIB_SHM_H__
#define __CTLIB_SHM_H__

#include "CTBase.h"

namespace CTLib
{

#ifdef WIN32

class CTShm
{
public:
	CTShm() : m_uiKey(0), m_uiSize(0), m_iShm(0), m_pszBuf(0)
	{
	}

	~CTShm()
	{
		Close();
	}

	// 创建或连接上一块共享内存
	int Create(const char* pszKey, size_t sSize)
	{
		CT_ASSERT(pszKey != NULL);

		m_sKey.assign(pszKey);
		m_uiSize = sSize;

		m_hShm = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_uiSize, m_sKey.c_str());
		if (m_hShm && (CT_ERRNO == ERROR_ALREADY_EXISTS))
		{
			CloseHandle(m_hShm);
			m_hShm = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_sKey.c_str());
			if (!m_hShm)
			{
				CT_ERROR(("OpenFileMapping: %d", CT_ERRNO));
				return -1;
			}
		}

		if (m_hShm)
		{
			m_pszBuf = (char*) MapViewOfFile(m_hShm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!m_pszBuf)
			{
				CT_ERROR(("MapViewOfFile: %d, Handle=%x", CT_ERRNO, m_hShm));
				CloseHandle(m_hShm);
				return -1;
			}

			return 0;
		}
		else
		{
			CT_ERROR(("CreateFileMapping: %d", CT_ERRNO));
			return -1;
		}
	}

	// 关闭一块共享内存
	int Close()
	{
		if (m_pszBuf)
		{
			if (!UnmapViewOfFile(m_pszBuf))
			{
				CT_WARNING(("UnmapViewOfFile: %d", CT_ERRNO));
			}
			m_pszBuf = NULL;
		}

		if (m_hShm)
		{
			if (!CloseHandle(m_hShm))
			{
				CT_WARNING(("CloseHandle: %d", CT_ERRNO));
			}
			m_hShm = NULL;
		}

		return 0;
	}

public:
	char* GetBuffer() const { return m_pszBuf; }
	const char* GetKey() const { return m_sKey.c_str(); }
	unsigned int GetSize() const { return m_uiSize; }

protected:
	string	    m_sKey;
	size_t    m_uiSize;

	HANDLE	        m_hShm;
	char*	        m_pszBuf;
};

#else

#include <sys/ipc.h>
#include <sys/shm.h>

class CTShm
{
public:
	CTShm() : m_uiKey(0), m_uiSize(0), m_iShm(0), m_pszBuf(0)
	{
	}

	// 退出时不关闭共享内存
	~CTShm()
	{
	}

	// 创建或连接上一块共享内存
	int Create(key_t uiKey, size_t sSize)
	{
		m_uiKey = (unsigned int) uiKey;
		m_uiSize = sSize;

		m_iShm = shmget(uiKey, m_uiSize, IPC_CREAT|IPC_EXCL|0666);
		if (m_iShm < 0)
		{
			if (CT_ERRNO != EEXIST)
			{
				CT_ERROR(("shmget create: %d", CT_ERRNO));
				return -1;
			}

			m_iShm = shmget(uiKey, m_uiSize, 0666);
			if (m_iShm < 0)
			{
				m_iShm = shmget(uiKey, 0, 0666);
				if (m_iShm < 0)
				{
					CT_ERROR(("shmget 0: %d", CT_ERRNO));
					return -2;
				}

				if (shmctl(m_iShm, IPC_RMID, 0))
				{
					CT_ERROR(("shmctl %d: %d", m_iShm, CT_ERRNO));
					return -3;
				}

				m_iShm = shmget(uiKey, m_uiSize, IPC_CREAT|IPC_EXCL|0666);
				if (m_iShm < 0)
				{
					CT_ERROR(("shmget recreate: %d", CT_ERRNO));
					return -4;
				}
			}
            CT_TRACE(("attach shm ok, id = %d", m_iShm));
		}
        else
        {
            CT_TRACE(("create shm ok, id = %d", m_iShm));
        }

		m_pszBuf = (char*) shmat(m_iShm, 0, 0);
		if (!m_pszBuf)
		{
			CT_ERROR(("shmat %d: %d", m_iShm, CT_ERRNO));
			return -5;
		}

		return 0;
	}

    // 创建或连接上一块共享内存
    // pszPathName是ftok的参数, pszPathName对应的文件必须要存在
    int Create(const char* pszPathName, size_t sSize)
    {
        //获取文件所在目录
        string s = pszPathName;
        if (s.rfind('/') == string::npos || s[s.length()-1] == '/')
        {
            CT_ERROR(("ftok(%s) failed, file path error", pszPathName));
            return -1;
        }
        s = s.substr(0, s.rfind('/')); //取文件所在目录
        //iKey将作为ftok的第二参数再次参与运算，防止重复
        key_t iKey = ftok(s.c_str(), 0);
        CT_TRACE(("dir key = %d", iKey));

        //防止重复
        iKey = ftok(pszPathName, iKey);
        if (iKey == -1)
        {
            CT_ERROR(("ftok(%s) failed %d", pszPathName, CT_ERRNO));
            return -1;
        }
    
        CT_TRACE(("dir key2 = %d", iKey));
        return Create(iKey, sSize);
    }

	// 关闭一块共享内存
	int Close()
	{
		if (m_iShm > 0)
		{
			if (shmctl(m_iShm, IPC_RMID, 0))
			{
				CT_ERROR(("shmctl %d: %d", m_iShm, CT_ERRNO));
				return -1;
			}
		}
		return 0;		
	}

	char* GetBuffer() const { return m_pszBuf; }
	key_t GetKey() const { return m_uiKey; }
	size_t GetSize() const { return m_uiSize; }

protected:
	unsigned int	m_uiKey;
	size_t	m_uiSize;
	int				m_iShm;
	char*			m_pszBuf;
};

#endif

} // namespace CTLib

#endif //__CTLIB_SHM_H__
