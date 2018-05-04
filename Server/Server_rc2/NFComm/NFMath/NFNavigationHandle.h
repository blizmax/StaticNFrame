#pragma once

#include "NFMath.h"

#include <vector>


class NFNavigationHandle
{
public:
	static const int NAV_ERROR = -1;

	enum NAV_TYPE
	{
		NAV_UNKNOWN = 0,
		NAV_MESH = 1,
		NAV_TILE = 2
	};

	enum NAV_OBJECT_STATE
	{
		NAV_OBJECT_STATE_MOVING = 1,	// 移动中
		NAV_OBJECT_STATE_MOVEOVER = 2,	// 移动已经结束了
	};
public:
	NFNavigationHandle():name()
	{
	}

	virtual ~NFNavigationHandle()
	{
	}
public:
	virtual NFNavigationHandle::NAV_TYPE type() const{ return NAV_UNKNOWN; }

	virtual int FindStraightPath(int layer, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths) = 0;
	virtual int RayCast(int layer, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec) = 0;

	virtual bool Create(const std::string& name) { return false; };

	virtual int FindRandomPointAroundCircle(int layer, const NFVector3& centerPos,
		std::vector<NFVector3>& points, int max_points, float maxRadius) = 0;
protected:
	std::string name;
};

