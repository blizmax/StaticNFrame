// -------------------------------------------------------------------------
//    @FileName         :    NFLoopThreadStatus.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"

#include <thread>
#include <atomic>

/**
* @brief Loop�߳�״̬
*
*/
class NFLoopTheadStatus
{
public:
	virtual ~NFLoopTheadStatus()
	{
	}

	enum Status
	{
		kNull = 0,
		kInitializing = 1,
		kInitialized = 2,
		kStarting = 3,
		kRunning = 4,
		kStopping = 5,
		kStopped = 6,
	};

	enum SubStatus
	{
		kSubStatusNull = 0,
		kStoppingListener = 1,
		kStoppingThreadPool = 2,
	};

	std::string StatusToString() const
	{
		NF_CASE_STRING_BIGIN(mStatus.load())
			;
			NF_CASE_STRING(kNull);
			NF_CASE_STRING(kInitialized);
			NF_CASE_STRING(kRunning);
			NF_CASE_STRING(kStopping);
			NF_CASE_STRING(kStopped);
			NF_CASE_STRING_END();
	}

	virtual bool IsRunning() const
	{
		return mStatus.load() == kRunning;
	}

	bool IsStopped() const
	{
		return mStatus.load() == kStopped;
	}

	bool IsStopping() const
	{
		return mStatus.load() == kStopping;
	}

protected:
	std::atomic<Status> mStatus = {kNull};
	std::atomic<SubStatus> mSubstatus = {kSubStatusNull};
};

