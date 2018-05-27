// -------------------------------------------------------------------------
//    @FileName         :    NFDuration.in.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------
#pragma once

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
