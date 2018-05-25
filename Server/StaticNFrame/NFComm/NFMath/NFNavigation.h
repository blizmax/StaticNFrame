// -------------------------------------------------------------------------
//    @FileName         :    NFNavigation.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFMath
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFMath.h"
#include "NFNavigationHandle.h"
#include <map>

/*
*/
typedef std::map<std::string, NFNavigationHandle*> mapNavigate;
typedef mapNavigate::iterator itmapNavigate;

class NFNavigation : public NFSingleton<NFNavigation>
{
public:
	NFNavigation();
	virtual ~NFNavigation();

	void Finalise();

	NFNavigationHandle* LoadNavigation(const std::string& name);

	bool HasNavigation(const std::string& name);

	bool RemoveNavigation(const std::string& name);

	NFNavigationHandle* FindNavigation(const std::string& name);

private:
	mapNavigate m_mapNavhandles;
};

