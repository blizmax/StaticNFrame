// -------------------------------------------------------------------------
//    @FileName         :    NFSystemInfo.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-12-23
//    @Module           :    NFSystemInfo
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFSystemInfo.h"
#include <iostream>
#include "sigar.h"

#include "NFComm/NFPluginModule/NFLogMgr.h"

static sigar_t *m_sigarproclist = 0;
static sigar_t *m_sigarcpulist = 0;
static sigar_t *m_sigar = 0;

NFSystemInfo::NFSystemInfo()
{
	mCpuCount = 1;
}

void NFSystemInfo::Init()
{
	mCurPid = GetProcessPid();
	sigar_open(&m_sigar);
	sigar_open(&m_sigarproclist);
	sigar_open(&m_sigarcpulist);
	CountCpu();
}

NFSystemInfo::~NFSystemInfo()
{
	sigar_close(m_sigarcpulist);
	sigar_close(m_sigarproclist);
	sigar_close(m_sigar);
}

uint32_t NFSystemInfo::GetProcessPid()
{
	return (uint32_t)getpid();
}

void NFSystemInfo::CountCpu()
{
	int status = SIGAR_OK;

	sigar_cpu_list_t cpulist;

	status = sigar_cpu_list_get(m_sigarcpulist, &cpulist);

	if (status != SIGAR_OK)
	{
		mCpuCount = 1;
	}
	else
	{
		mCpuCount = cpulist.number;
		sigar_cpu_list_destroy(m_sigarcpulist, &cpulist);
	}
}

uint32_t NFSystemInfo::GetCpuCount() const
{
	return mCpuCount;
}

float NFSystemInfo::GetCurCpuPer() const
{
	return mCurCpuPer;
}

uint64_t NFSystemInfo::GetCurMemPer() const
{
	return mCurMemPer;
}

void NFSystemInfo::CountCurCpuPer()
{
	sigar_proc_cpu_t cpu;

	int status = sigar_proc_cpu_get(m_sigarproclist, mCurPid, &cpu);
	if (status == SIGAR_OK)
	{
		mCurCpuPer = float(cpu.percent) * 100.f;

#if NF_PLATFORM == NF_PLATFORM_WIN
		mCurCpuPer /= float(mCpuCount);
#endif
	}
	else
	{
		NFLogError("sigar_proc_cpu_get cpu error, cur_pid:{}", mCurPid);
	}

	sigar_proc_state_t procstate;
	if (sigar_proc_state_get(m_sigarproclist, mCurPid, &procstate) != SIGAR_OK)
	{
		NFLogError("sigar_proc_state_get error, cur_pid:{}", mCurPid);
	}
}

void NFSystemInfo::CountCurMemPer()
{
	int status;

	sigar_proc_mem_t proc_mem;
	status = sigar_proc_mem_get(m_sigarproclist, mCurPid, &proc_mem);

	if (status == SIGAR_OK)
	{
		mCurMemPer = proc_mem.resident;
	}
	else
	{
		NFLogError("sigar_proc_mem_get error, cur_pid:{}", mCurPid);
	}
}

NFMemInfo NFSystemInfo::GetMemInfo() const
{
	return mMachineMemInfo;
}

void NFSystemInfo::CountSystemInfo()
{
	static uint64_t lastTime = 0;

	if (lastTime + 10 > (uint64_t)NFGetSecondTime()) return;

	lastTime = (uint64_t)NFGetSecondTime();

	CountMemInfo();
	CountCurCpuPer();
	CountCurMemPer();
}

void NFSystemInfo::CountMemInfo()
{
	sigar_mem_t sigar_mem;
	sigar_mem_get(m_sigar, &sigar_mem);

	mMachineMemInfo.mTotalMem = sigar_mem.total;
	mMachineMemInfo.mFreeMem = sigar_mem.actual_free;
	mMachineMemInfo.mUsedMem = sigar_mem.actual_used;
}
