// -------------------------------------------------------------------------
//    @FileName         :    NFDuration.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFDuration.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif

static const int64_t kNanosecond = 1LL;
static const int64_t kMicrosecond = 1000 * kNanosecond;
static const int64_t kMillisecond = 1000 * kMicrosecond;
static const int64_t kSecond = 1000 * kMillisecond;
static const int64_t kMinute = 60 * kSecond;
static const int64_t kHour = 60 * kMinute;

NFDuration::NFDuration(const struct timeval& t)
	: ns_(t.tv_sec * kSecond + t.tv_usec * kMicrosecond)
{
}

NFDuration::NFDuration(int64_t nanoseconds)
	: ns_(nanoseconds)
{
}

NFDuration::NFDuration(double seconds)
	: ns_((int64_t)(seconds * kSecond))
{
}

double NFDuration::Seconds() const
{
	return double(ns_) / kSecond;
}

double NFDuration::Milliseconds() const
{
	return double(ns_) / kMillisecond;
}

double NFDuration::Microseconds() const
{
	return double(ns_) / kMicrosecond;
}

double NFDuration::Minutes() const
{
	return double(ns_) / kMinute;
}

double NFDuration::Hours() const
{
	return double(ns_) / kHour;
}

void NFDuration::To(timeval* t) const
{
	t->tv_sec = (long)(ns_ / kSecond);
	t->tv_usec = (long)(ns_ % kSecond) / (long)kMicrosecond;
}

bool NFDuration::IsZero() const
{
	return ns_ == 0;
}

struct timeval NFDuration::TimeVal() const
{
	struct timeval t;
	To(&t);
	return t;
}

bool NFDuration::operator<(const NFDuration& rhs) const
{
	return ns_ < rhs.ns_;
}

bool NFDuration::operator<=(const NFDuration& rhs) const
{
	return ns_ <= rhs.ns_;
}

bool NFDuration::operator>(const NFDuration& rhs) const
{
	return ns_ > rhs.ns_;
}

bool NFDuration::operator>=(const NFDuration& rhs) const
{
	return ns_ >= rhs.ns_;
}

bool NFDuration::operator==(const NFDuration& rhs) const
{
	return ns_ == rhs.ns_;
}

NFDuration NFDuration::operator+=(const NFDuration& rhs)
{
	ns_ += rhs.ns_;
	return *this;
}

NFDuration NFDuration::operator-=(const NFDuration& rhs)
{
	ns_ -= rhs.ns_;
	return *this;
}

NFDuration NFDuration::operator*=(int n)
{
	ns_ *= n;
	return *this;
}

NFDuration NFDuration::operator/=(int n)
{
	ns_ /= n;
	return *this;
}

