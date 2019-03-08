/**
* 时间类
*/

#ifndef __CTLIB_TIME_H__
#define __CTLIB_TIME_H__

#include "CTBaseDefine.h"
#include "CTPlatform.h"

namespace CTLib
{

class CTTimeSpan
{
public:
	CTTimeSpan() {}
	CTTimeSpan(time_t time) { m_timeSpan = time; }
	CTTimeSpan(long lDays, int nHours, int nMins, int nSecs) { m_timeSpan = nSecs + 60* (nMins + 60* (nHours + 24* lDays)); }

	CTTimeSpan(const CTTimeSpan& timeSpanSrc) { m_timeSpan = timeSpanSrc.m_timeSpan; }
	const CTTimeSpan& operator=(const CTTimeSpan& timeSpanSrc) { m_timeSpan = timeSpanSrc.m_timeSpan; return *this; }

	int GetDays() const { return static_cast<int>(m_timeSpan / (24 * 3600)); }
    int GetHours() const { return static_cast<int>(GetTotalHours() - GetDays() * 24); }
    int GetMinutes() const { return static_cast<int>(GetTotalMinutes() - GetTotalHours() * 60); }
    int GetSeconds() const { return static_cast<int>(GetTotalSeconds() - GetTotalMinutes() * 60); }

    int GetTotalHours() const { return static_cast<int>(m_timeSpan / 3600); }
	int GetTotalMinutes() const { return static_cast<int>(m_timeSpan / 60); }
	int GetTotalSeconds() const { return static_cast<int>(m_timeSpan); }

	CTTimeSpan operator-(CTTimeSpan timeSpan) const { return CTTimeSpan(m_timeSpan - timeSpan.m_timeSpan); }
	CTTimeSpan operator+(CTTimeSpan timeSpan) const { return CTTimeSpan(m_timeSpan + timeSpan.m_timeSpan); }
	const CTTimeSpan& operator+=(CTTimeSpan timeSpan) { m_timeSpan += timeSpan.m_timeSpan; return *this; }
	const CTTimeSpan& operator-=(CTTimeSpan timeSpan) { m_timeSpan -= timeSpan.m_timeSpan; return *this; }
	bool operator==(CTTimeSpan timeSpan) const { return m_timeSpan == timeSpan.m_timeSpan; }
	bool operator!=(CTTimeSpan timeSpan) const { return m_timeSpan != timeSpan.m_timeSpan; }
	bool operator<(CTTimeSpan timeSpan) const  { return m_timeSpan < timeSpan.m_timeSpan; }
	bool operator>(CTTimeSpan timeSpan) const  { return m_timeSpan > timeSpan.m_timeSpan; }
	bool operator<=(CTTimeSpan timeSpan) const { return m_timeSpan <= timeSpan.m_timeSpan; }
	bool operator>=(CTTimeSpan timeSpan) const { return m_timeSpan >= timeSpan.m_timeSpan; }

    /**
    * 时间格式化.
    * 仅支持以下格式参数
    *    -  %D - 天数
    *    -  %H - 小时 (0-23)
    *    -  %M - 分 (0-59)
    *    -  %S - 秒 (0-59)
    *    -  %% - 百分符号
    */
	string Format(const char *pszFormat) const
	{
		char szBuffer[32];
		char ch = 0;
        string s;
	
		while ((ch = *pszFormat++) != '\0')
		{
			if (ch == '%')
			{
				switch (ch = *pszFormat++)
				{
				case '%':
					s += ch;
					break;
				case 'D':
                    ct_snprintf(CT_STRSIZE(szBuffer), "%04d", GetDays());
					s += szBuffer;
					break;
				case 'H':
                    ct_snprintf(CT_STRSIZE(szBuffer), "%02d", GetHours());
                    s += szBuffer;
                    break;
				case 'M':
                    ct_snprintf(CT_STRSIZE(szBuffer), "%02d", GetMinutes());
                    s += szBuffer;
                    break;
				case 'S':
                    ct_snprintf(CT_STRSIZE(szBuffer), "%02d", GetSeconds());
                    s += szBuffer;
                    break;
				default:
					return "";
				}
			}
			else
			{
                s += ch;
			}
		}

		return s;
	}

private:
	time_t m_timeSpan;
	friend class CTTime;
};	

class CTTime
{
public:
	CTTime() {}
	CTTime(time_t time)  { m_time = time; }
	CTTime(struct tm t) 
	{
		m_time = mktime(&t);
	}
	
	CTTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1)
	{
		struct tm atm;
		atm.tm_sec = nSec;
		atm.tm_min = nMin;
		atm.tm_hour = nHour;
		atm.tm_mday = nDay;
		atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
		atm.tm_year = nYear - 1900;     // tm_year is 1900 based
		atm.tm_isdst = nDST;
		m_time = mktime(&atm);
	}

    CTTime(const string& strDateTime)
    {
        struct tm stTm;
        if (strDateTime.length() == 19)	// YYYY-mm-dd HH:MM:SS
        {
            stTm.tm_year = strtol(strDateTime.substr(0, 4).c_str(), NULL, 10) - 1900;
            stTm.tm_mon = strtol(strDateTime.substr(5, 2).c_str(), NULL, 10) - 1;
            stTm.tm_mday = strtol(strDateTime.substr(8, 2).c_str(), NULL, 10);
            stTm.tm_hour = strtol(strDateTime.substr(11, 2).c_str(), NULL, 10);
            stTm.tm_min = strtol(strDateTime.substr(14, 2).c_str(), NULL, 10);
            stTm.tm_sec = strtol(strDateTime.substr(17, 2).c_str(), NULL, 10);
            m_time = mktime(&stTm);
            if (m_time == -1)
            {
                m_time = 0;
            }
        }
        else if (strDateTime.length() == 14) // YYYYmmddHHMMSS
        {
            stTm.tm_year = strtol(strDateTime.substr(0, 4).c_str(), NULL, 10) - 1900;
            stTm.tm_mon = strtol(strDateTime.substr(4, 2).c_str(), NULL, 10) - 1;
            stTm.tm_mday = strtol(strDateTime.substr(6, 2).c_str(), NULL, 10);
            stTm.tm_hour = strtol(strDateTime.substr(8, 2).c_str(), NULL, 10);
            stTm.tm_min = strtol(strDateTime.substr(10, 2).c_str(), NULL, 10);
            stTm.tm_sec = strtol(strDateTime.substr(12, 2).c_str(), NULL, 10);
            m_time = mktime(&stTm);
            if (m_time == -1)
            {
                m_time = 0;
            }
        }
        else if (strDateTime.length() == 10)	// YYYY-mm-dd
        {
            stTm.tm_year = strtol(strDateTime.substr(0, 4).c_str(), NULL, 10) - 1900;
            stTm.tm_mon = strtol(strDateTime.substr(5, 2).c_str(), NULL, 10) - 1;
            stTm.tm_mday = strtol(strDateTime.substr(8, 2).c_str(), NULL, 10);
            stTm.tm_hour = 0;
            stTm.tm_min = 0;
            stTm.tm_sec = 0;
            m_time = mktime(&stTm);
            if (m_time == -1)
            {
                m_time = 0;
            }
        }
        else if (strDateTime.length() == 8)     // YYYYmmdd
        {
            stTm.tm_year = strtol(strDateTime.substr(0, 4).c_str(), NULL, 10) - 1900;
            stTm.tm_mon = strtol(strDateTime.substr(4, 2).c_str(), NULL, 10) - 1;
            stTm.tm_mday = strtol(strDateTime.substr(6, 2).c_str(), NULL, 10);
            stTm.tm_hour = 0;
            stTm.tm_min = 0;
            stTm.tm_sec = 0;
            m_time = mktime(&stTm);
            if (m_time == -1)
            {
                m_time = 0;
            }
        }
        else
        {
            m_time = 0;
        }
    }

	CTTime(const CTTime& timeSrc) { m_time = timeSrc.m_time; }

	const CTTime& operator=(const CTTime& timeSrc) { m_time = timeSrc.m_time; return *this; }
	const CTTime& operator=(time_t t) { m_time = t; return *this; }

	struct tm* GetGmtTm(struct tm* ptm = NULL) const
	{
		if (ptm != NULL)
		{
			*ptm = *gmtime(&m_time);
			return ptm;
		}
		else
		{
			return gmtime(&m_time);
		}
	}
	
	struct tm* GetLocalTm(struct tm* ptm = NULL) const
	{
		if (ptm != NULL)
		{
			struct tm* ptmTemp = localtime(&m_time);
			if (ptmTemp == NULL)
            {
				return NULL;    // indicates the m_time was not initialized!
            }

			*ptm = *ptmTemp;
			return ptm;
		}
		else
		{
			return localtime(&m_time);
		}
	}

	time_t GetTime() const { return m_time; }
	int GetYear() const { return (GetLocalTm(NULL)->tm_year) + 1900; }
	int GetMonth() const { return GetLocalTm(NULL)->tm_mon + 1; }
	int GetDay() const { return GetLocalTm(NULL)->tm_mday; }
	int GetHour() const { return GetLocalTm(NULL)->tm_hour; }
	int GetMinute() const { return GetLocalTm(NULL)->tm_min; }
	int GetSecond() const { return GetLocalTm(NULL)->tm_sec; }

    static int GetTimeZoneHour() { return 8;}
    static int GetTimeZoneSec() { return GetTimeZoneHour() * 3600; }

    /**
    * 返回星期几.
    * @return 0=星期天 1=星期一 6=星期六
    */
	int GetDayOfWeek() const { return GetLocalTm(NULL)->tm_wday; }

	CTTimeSpan operator-(CTTime time) const { return CTTimeSpan(m_time - time.m_time); }
	CTTime operator-(CTTimeSpan timeSpan) const { return CTTime(m_time - timeSpan.m_timeSpan); }
	CTTime operator+(CTTimeSpan timeSpan) const { return CTTime(m_time + timeSpan.m_timeSpan); }
	const CTTime& operator+=(CTTimeSpan timeSpan) { m_time += timeSpan.m_timeSpan; return *this; }
	const CTTime& operator-=(CTTimeSpan timeSpan) { m_time -= timeSpan.m_timeSpan; return *this; }
	bool operator==(CTTime time) const { return m_time == time.m_time; }
	bool operator!=(CTTime time) const { return m_time != time.m_time; }
	bool operator<(CTTime time) const  { return m_time < time.m_time; }
	bool operator>(CTTime time) const  { return m_time > time.m_time; }
	bool operator<=(CTTime time) const { return m_time <= time.m_time; }
	bool operator>=(CTTime time) const { return m_time >= time.m_time; }

	string Format(const char* pszFormat) const
	{
        time_t time = m_time;
		struct tm* ptmTemp = localtime(&time);

        char szBuffer[256];
        if (ptmTemp == NULL || !strftime(CT_STRSIZE(szBuffer), pszFormat, ptmTemp))
		{
			return "";
		}

		return string(szBuffer);	
	}

	string FormatGmt(const char* pszFormat) const
	{
        time_t time = m_time;
        struct tm* ptmTemp = gmtime(&time);

        char szBuffer[256];
        if (ptmTemp == NULL || !strftime(CT_STRSIZE(szBuffer), pszFormat, ptmTemp))
        {
            return "";
        }

        return string(szBuffer);	
	}

    char* Format(char* pszBuffer, int iMaxLen, const char* pszFormat) const
    {
        time_t time = m_time;
        struct tm* ptmTemp = localtime(&time);
        if (ptmTemp == NULL || !strftime(pszBuffer, iMaxLen, pszFormat, ptmTemp))
        {
            pszBuffer[0] = '\0';
        }
        return pszBuffer;
    }

    char* FormatGmt(char* pszBuffer, int iMaxLen, const char* pszFormat) const
    {
        time_t time = m_time;
        struct tm* ptmTemp = gmtime(&time);
        if (ptmTemp == NULL || !strftime(pszBuffer, iMaxLen, pszFormat, ptmTemp))
        {
            pszBuffer[0] = '\0';
        }
        return pszBuffer;
    }

private:
	time_t m_time;
};

class CTTimeUtils
{
public:
	static struct timeval& GetNow(struct timeval &tv)
	{
#ifdef WIN32
		SYSTEMTIME sy;
		GetSystemTime(&sy);
		tv.tv_usec = sy.wMilliseconds * 1000;
		tv.tv_sec = static_cast<long>(time(0));
#else
		gettimeofday(&tv, NULL);
#endif

        return tv;
	}

    /**
    * 计算两个时间的差
    * @return 返回差值, 单位微妙
    */
	static int Diff(const struct timeval& tv1, const struct timeval& tv2)
	{
		return (tv1.tv_sec - tv2.tv_sec) * 1000000 + 
			   (tv1.tv_usec - tv2.tv_usec);
	}

    /**
    * 比较两个时间是否是同一天, tTimeOffset为临界值偏移量，如果=0表示以24点为基准，如果=3600，表示以1点为基准
    * @return true/false
    */
    static bool IsSameDay(time_t tTime1, time_t tTime2, time_t tTimeOffset = 0)
    {
        if (tTime1 == 0 || tTime2 == 0)
        {
            return false;
        }

        //修正参数
        if (tTimeOffset < 0)
        {
            tTimeOffset = 0;
        }
        else if (tTimeOffset > 86400)
        {
            tTimeOffset = GetSecOfDay(tTimeOffset);
        }

        //计算当前偏移
        time_t tTime1Begin = 0;
        time_t tTime1End = 0;
        if (GetSecOfDay(tTime1) >= tTimeOffset) //下一天
        {
            tTime1Begin = GetDayBeginSec(tTime1) + tTimeOffset;
            tTime1End = GetDayBeginSec(tTime1) + tTimeOffset + 86400;
        }
        else                                    //当天
        {
            tTime1Begin = GetDayBeginSec(tTime1) + tTimeOffset - 86400;
            tTime1End = GetDayBeginSec(tTime1) + tTimeOffset;
        }

        return ((tTime2 >= tTime1Begin) && (tTime2 < tTime1End));
    }

    static bool IsYesterday(time_t tTime1, time_t tTime2, time_t tTimeOffset = 0)
    {
        return IsSameDay(tTime1, tTime2-86400, tTimeOffset);
    }

    /**
    * 比较两个时间是否是同一月
    * @return true/false
    */
    static bool IsSameMonth(time_t tTime1, time_t tTime2)
    {
        if (tTime1 <= 0 || tTime2 <= 0)
        {
            return false;
        }

		struct tm* ptmTime1 = localtime((time_t*)&tTime1);
        if (NULL == ptmTime1)
        {
            return false;
        }

        int iYearTime1 = static_cast<int>(ptmTime1->tm_year);
        int iMonthTime1 = static_cast<int>(ptmTime1->tm_mon);

		struct tm* ptmTime2 = localtime((time_t*)&tTime2);
        if (NULL == ptmTime2)
        {
            return false;
        }

        int iYearTime2 = static_cast<int>(ptmTime2->tm_year);
        int iMonthTime2 = static_cast<int>(ptmTime2->tm_mon);

        return ((iYearTime1 == iYearTime2) && (iMonthTime1 == iMonthTime2));
    }

    static int GetSecOfDay(time_t tTime)
    {
        return (tTime + CTTime::GetTimeZoneSec()) % 86400;
    }

    static int GetDaysOfSec(time_t tTime)
    {
        return (tTime + CTTime::GetTimeZoneSec()) / 86400;
    } 

    //计算一天0点时间
    static int GetDayBeginSec(time_t tTime)
    {
        return (tTime + CTLib::CTTime::GetTimeZoneSec()) / 86400 * 86400 - CTLib::CTTime::GetTimeZoneSec();
    }

    //获取小时数. 取值范围[0，23]
    static int GetHourFromTs(time_t tTime)
    {
        return (tTime - GetDayBeginSec(tTime)) / 3600;
    }

    //通过字符串中，获得时间戳 . "2013-05-23 16:33:00".   返回<0表示解析出错
    static int GetTsFromString (const char * pInputTime)
    {
        if (strlen(pInputTime) != 19)
        {
            return -1;
        }

        const char fmt[] = "%Y-%m-%d %H:%M:%S";
        struct tm tb;
        char * pc = strptime(pInputTime, fmt, &tb);
        if (0 == pc)
        {            
            return -2;  
        }

        return (int)mktime(&tb);
    }
};

} // namespace CTLib

#endif //__CTLIB_TIME_H__
