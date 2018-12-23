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
	}

	uint64_t mTotalMem;		//全部内存
	uint64_t mFreeMem;		//释放的内存
	uint64_t mUsedMem;		//已经使用的内存
};

class NFProcessInfos
{
public:
	NFProcessInfos()
	{
		mCpu = 0.0;
		mMemUsed = 0;
	}

	float mCpu;
	uint64_t mMemUsed;
};

class NFSystemInfo
{
public:
	NFSystemInfo();
	virtual ~NFSystemInfo();

	virtual uint32_t GetProcessPid(); //获得当前机子的进程ID

	virtual NFMemInfo GetMemInfo() const;
	virtual float GetCurCpuPer() const;
	virtual uint64_t GetCurMemPer() const;
	virtual uint32_t GetCpuCount() const;

	virtual void CountCurCpuPer();
	virtual void CountCurMemPer();
	virtual void CountCpu();
	virtual void CountMemInfo();
	virtual void CountSystemInfo();
private:
	uint32_t mCurPid = 0;
	float mCurCpuPer = 0.0;
	uint64_t mCurMemPer = 0;
	uint32_t mCpuCount = 1;
	NFMemInfo mMachineMemInfo; //当前物理机子的内存信息
};