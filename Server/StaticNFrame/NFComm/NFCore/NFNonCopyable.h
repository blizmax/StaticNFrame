// -------------------------------------------------------------------------
//    @FileName         :    NFNonCopyable.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-11
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"

class NFNonCopyable
{
protected:
	NFNonCopyable()
	{
	}

	~NFNonCopyable()
	{
	}

private:
	NFNonCopyable(const NFNonCopyable&);
	const NFNonCopyable& operator=(const NFNonCopyable&);
};

