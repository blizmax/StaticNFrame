#pragma once

#include "NFMath.h"
#include "NFNavigationHandle.h"

#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include <string>
#include <vector>

struct NavMeshSetHeader
{
	int version;
	int tileCount;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

class NFNavMeshHandle : public NFNavigationHandle
{
public:
	static const int MAX_POLYS = 256;
	static const int NAV_ERROR_NEARESTPOLY = -2;

	static const long RCN_NAVMESH_VERSION = 1;
	static const int INVALID_NAVMESH_POLYREF = 0;
public:
	NFNavMeshHandle();
	virtual ~NFNavMeshHandle();

	virtual int FindStraightPath(int layer, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths) override;
	virtual int RayCast(int layer, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec) override;

	virtual NFNavigationHandle::NAV_TYPE Type() const
	{
		return NAV_MESH;
	}

	virtual bool Create(const std::string& name) override;

	virtual int FindRandomPointAroundCircle(int layer, const NFVector3& centerPos,
	                                        std::vector<NFVector3>& points, int max_points, float maxRadius) override;

	bool IsValidPos(int layer, const NFVector3& pos);
	void GetValidPos(int layer, const NFVector3& pos, NFVector3& out);

	int GetTitelWidth(int layer);
	int GetTitelHeight(int layer);
	NFVector3 GetOriginPos(int layer);

	std::vector<dtNavMesh*> navmesh_layers;
	std::vector<dtNavMeshQuery*> navmeshQuery_layers;
};

