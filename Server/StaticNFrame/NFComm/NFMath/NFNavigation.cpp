
#include "NFNavigation.h"
#include "NFNavMeshHandle.h"


//-------------------------------------------------------------------------------------
NFNavigation::NFNavigation()
{
}

//-------------------------------------------------------------------------------------
NFNavigation::~NFNavigation() {
	Finalise();
}

//-------------------------------------------------------------------------------------
void NFNavigation::Finalise()
{
	for (itmapNavigate it = m_mapNavhandles.begin(); it != m_mapNavhandles.end(); ++it) {
		NFNavigationHandle* p = it->second;
		delete p;
	}
}

//-------------------------------------------------------------------------------------
bool NFNavigation::RemoveNavigation(const std::string& name)
{
	itmapNavigate iter = m_mapNavhandles.find(name);
	if (iter != m_mapNavhandles.end()) {
		m_mapNavhandles.erase(iter);
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------
NFNavigationHandle* NFNavigation::FindNavigation(const std::string& name)
{
	itmapNavigate iter = m_mapNavhandles.find(name);
	if (iter != m_mapNavhandles.end())
	{
		//if (iter->second->type() == NFNavigationHandle::NAV_MESH) {
		//	return iter->second;
		//}
		// 		else if (iter->second->type() == NavigationHandle::NAV_TILE)
		// 		{
		// 			// ����tile��Ҫ����ײ�� ÿһ��space����Ҫһ���µ����ݣ� ����������ÿ����ķ�ʽ�����ӹ����ٶ�
		// 			NavTileHandle* pNavTileHandle = new NavTileHandle(*(KBEngine::NavTileHandle*)iter->second.get());
		// // 			DEBUG_MSG(fmt::format("Navigation::findNavigation: copy NavTileHandle({:p})!\n", (void*)pNavTileHandle));
		// // 			return NavigationHandlePtr(pNavTileHandle);
		// 		}

		return iter->second;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------
bool NFNavigation::HasNavigation(const std::string& name)
{
	return m_mapNavhandles.find(name) != m_mapNavhandles.end();
}

//-------------------------------------------------------------------------------------
NFNavigationHandle* NFNavigation::LoadNavigation(const std::string& name)
{
	if (name.empty())
		return NULL;

	itmapNavigate iter = m_mapNavhandles.find(name);
	if (iter != m_mapNavhandles.end())
	{
		return iter->second;
	}

	NFNavigationHandle* pNavigationHandle_ = new NFNavMeshHandle;

	if (pNavigationHandle_->Create(name)) {
		m_mapNavhandles[name] = pNavigationHandle_;
		return pNavigationHandle_;
	}
	else
	{
		delete pNavigationHandle_;
		pNavigationHandle_ = NULL;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------		
