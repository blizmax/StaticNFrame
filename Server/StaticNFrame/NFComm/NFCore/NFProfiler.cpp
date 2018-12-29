// -------------------------------------------------------------------------
//    @FileName         :    NFProfiler.cpp
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFProfiler.h"
#include "NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include <string>
#include <vector>

void NFProfiler::BeginProfiler(PROFILE_TIMER* timer)
{
	if (!mIsOpenProfiler)
	{
		return;
	}

	if (!timer || (mStackLevel == NF_ARRAYSIZE(mStacks)))
	{
		NF_ASSERT_MSG(false, "");
		return;
	}

	if (timer->index < 0)
	{
		timer->index = mTimerCount;
		mTimers[mTimerCount++] = timer;
		timer->level = mStackLevel;
	}
	// save parent
	if (mStackLevel > 0)
	{
		PROFILE_TIMER* parent = mStacks[mStackLevel - 1];
		if (timer->parentIndex == PROFILER_NO_PARENT)
		{
			timer->parentIndex = parent->index;
			timer->level = mStackLevel;
		}
		else if (timer->parentIndex != parent->index)
		{
			timer->parentIndex = PROFILER_MULTI_PARENT;
		}
	}

	mStacks[mStackLevel++] = timer;
	timer->beginTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void NFProfiler::EndProfiler()
{
	if (!mIsOpenProfiler)
	{
		return;
	}

	if (mStackLevel == 0)
	{
		NF_ASSERT_MSG(false, "Not match BeginProfile()");
		return;
	}

	PROFILE_TIMER* timer = mStacks[--mStackLevel];

	//  static const long long NonoSecond = 1000000000LL;

	long long diffNanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer->beginTime;

	timer->sampleTime += diffNanosecond;
	timer->sampleCount += 1;
}

void NFProfiler::SetProfilerThreadID()
{
	mProfileThreadID = ThreadId();
}

void NFProfiler::BeginProfilerWithThreadCheck(PROFILE_TIMER* timer)
{
	if (mProfileThreadID == ThreadId())
	{
		BeginProfiler(timer);
	}
}

void NFProfiler::EndProfilerWithThreadCheck()
{
	if (mProfileThreadID == ThreadId())
	{
		EndProfiler();
	}
}

void NFProfiler::ResetAllProfilerTimer()
{
	for (unsigned i = 0; i < mTimerCount; ++i)
	{
		auto timer = mTimers[i];
		timer->Clear();
	}
	mTimerCount = 0;
	mStackLevel = 0;
}

void NFProfiler::AddChildWithSort(CALL_TREE_NODE* parent, CALL_TREE_NODE* child)
{
	CALL_TREE_NODE* pre = nullptr;
	for (CALL_TREE_NODE* node = parent->firstChild; node; node = node->nextBrather)
	{
		if (node->sampleTime < child->sampleTime)
		{
			break;
		}
		pre = node;
	}
	if (pre)
	{
		child->nextBrather = pre->nextBrather;
		pre->nextBrather = child;
	}
	else
	{
		child->nextBrather = parent->firstChild;
		parent->firstChild = child;
	}
}

bool NFProfiler::BuildCallTree(CALL_TREE_NODE* head, std::vector<CALL_TREE_NODE>* callTree)
{
	CALL_TREE_NODE node;
	unsigned timerCount = 0;

	memset(head, 0, sizeof(*head));
	// init
	callTree->reserve(mTimerCount * 2); // must reserve enouth, otherwise pushback() may realloc the memeory, and the pointer to child will be invalid
	for (unsigned i = 0; i < mTimerCount; ++i)
	{
		PROFILE_TIMER* timer = mTimers[i];

		node.name = &(timer->name[0]);
		node.parentIndex = timer->parentIndex;
		node.sampleCount = timer->sampleCount;
		node.sampleTime = timer->sampleTime;

		node.firstChild = nullptr;
		node.nextBrather = nullptr;
		callTree->push_back(node);
	}

	// build tree
	timerCount = (unsigned)callTree->size();
	CALL_TREE_NODE* nodeArray = callTree->data();
	for (unsigned i = 0; i < timerCount; ++i)
	{
		CALL_TREE_NODE& node = nodeArray[i];
		if (node.parentIndex >= 0)
		{
			AddChildWithSort(&nodeArray[node.parentIndex], &node);
		}
		else
		{
			AddChildWithSort(head, &node);
		}
	}
	return true;
}

void NFProfiler::OutputNode(
	const CALL_TREE_NODE& node,
	bool showSplitLine,
	long long totalTime,
	int level,
	std::string* report
)
{
	static const int LEVEL_INDENT = 4;
	char line[PROFILER_MAX_STACK_LEVEL * LEVEL_INDENT + PROFILER_MAX_TIMER_NAME_LEN + 128];

	int indentLength = LEVEL_INDENT * level;
	memset(line, ' ', indentLength);
	for (int i = 0; i < level; ++i)
	{
		line[LEVEL_INDENT * i] = '|';
	}

	// split line
	if (showSplitLine)
	{
		line[indentLength] = '|';
		line[indentLength + 1] = '\t';
		line[indentLength + 2] = '\n';
		line[indentLength + 3] = '\0';
		report->append(line);
	}

	char* writePos = line + indentLength;
	int freeSize = NF_ARRAYSIZE(line) - indentLength;
	if (totalTime > 0)
	{
		snprintf(
			writePos,
			freeSize,
			"+-[%5.2f%% %6.3fms count=%u]----%s%s\t\n",
			node.sampleTime * 100.f / totalTime,
			node.sampleTime / 1000000.f,
			node.sampleCount,
			node.name,
			(node.parentIndex == PROFILER_MULTI_PARENT) ? "<multi parent>" : ""
		);
	}

	writePos[freeSize - 1] = '\0';
	report->append(line);

	if (report->length() + NF_ARRAYSIZE(line) >= (unsigned)4092)
	{
		NFLogDebug("profile:\t\n{}", *report);
		report->clear();
	}
}

void NFProfiler::OutputCallTree(
	const CALL_TREE_NODE& node,
	long long totalTime,
	long long minShowTime,
	int level,
	std::string* report
)
{
	long long childTime = 0;
	bool lastBratherHasChild = false;

	for (CALL_TREE_NODE* child = node.firstChild; child; child = child->nextBrather)
	{
		if (child->sampleTime < minShowTime)
		{
			continue;
		}

		OutputNode(*child, lastBratherHasChild, totalTime, level, report);
		childTime += child->sampleTime;

		lastBratherHasChild = false;
		if (child->firstChild)
		{
			OutputCallTree(*child, totalTime, minShowTime, level + 1, report);
			lastBratherHasChild = true;
		}
	}

	long long other = node.sampleTime - childTime;
	if (other > minShowTime)
	{
		CALL_TREE_NODE otherNode = {0};
		otherNode.name = "<other: code not be sample and sampled but too small>";
		otherNode.sampleCount = node.sampleCount;
		otherNode.sampleTime = other;

		OutputNode(otherNode, lastBratherHasChild, totalTime, level, report);
	}
}

bool NFProfiler::OutputTopProfilerTimer()
{
	std::string report;
	CALL_TREE_NODE head;
	std::vector<CALL_TREE_NODE> tree;

	long long totalTime = 0LL;
	for (unsigned i = 0; i < mTimerCount; ++i)
	{
		PROFILE_TIMER* timer = mTimers[i];
		if (timer->level == 0)
		{
			totalTime += timer->sampleTime;
		}
	}
	long long minShowTime = (long long)(totalTime * 0.005f);

	head.sampleCount = 1;
	head.sampleTime = totalTime;
	BuildCallTree(&head, &tree);
	OutputCallTree(head, totalTime, minShowTime, 0, &report);
	NFLogDebug("profile:\r\n{}", report);
	return true;
}

