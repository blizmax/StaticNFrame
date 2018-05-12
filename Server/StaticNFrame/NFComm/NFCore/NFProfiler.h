// -------------------------------------------------------------------------
//    @FileName         :    NFProfiler.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#ifndef NF_PROFILER_H
#define NF_PROFILER_H

#include <time.h>
#include <string.h>
#include <string>
#include <vector>
#include "NFPlatform.h"
#include "NFSingleton.hpp"

#ifdef _MSC_VER
#include <windows.h>
#endif

const unsigned PROFILER_MAX_TIMER_COUNT = 4096;
const unsigned PROFILER_MAX_STACK_LEVEL = 20;
const unsigned PROFILER_MAX_TIMER_NAME_LEN = 64;

const int PROFILER_NO_PARENT = -1;
const int PROFILER_MULTI_PARENT = -2;

struct PROFILE_TIMER
{
    int index;
    int parentIndex;
    int level;
    unsigned  sampleCount;
    long long beginTime;
    long long sampleTime; // in nanosecond
    char name[PROFILER_MAX_TIMER_NAME_LEN];

    PROFILE_TIMER(const char* _name)
    {
        Clear();
        strncpy(name, _name, sizeof(name));
        name[sizeof(name) - 1] = '\0';
    }

    PROFILE_TIMER() : PROFILE_TIMER("") {  }

    void SetName(const char* _name)
    {
        strncpy(name, _name, sizeof(name));
        name[sizeof(name) - 1] = '\0';
    }

    void Clear()
    {
        index = -1;
        parentIndex = PROFILER_NO_PARENT;
        level = -1;

        memset(&beginTime, 0, sizeof(beginTime));
        sampleCount = 0;
        sampleTime = 0;
    }
};

struct CALL_TREE_NODE
{
    const char* name;
    int parentIndex;
    unsigned  sampleCount;
    long long sampleTime;

    CALL_TREE_NODE* firstChild;
    CALL_TREE_NODE* nextBrather;
};

class _NFExport NFProfiler : public NFSingleton<NFProfiler>
{
public:
    NFProfiler()
    {
        mTimerCount = 0;
        mStackLevel = 0;
        mIsOpenProfiler = true;
        mProfileThreadID = 0;
		for (int i = 0; i < (int)PROFILER_MAX_TIMER_COUNT; i++)
		{
			mTimers[i] = nullptr;
		}
		for (int i = 0; i < (int)PROFILER_MAX_STACK_LEVEL; i++)
		{
			mStacks[i] = nullptr;
		}
    }

    virtual ~NFProfiler()
    {

    }
public:
    // pointer to timer must keep always valid, normally should be a static variable.
    void BeginProfiler(PROFILE_TIMER* timer);
    void EndProfiler();
public:
    // for support profile main thread in multi thread program(ignore another, only main thread)
    void SetProfilerThreadID();
    void BeginProfilerWithThreadCheck(PROFILE_TIMER* timer);
    void EndProfilerWithThreadCheck();
    void ResetAllProfilerTimer();
public:
    bool OutputTopProfilerTimer();
protected:
    void AddChildWithSort(CALL_TREE_NODE* parent, CALL_TREE_NODE* child);
    bool BuildCallTree(CALL_TREE_NODE* head, std::vector<CALL_TREE_NODE>* callTree);
    void OutputNode(const CALL_TREE_NODE& node, bool showSplitLine, long long totalTime, int level, std::string* report);
    void OutputCallTree(const CALL_TREE_NODE& node, long long totalTime, long long minShowTime, int level, std::string* report);
private:
    bool        mIsOpenProfiler;
    NF_THREAD_ID mProfileThreadID;
private:
    unsigned    mTimerCount;
    PROFILE_TIMER* mTimers[PROFILER_MAX_TIMER_COUNT];

    unsigned mStackLevel;
    PROFILE_TIMER* mStacks[PROFILER_MAX_STACK_LEVEL];
};

#define _PROFILER_MERGE_STRING2(x,y) x##y
#define _PROFILER_MERGE_STRING(x,y) _PROFILER_MERGE_STRING2(x,y)

#define BEGIN_PROFILE(name) { \
        static PROFILE_TIMER _PROFILER_MERGE_STRING(s_timer_, __LINE__)(name); \
        NFProfiler::Instance()->BeginProfiler(&_PROFILER_MERGE_STRING(s_timer_, __LINE__))

#define END_PROFILE() NFProfiler::Instance()->EndProfiler(); }

#define PRINTF_PROFILE() NFProfiler::Instance()->OutputTopProfilerTimer();

#define CLEAR_PROFILE() NFProfiler::Instance()->ResetAllProfilerTimer();

class _NFExport ProfilerSegmentHelper
{
public:
    ProfilerSegmentHelper(PROFILE_TIMER* timer)
    {
        NFProfiler::Instance()->BeginProfiler(timer);
    }

    ~ProfilerSegmentHelper()
    {
        NFProfiler::Instance()->EndProfiler();
    }
};

#define PROFILE_SEGMENT(name) \
    static PROFILE_TIMER  _PROFILER_MERGE_STRING(s_timer_, __LINE__)(name); \
    ProfilerSegmentHelper _PROFILER_MERGE_STRING(segment_timer, __LINE__)( \
                                                                           &_PROFILER_MERGE_STRING(s_timer_, __LINE__))

#endif // NF_PROFILER_H
