// -------------------------------------------------------------------------
//    @FileName         :    NFSystemInfo.h
//    @Author           :    GaoYi
//    @Date             :    2018-12-23
//    @Module           :    NFSystemInfo
//    @Desc             :
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"

class NFMemInfo
{
public:
	NFMemInfo()
	{
		mTotalMem = 0;
		mFreeMem = 0;
		mUsedMem = 0;
		mUsedPercent = 0.0;
		mFreePercent = 0.0;
	}

	uint64_t mTotalMem;		//ȫ���ڴ�
	uint64_t mFreeMem;		//���õ��ڴ�
	uint64_t mUsedMem;		//�Ѿ�ʹ�õ��ڴ�
	double mUsedPercent;		//ʹ����
	double mFreePercent;		//������
};

class NFCpuInfo
{
public:

};

class NFOsInfo
{
public:
	NFOsInfo()
	{

	}

	std::string mOsName;
	std::string mOsVersion;
	std::string mOsArch;
	std::string mOsMachine;
	std::string mOsDescription;
	std::string mPatchLevel;
	std::string mVendor;
	std::string mVendorVersion;
	std::string mVerdorName;
	std::string mVendorCodeName;
};

class NFProcessInfo
{
public:
	NFProcessInfo()
	{
		mPid = 0;
		mCpuUsed = 0.0;
		mMemUsed = 0;
		mThreads = 0;
	}

	std::string mName;	//��������
	std::string mCwd;	//����Ŀ¼

	uint32_t mPid = 0;
	double   mCpuUsed = 0.0;
	uint64_t mMemUsed = 0;
	uint32_t mThreads = 0;
};

class _NFExport NFSystemInfo
{
public:
	NFSystemInfo();
	virtual ~NFSystemInfo();

	virtual void Init();

	virtual uint32_t GetProcessPid(); //��õ�ǰ���ӵĽ���ID

	virtual const NFMemInfo& GetMemInfo() const;
	virtual const NFProcessInfo& GetProcessInfo() const;
	virtual const NFOsInfo& GetOsInfo() const;
	virtual float GetCurCpuPer() const;
	virtual uint64_t GetCurMemPer() const;
	virtual uint32_t GetCpuCount() const;

	virtual double CountProcessCpuPer(uint32_t pid);
	virtual uint64_t CountProcessMemPer(uint32_t pid);

	virtual void CountCurProcessBaseInfo();
	virtual void CountCpu();
	virtual void CountMemInfo();
	virtual void CountCurProcessInfo();
	virtual void CountSystemInfo();
	virtual void CountOsInfo();

	virtual uint32_t GetUserCount() const;
	virtual void SetUserCount(uint32_t userCount);
private:
	NFProcessInfo mCurProcessInfo;
	uint32_t mCpuCount = 1;
	NFMemInfo mMachineMemInfo; //��ǰ������ӵ��ڴ���Ϣ
	NFOsInfo mMachineOsInfo;
	uint32_t mUserCount = 0; //��ǰ�����������Ŀ
};
