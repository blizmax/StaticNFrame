// -------------------------------------------------------------------------
//    @FileName         :    NFDuration.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------
#pragma once

#include "NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif

// A Duration represents the elapsed time between two instants
// as an int64 nanosecond count. The representation limits the
// largest representable duration to approximately 290 years.
class NFDuration
{
public:
#if NF_PLATFORM == NF_PLATFORM_WIN
	static int64_t kNanosecond; // = 1LL
	static int64_t kMicrosecond;// = 1000
	static int64_t kMillisecond;// = 1000 * kMicrosecond
	static int64_t kSecond; // = 1000 * kMillisecond
	static int64_t kMinute; // = 60 * kSecond
	static int64_t kHour; // = 60 * kMinute
#else
	static int64_t kNanosecond = 1LL;
	static int64_t kMicrosecond = 1000;
	static int64_t kMillisecond = 1000 * kMicrosecond;
	static int64_t kSecond = 1000 * kMillisecond;
	static int64_t kMinute = 60 * kSecond;
	static int64_t kHour = 60 * kMinute;
#endif
public:
	NFDuration();
	explicit NFDuration(const struct timeval& t);
	explicit NFDuration(int64_t nanoseconds);
	explicit NFDuration(int nanoseconds);
	explicit NFDuration(double seconds);

	// Nanoseconds returns the duration as an integer nanosecond count.
	int64_t Nanoseconds() const;

	// These methods return double because the dominant
	// use case is for printing a floating point number like 1.5s, and
	// a truncation to integer would make them not useful in those cases.

	// Seconds returns the duration as a floating point number of seconds.
	double Seconds() const;

	double Milliseconds() const;
	double Microseconds() const;
	double Minutes() const;
	double Hours() const;

	struct timeval TimeVal() const;
	void To(struct timeval* t) const;

	bool IsZero() const;
	bool operator<(const NFDuration& rhs) const;
	bool operator<=(const NFDuration& rhs) const;
	bool operator>(const NFDuration& rhs) const;
	bool operator>=(const NFDuration& rhs) const;
	bool operator==(const NFDuration& rhs) const;

	NFDuration operator+=(const NFDuration& rhs);
	NFDuration operator-=(const NFDuration& rhs);
	NFDuration operator*=(int ns);
	NFDuration operator/=(int ns);

private:
	int64_t ns_; // nanoseconds
};

inline NFDuration::NFDuration()
	: ns_(0)
{
}

inline NFDuration::NFDuration(const struct timeval& t)
	: ns_(t.tv_sec * kSecond + t.tv_usec * kMicrosecond)
{
}

inline NFDuration::NFDuration(int64_t nanoseconds)
	: ns_(nanoseconds)
{
}

inline NFDuration::NFDuration(int nanoseconds)
	: ns_(nanoseconds)
{
}

inline NFDuration::NFDuration(double seconds)
	: ns_((int64_t)(seconds * kSecond))
{
}

inline int64_t NFDuration::Nanoseconds() const
{
	return ns_;
}

inline double NFDuration::Seconds() const
{
	return double(ns_) / kSecond;
}

inline double NFDuration::Milliseconds() const
{
	return double(ns_) / kMillisecond;
}

inline double NFDuration::Microseconds() const
{
	return double(ns_) / kMicrosecond;
}

inline double NFDuration::Minutes() const
{
	return double(ns_) / kMinute;
}

inline double NFDuration::Hours() const
{
	return double(ns_) / kHour;
}

inline bool NFDuration::IsZero() const
{
	return ns_ == 0;
}

inline struct timeval NFDuration::TimeVal() const
{
	struct timeval t;
	To(&t);
	return t;
}

inline void NFDuration::To(struct timeval* t) const
{
	t->tv_sec = (long)(ns_ / kSecond);
	t->tv_usec = (long)(ns_ % kSecond) / (long)kMicrosecond;
}

inline bool NFDuration::operator<(const NFDuration& rhs) const
{
	return ns_ < rhs.ns_;
}

inline bool NFDuration::operator<=(const NFDuration& rhs) const
{
	return ns_ <= rhs.ns_;
}

inline bool NFDuration::operator>(const NFDuration& rhs) const
{
	return ns_ > rhs.ns_;
}

inline bool NFDuration::operator>=(const NFDuration& rhs) const
{
	return ns_ >= rhs.ns_;
}

inline bool NFDuration::operator==(const NFDuration& rhs) const
{
	return ns_ == rhs.ns_;
}

inline NFDuration NFDuration::operator+=(const NFDuration& rhs)
{
	ns_ += rhs.ns_;
	return *this;
}

inline NFDuration NFDuration::operator-=(const NFDuration& rhs)
{
	ns_ -= rhs.ns_;
	return *this;
}

inline NFDuration NFDuration::operator*=(int n)
{
	ns_ *= n;
	return *this;
}

inline NFDuration NFDuration::operator/=(int n)
{
	ns_ /= n;
	return *this;
}


