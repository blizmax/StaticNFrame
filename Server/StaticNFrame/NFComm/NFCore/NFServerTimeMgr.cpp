// -------------------------------------------------------------------------
//    @FileName         :    NFServerTimeMgr.cpp
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFServerTimeMgr.h"
#include "NFPlatform.h"

uint64_t NFServerTimeMgr::ms_startTick = 0;
uint64_t NFServerTimeMgr::ms_curTick = 0;
uint64_t NFServerTimeMgr::ms_lastTick = 0;
int      NFServerTimeMgr::ms_deltaTick = 0;
uint64_t NFServerTimeMgr::ms_unixSec = 0;
uint64_t NFServerTimeMgr::ms_frames = 0;
uint64_t NFServerTimeMgr::ms_secs = 0;
bool     NFServerTimeMgr::ms_perSecFirstFrame = 0;
uint64_t NFServerTimeMgr::ms_lastUnixSec = 0;
int      NFServerTimeMgr::ms_fps = 0;

NFServerTimeMgr::NFServerTimeMgr()
{}

NFServerTimeMgr::~NFServerTimeMgr()
{}

bool NFServerTimeMgr::Init(int fps)
{
	ms_fps = fps;
	ms_curTick = NFGetTime();
	ms_startTick = ms_curTick;
	ms_lastTick = ms_curTick;
	ms_deltaTick = 0;
	ms_unixSec = NFGetSecondTime();
	ms_frames = 0;
	ms_secs = 0;
	ms_perSecFirstFrame = false;
	ms_lastUnixSec = ms_unixSec;
	return true;
}

bool NFServerTimeMgr::UnInit()
{
	return true;
}

bool NFServerTimeMgr::Update(uint64_t tick)
{
	ms_lastTick = ms_curTick;
	ms_curTick = tick;
	ms_deltaTick = (int)(ms_curTick - ms_lastTick);
	ms_unixSec = NFGetSecondTime();

	++ms_frames;

	if (ms_lastUnixSec > ms_unixSec)
	{
		ms_secs += ms_unixSec - ms_lastUnixSec;
		ms_lastUnixSec = ms_unixSec;
		ms_perSecFirstFrame = true;
	}
	else
	{
		ms_perSecFirstFrame = false;
	}

	return true;
}