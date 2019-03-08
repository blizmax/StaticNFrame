#ifndef __CT_TIME_ESCAPE_CHECK__
#define __CT_TIME_ESCAPE_CHECK__

#ifdef WIN32
#include "base\CTBaseDefine.h"
#else
#include <sys/time.h>
#endif

#include "base/CTSingleton.h"

namespace CTLib
{

class CTTimeEscapeCheck
{
public:
	CTTimeEscapeCheck()
	{

	}

	void Reset(unsigned int* uiStartSec = NULL, unsigned int* uiStartUSec = NULL)
	{
		gettimeofday(&m_stStartTime,NULL);

		if(uiStartSec)
		{
			*uiStartSec = m_stStartTime.tv_sec;
		}

		if(uiStartUSec)
		{
			*uiStartUSec = m_stStartTime.tv_usec;
		}
	}

	unsigned int CheckUSec(unsigned int* uiStartSec = NULL, unsigned int* uiStartUSec = NULL)
	{
		gettimeofday(&m_stNow,NULL);
		unsigned int iTemp = (1000000*(m_stNow.tv_sec-m_stStartTime.tv_sec)+m_stNow.tv_usec-m_stStartTime.tv_usec) & 0xFFFFFFFF;
		Reset(uiStartSec, uiStartUSec);
		return iTemp;
	}

	unsigned int CheckMSec(unsigned int* uiStartSec = NULL, unsigned int* uiStartUSec = NULL)
	{
		return CheckUSec(uiStartSec, uiStartUSec) / 1000;
	}

	unsigned int CheckSec(unsigned int* uiStartSec = NULL, unsigned int* uiStartUSec = NULL)
	{
		return CheckUSec(uiStartSec, uiStartUSec) / 1000000;
	}

	unsigned int IntervalUSec(unsigned int uiStartSec, unsigned int uiStartUSec)
	{
		gettimeofday(&m_stNow,NULL);
		int iTemp = 1000000*(m_stNow.tv_sec-uiStartSec)+m_stNow.tv_usec-uiStartUSec;
		return (iTemp < 0 ? 0 : (unsigned int)iTemp);
	}

	unsigned int IntervalMSec(unsigned int uiStartSec, unsigned int uiStartUSec)
	{
		return IntervalUSec(uiStartSec, uiStartUSec) / 1000;
	}

	unsigned int IntervalSec(unsigned int uiStartSec, unsigned int uiStartUSec)
	{
		return IntervalUSec(uiStartSec, uiStartUSec) / 1000000;
	}

private:
	struct timeval m_stStartTime;
	struct timeval m_stNow;
};

}

#define TIME_ESCAPE_CHECK  (CTSingleton<CTTimeEscapeCheck>::Instance())

#endif

