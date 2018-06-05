// -------------------------------------------------------------------------
//    @FileName         :    NFNavMeshHandle.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFMath
//
// -------------------------------------------------------------------------
#include "NFNavMeshHandle.h"
#include "NFNavigation.h"
#include "NFMath.h"

static float frand()
{
	//	return ((float)(rand() & 0xffff)/(float)0xffff);
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

//-------------------------------------------------------------------------------------
NFNavMeshHandle::NFNavMeshHandle() :
	NFNavigationHandle()
{
}

//-------------------------------------------------------------------------------------
NFNavMeshHandle::~NFNavMeshHandle()
{
	std::vector<dtNavMesh*>::iterator iter = navmesh_layers.begin();
	for (; iter != navmesh_layers.end(); ++iter)
		dtFreeNavMesh((*iter));

	std::vector<dtNavMeshQuery*>::iterator iter1 = navmeshQuery_layers.begin();
	for (; iter1 != navmeshQuery_layers.end(); ++iter1)
		dtFreeNavMeshQuery((*iter1));

	//DEBUG_MSG(fmt::format("NavMeshHandle::~NavMeshHandle(): ({}) is destroyed!\n", name));
}

//-------------------------------------------------------------------------------------
int NFNavMeshHandle::FindStraightPath(int layer, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		//ERROR_MSG(fmt::format("NavMeshHandle::findStraightPath: not found layer({})\n",  layer));
		return NAV_ERROR;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];
	// dtNavMesh*
	float spos[3];
	spos[0] = start.x;
	spos[1] = start.y;
	spos[2] = start.z;

	float epos[3];
	epos[0] = end.x;
	epos[1] = end.y;
	epos[2] = end.z;

	dtQueryFilter filter;
	filter.setIncludeFlags(0xffff);
	filter.setExcludeFlags(0);

	const float extents[3] = { 2.f, 4.f, 2.f };

	dtPolyRef startRef = INVALID_NAVMESH_POLYREF;
	dtPolyRef endRef = INVALID_NAVMESH_POLYREF;

	float startNearestPt[3];
	float endNearestPt[3];
	navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, startNearestPt);
	navmeshQuery->findNearestPoly(epos, extents, &filter, &endRef, endNearestPt);

	if (!startRef || !endRef)
	{
		//ERROR_MSG(fmt::format("NavMeshHandle::findStraightPath({2}): Could not find any nearby poly's ({0}, {1})\n", startRef, endRef, name));
		return NAV_ERROR_NEARESTPOLY;
	}

	dtPolyRef polys[MAX_POLYS];
	int npolys;
	float straightPath[MAX_POLYS * 3];
	unsigned char straightPathFlags[MAX_POLYS];
	dtPolyRef straightPathPolys[MAX_POLYS];
	int nstraightPath;
	int pos = 0;

	navmeshQuery->findPath(startRef, endRef, startNearestPt, endNearestPt, &filter, polys, &npolys, MAX_POLYS);
	nstraightPath = 0;

	if (npolys)
	{
		float epos1[3];
		dtVcopy(epos1, endNearestPt);

		if (polys[npolys - 1] != endRef)
			navmeshQuery->closestPointOnPoly(polys[npolys - 1], endNearestPt, epos1);

		navmeshQuery->findStraightPath(startNearestPt, endNearestPt, polys, npolys, straightPath, straightPathFlags, straightPathPolys, &nstraightPath, MAX_POLYS);

		NFVector3 currpos;
		for (int i = 0; i < nstraightPath * 3;)
		{
			currpos.x = straightPath[i++];
			currpos.y = straightPath[i++];
			currpos.z = straightPath[i++];
			paths.push_back(currpos);
			pos++;

			//DEBUG_MSG(fmt::format("NavMeshHandle::findStraightPath: {}->{}, {}, {}\n", pos, currpos.x, currpos.y, currpos.z));
		}
	}
	return pos;
}

//-------------------------------------------------------------------------------------
int NFNavMeshHandle::RayCast(int layer, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return NAV_ERROR;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];
	if (navmeshQuery == nullptr) return NAV_ERROR;

	float hitPoint[3];
	float spos[3];
	spos[0] = start.x;
	spos[1] = start.y;
	spos[2] = start.z;

	float epos[3];
	epos[0] = end.x;
	epos[1] = end.y;
	epos[2] = end.z;

	dtQueryFilter filter;
	filter.setIncludeFlags(0xffff);
	filter.setExcludeFlags(0);

	const float extents[3] = { 2.f, 4.f, 2.f };

	dtPolyRef startRef = INVALID_NAVMESH_POLYREF;

	float nearestPt[3];
	navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, nearestPt);

	if (!startRef)
	{
		return NAV_ERROR_NEARESTPOLY;
	}

	float t = 0;
	float hitNormal[3];
	memset(hitNormal, 0, sizeof(hitNormal));

	dtPolyRef polys[MAX_POLYS];
	int npolys;

	navmeshQuery->raycast(startRef, spos, epos, &filter, &t, hitNormal, polys, &npolys, MAX_POLYS);

	if (t > 1)
	{
		// no hit
		return NAV_ERROR;
	}
	else
	{
		// Hit
		hitPoint[0] = spos[0] + (epos[0] - spos[0]) * t;
		hitPoint[1] = spos[1] + (epos[1] - spos[1]) * t;
		hitPoint[2] = spos[2] + (epos[2] - spos[2]) * t;
		if (npolys)
		{
			float h = 0;
			navmeshQuery->getPolyHeight(polys[npolys - 1], hitPoint, &h);
			hitPoint[1] = h;
		}
	}

	hitPointVec.push_back(NFVector3(hitPoint[0], hitPoint[1], hitPoint[2]));
	return 1;
}

//-------------------------------------------------------------------------------------
bool NFNavMeshHandle::Create(const std::string& path)
{
	if (path.empty())
		return false;
	//NavMeshHandle* pNavMeshHandle = NULL;
	//pNavMeshHandle = new NavMeshHandle();

	FILE* fp = fopen(path.c_str(), "rb");
	if (!fp)
	{
		// 			ERROR_MSG(fmt::format("NavMeshHandle::create: open({}) is error!\n",
		// 				Resmgr::getSingleton().matchRes(path)));
		return false;
	}

	bool safeStorage = true;
	int pos = 0;
	int size = sizeof(NavMeshSetHeader);

	fseek(fp, 0, SEEK_END);
	size_t flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pBuf = new char[flen];

	size_t readsize = fread(pBuf, 1, flen, fp);
	if (readsize != flen)
	{
		// 			ERROR_MSG(fmt::format("NavMeshHandle::create: open({}), read(size={} != {}) error!\n",
		// 				Resmgr::getSingleton().matchRes(path), readsize, flen));

		fclose(fp);
		delete[] pBuf;
		pBuf = nullptr;
		return false;
	}

	if (readsize < sizeof(NavMeshSetHeader))
	{
		// 			ERROR_MSG(fmt::format("NavMeshHandle::create: open({}), NavMeshSetHeader is error!\n",
		// 				Resmgr::getSingleton().matchRes(path)));

		fclose(fp);
		delete[] pBuf;
		pBuf = nullptr;
		return false;
	}

	NavMeshSetHeader header;
	memcpy(&header, pBuf, size);
	pos += size;
	if (header.version != NFNavMeshHandle::RCN_NAVMESH_VERSION)
	{
		// 			ERROR_MSG(fmt::format("NavMeshHandle::create: navmesh version({}) is not match({})!\n",
		// 				header.version, ((int)NavMeshHandle::RCN_NAVMESH_VERSION)));

		fclose(fp);
		delete[] pBuf;
		pBuf = nullptr;
		return false;
	}

	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		//ERROR_MSG("NavMeshHandle::create: dtAllocNavMesh is failed!\n");
		fclose(fp);
		delete[] pBuf;
		pBuf = nullptr;
		return false;
	}
	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		//ERROR_MSG(fmt::format("NavMeshHandle::create: mesh init is error({})!\n", status));
		fclose(fp);
		delete[] pBuf;
		pBuf = nullptr;
		return false;
	}

	// Read tiles.
	bool success = true;
	for (int i = 0; i < header.tileCount; ++i)
	{
		NavMeshTileHeader tileHeader;
		size = sizeof(NavMeshTileHeader);
		memcpy(&tileHeader, &pBuf[pos], size);
		pos += size;

		size = tileHeader.dataSize;
		if (!tileHeader.tileRef || !tileHeader.dataSize)
		{
			success = false;
			status = DT_FAILURE + DT_INVALID_PARAM;
			break;
		}

		unsigned char* tileData =
			static_cast<unsigned char*>(dtAlloc(size, DT_ALLOC_PERM));
		if (!tileData)
		{
			success = false;
			status = DT_FAILURE + DT_OUT_OF_MEMORY;
			break;
		}
		memcpy(tileData, &pBuf[pos], size);
		pos += size;

		status = mesh->addTile(tileData
			, size
			, (safeStorage ? DT_TILE_FREE_DATA : 0) //-V547
			, tileHeader.tileRef
			, nullptr);

		if (dtStatusFailed(status))
		{
			success = false;
			break;
		}
	}

	fclose(fp);
	delete[] pBuf;
	pBuf = nullptr;

	if (!success)
	{
		//ERROR_MSG(fmt::format("NavMeshHandle::create:  error({})!\n", status));
		dtFreeNavMesh(mesh);
		return false;
	}

	navmesh_layers.push_back(mesh);
	dtNavMeshQuery* pMavmeshQuery = new dtNavMeshQuery();
	navmeshQuery_layers.push_back(pMavmeshQuery);
	pMavmeshQuery->init(mesh, 1024);
	name = path;
	unsigned int tileCount = 0;
	unsigned int nodeCount = 0;
	unsigned int polyCount = 0;
	unsigned int vertCount = 0;
	unsigned int triCount = 0;
	unsigned int triVertCount = 0;
	unsigned int dataSize = 0;

	const dtNavMesh* navmesh = mesh;
	for (int i = 0; i < navmesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = navmesh->getTile(i);
		if (!tile || !tile->header)
			continue;

		tileCount++;
		nodeCount += tile->header->bvNodeCount;
		polyCount += tile->header->polyCount;
		vertCount += tile->header->vertCount;
		triCount += tile->header->detailTriCount;
		triVertCount += tile->header->detailVertCount;
		dataSize += tile->dataSize;
	}
	return true;
}

//-------------------------------------------------------------------------------------

int NFNavMeshHandle::FindRandomPointAroundCircle(int layer, const NFVector3& centerPos,
	std::vector<NFVector3>& points, int max_points, float maxRadius)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return NAV_ERROR;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];
	dtQueryFilter filter;
	filter.setIncludeFlags(0xffff);
	filter.setExcludeFlags(0);
	if (maxRadius <= 0.0001f)
	{
		NFVector3 currpos;

		for (int i = 0; i < max_points; i++)
		{
			float pt[3];
			dtPolyRef ref;
			dtStatus status = navmeshQuery->findRandomPoint(&filter, frand, &ref, pt);
			if (dtStatusSucceed(status))
			{
				currpos.x = pt[0];
				currpos.y = pt[1];
				currpos.z = pt[2];

				points.push_back(currpos);
			}
		}

		return static_cast<int>(points.size());
	}

	const float extents[3] = { 2.f, 4.f, 2.f };

	dtPolyRef startRef = INVALID_NAVMESH_POLYREF;

	float spos[3];
	spos[0] = centerPos.x;
	spos[1] = centerPos.y;
	spos[2] = centerPos.z;

	float startNearestPt[3];
	navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, startNearestPt);

	if (!startRef)
	{
		return NAV_ERROR_NEARESTPOLY;
	}

	NFVector3 currpos;

	for (int i = 0; i < max_points; i++)
	{
		float pt[3];
		dtPolyRef ref;
		dtStatus status = navmeshQuery->findRandomPointAroundCircle(startRef, spos, maxRadius, &filter, frand, &ref, pt);
		if (dtStatusSucceed(status))
		{
			currpos.x = pt[0];
			currpos.y = pt[1];
			currpos.z = pt[2];

			points.push_back(currpos);
		}
	}
	return static_cast<int>(points.size());
}

bool NFNavMeshHandle::IsValidPos(int layer, const NFVector3& pos)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return NAV_ERROR;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];

	dtPolyRef startRef = INVALID_NAVMESH_POLYREF;

	dtQueryFilter filter;
	filter.setIncludeFlags(0xffff);
	filter.setExcludeFlags(0);

	const float extents[3] = { 2.f, 100.f, 2.f };

	float spos[3];
	spos[0] = pos.x;
	spos[1] = pos.y;
	spos[2] = pos.z;

	float startNearestPt[3];
	navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, startNearestPt);

	return floatEqual(pos.x, startNearestPt[0]) && floatEqual(pos.z, startNearestPt[2]);
}

void NFNavMeshHandle::GetValidPos(int layer, const NFVector3& pos, NFVector3& out)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];

	dtPolyRef startRef = INVALID_NAVMESH_POLYREF;

	dtQueryFilter filter;
	filter.setIncludeFlags(0xffff);
	filter.setExcludeFlags(0);

	const float extents[3] = { 2.f, 100.f, 2.f };

	float spos[3];
	spos[0] = pos.x;
	spos[1] = pos.y;
	spos[2] = pos.z;

	float startNearestPt[3];
	navmeshQuery->findNearestPoly(spos, extents, &filter, &startRef, startNearestPt);

	out.x = startNearestPt[0];
	out.y = startNearestPt[1];
	out.z = startNearestPt[2];
}

int NFNavMeshHandle::GetTitelWidth(int layer)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return NAV_ERROR;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];

	return static_cast<int>(navmeshQuery->getAttachedNavMesh()->GetTileWidth()) * navmeshQuery->getAttachedNavMesh()->getMaxTiles();
}

int NFNavMeshHandle::GetTitelHeight(int layer)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return NAV_ERROR;
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];

	return static_cast<int>(navmeshQuery->getAttachedNavMesh()->GetTileHeight()) * navmeshQuery->getAttachedNavMesh()->getMaxTiles();
}

NFVector3 NFNavMeshHandle::GetOriginPos(int layer)
{
	if (layer >= static_cast<int>(navmeshQuery_layers.size()))
	{
		return NFVector3();
	}
	dtNavMeshQuery* navmeshQuery = navmeshQuery_layers[layer];

	const float* fPos = navmeshQuery->getAttachedNavMesh()->GetOriginPos();

	return NFVector3(fPos[0], fPos[1], fPos[2]);
}