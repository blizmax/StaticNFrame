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
	NFDuration() : ns_(0) { }
	explicit NFDuration(const struct timeval& t);
	explicit NFDuration(int64_t nanoseconds);
	explicit NFDuration(double seconds);

	// Nanoseconds returns the duration as an integer nanosecond count.
	int64_t Nanoseconds() const { return ns_; }

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




