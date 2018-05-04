
#pragma once
#include <stdint.h>
#include <list>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "NFComm/NFCore/NFPlatform.h"

using namespace std;

//�¼�����������
#define EVENT_FIRE_MAX_LAYER 20

//�¼�����������(�����¼�Ƕ�ײ���̫�������ѭ��) 
#define EVENT_REF_MAX_CNT 5

//key����
struct SKey
{
public:
	SKey()
	{
		nSrcID = 0;
		nEventID = 0;
		bySrcType = 0;
	}
	uint64_t nSrcID;		//ԴID(�����UID)
	uint16_t nEventID;		//�¼�ID
	uint8_t  bySrcType;		//Դ���ͣ���ͬ���������ͣ�
};


//�¼�key
struct SEventKey
{
public:
	SKey eKey;
	//uint64_t nKey;
public:
	SEventKey()
	{
		//nKey = 0;
	}
	bool operator == (const SEventKey &eventKey) const 
	{
		return ((eKey.nSrcID == eventKey.eKey.nSrcID) &&
			(eKey.nEventID == eventKey.eKey.nEventID) &&
			(eKey.bySrcType == eventKey.eKey.bySrcType));
		//return nKey == eventKey.nKey;
	}

	bool operator < (const SEventKey &eventKey) const
	{
		if (eKey.nSrcID < eventKey.eKey.nSrcID)
		{
			return true;
		}
		else if (eKey.nSrcID > eventKey.eKey.nSrcID)
		{
			return false;
		}
		else
		{
			if (eKey.nEventID < eventKey.eKey.nEventID)
			{
				return true;
			}
			else if (eKey.nEventID > eventKey.eKey.nEventID)
			{
				return false;
			}
			else
			{
				if (eKey.bySrcType < eventKey.eKey.bySrcType)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		//return nKey < eventKey.nKey;
	}
	
}; 

class NFEventContext;

namespace std {
	template<> struct hash < SEventKey >
	{
		size_t  operator()(const SEventKey &eventKey) const
		{
			return ((hash<uint64_t>()(eventKey.eKey.nSrcID)) ^ (hash<uint16_t>()(eventKey.eKey.nEventID) << 1)) ^ (hash<uint8_t>()(eventKey.eKey.bySrcType) << 1);
			//return hash<uint64_t>()(eventKey.nKey);
		}
	};
}

//�¼�ģ��
template<class TEventSink,class TEventObj>
class EventTemplate
{
private:
	//������Ϣ
	struct SubscribeInfo 
	{
		TEventSink *pSink;		//
		std::string szDesc;		//��������
		int32_t nRefCount;		//���ô���
		bool bRemoveFlag;		//�Ƿ���Ҫ�Ƴ���־
		SubscribeInfo(TEventSink *pParamSink, const std::string& desc):szDesc(desc)
		{
			pSink = pParamSink;
			//szDesc = desc;
			nRefCount = 0;
			bRemoveFlag = false;
		}
		void Add()
		{
			nRefCount++;
		}
		void Sub()
		{
			--nRefCount;
		}
	};
	
	//������Ϣ�б�
	typedef std::list<SubscribeInfo> TLIST_SUBCRIBLE;
	//���ж�����Ϣ
	typedef std::unordered_map<SEventKey, TLIST_SUBCRIBLE*> TMAP_ALL_SUBSCRIBEOBJ;

	//����key�б�
	typedef std::set<SEventKey> TSET_SUBSCRIBEKEY;

	//���ж���key
	typedef std::unordered_map<void*, TSET_SUBSCRIBEKEY*> TMAP_ALL_SUBSCRIBEKEY;

public:

	EventTemplate()
	{
		m_nFireLayer = 0;
	}

	virtual ~EventTemplate()
	{
		typename TMAP_ALL_SUBSCRIBEKEY::iterator iterA = m_mapAllSubscribeKey.begin();
		typename TMAP_ALL_SUBSCRIBEOBJ::iterator iterB = m_mapAllSubscribeObj.begin();

		for (; iterA != m_mapAllSubscribeKey.end(); ++iterA)
		{
			TSET_SUBSCRIBEKEY *pSetSubcribeKey = iterA->second;
			pSetSubcribeKey->clear();
			NFSafeDelete(pSetSubcribeKey);
		}
		m_mapAllSubscribeKey.clear();
		
		for (; iterB != m_mapAllSubscribeObj.end(); ++iterB)
		{
			TLIST_SUBCRIBLE *pLstSubcribe = iterB->second;
			pLstSubcribe->clear();
			NFSafeDelete(pLstSubcribe);
		}
		m_mapAllSubscribeObj.clear();
	}

	//�����¼�
	bool Subscribe(TEventSink *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc)
	{
		if (nullptr == pSink) return false;

		SEventKey skey;
		skey.eKey.nEventID = nEventID;
		skey.eKey.nSrcID = nSrcID;
		skey.eKey.bySrcType = bySrcType;
		//skey.nKey = EVENT_KEY(nSrcID, nEventID, bySrcType);

		TMAP_ALL_SUBSCRIBEKEY::iterator iter = m_mapAllSubscribeKey.find(pSink);
		if (iter == m_mapAllSubscribeKey.end())
		{
			TSET_SUBSCRIBEKEY *psetKey = new TSET_SUBSCRIBEKEY();
			psetKey->insert(skey);
			m_mapAllSubscribeKey.emplace(pSink, psetKey);
			//m_mapAllSubscribeKey.insert(TMAP_ALL_SUBSCRIBEKEY::value_type(pSink, psetKey));
		}
		else
		{
			TSET_SUBSCRIBEKEY *pAllSubscribeKey = iter->second;
			TSET_SUBSCRIBEKEY::iterator iterSet = pAllSubscribeKey->find(skey);
			if (iterSet != pAllSubscribeKey->end())
			{
				return true;
			}
			pAllSubscribeKey->insert(skey);
		}

		//������Ϣ
		SubscribeInfo info(pSink, desc);
		typename TMAP_ALL_SUBSCRIBEOBJ::iterator iterObj = m_mapAllSubscribeObj.find(skey);
		if (iterObj != m_mapAllSubscribeObj.end())
		{
			TLIST_SUBCRIBLE *pListSubscribe = iterObj->second;
			pListSubscribe->push_back(info);
		}
		else
		{
			TLIST_SUBCRIBLE *pListSubscribe = new TLIST_SUBCRIBLE();
			pListSubscribe->push_back(info);
			m_mapAllSubscribeObj.emplace(skey, pListSubscribe);
			//m_mapAllSubscribeObj.insert(typename TMAP_ALL_SUBSCRIBEOBJ::value_type(skey, pListSubscribe));
		}
		return true;
	}

	//ȡ�������¼�
	bool UnSubscribe(TEventSink *pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType)
	{
		if (nullptr == pSink)
		{
			return false;
		}

		SEventKey skey;
		skey.eKey.nEventID = nEventID;
		skey.eKey.nSrcID = nSrcID;
		skey.eKey.bySrcType = bySrcType;
		//skey.nKey = EVENT_KEY(nSrcID, nEventID, bySrcType);

		TMAP_ALL_SUBSCRIBEKEY::iterator iter = m_mapAllSubscribeKey.find(pSink);
		if (iter == m_mapAllSubscribeKey.end())
		{
			return false;
		}
		TSET_SUBSCRIBEKEY *pSetSubscribeKey = iter->second;
		if (pSetSubscribeKey->find(skey) == pSetSubscribeKey->end())
		{
			return false;
		}
		pSetSubscribeKey->erase(skey);
		if (0 == pSetSubscribeKey->size())
		{
			m_mapAllSubscribeKey.erase(iter);
			NFSafeDelete(pSetSubscribeKey);
		}

		//��������Ϣ
		DelSubcribeInfo(pSink, skey);

		return true;
	}

	//ȡ�����ж���
	bool UnSubscribeAll(TEventSink *pSink)
	{
		if (nullptr == pSink)
		{
			return false;
		}
		TMAP_ALL_SUBSCRIBEKEY::iterator iter = m_mapAllSubscribeKey.find(pSink);
		if (iter == m_mapAllSubscribeKey.end())
		{
			return false;
		}
		TSET_SUBSCRIBEKEY *pSubscribeKey = iter->second;
		TSET_SUBSCRIBEKEY::iterator iterSet = pSubscribeKey->begin();
		for (; iterSet != pSubscribeKey->end(); ++iterSet)
		{
			DelSubcribeInfo(pSink, (*iterSet));
		}
		m_mapAllSubscribeKey.erase(iter);
		NFSafeDelete(pSubscribeKey);
		pSubscribeKey = nullptr;

		return true;
	}

	bool Fire(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
	{
		SEventKey skey;
		skey.eKey.nEventID = nEventID;
		skey.eKey.nSrcID = nSrcID;
		skey.eKey.bySrcType = bySrcType;
		//skey.nKey = EVENT_KEY(nSrcID, nEventID, bySrcType);
		if (skey.eKey.nSrcID != 0)
		{
			bool bRes = Fire(skey, nEventID, nSrcID, bySrcType, pEventContext);
			if (!bRes)
			{
				return false;
			}
		}
		skey.eKey.nSrcID = 0;
		//skey.nKey = EVENT_KEY(skey.eKey.nSrcID, nEventID, bySrcType);
		bool bRes = Fire(skey, nEventID, nSrcID, bySrcType, pEventContext);
		if (!bRes)
		{
			return false;
		}
		return true;
	}


private:
	//ɾ��������Ϣ
	bool DelSubcribeInfo(TEventSink *pSink, const SEventKey &skey)
	{
		typename TMAP_ALL_SUBSCRIBEOBJ::iterator iter = m_mapAllSubscribeObj.find(skey);
		if (iter != m_mapAllSubscribeObj.end())
		{
			TLIST_SUBCRIBLE *pListSubscribe = iter->second;
			typename TLIST_SUBCRIBLE::iterator iterLst = pListSubscribe->begin();
			for (; iterLst != pListSubscribe->end(); ++iterLst)
			{
				SubscribeInfo &sInfo = (*iterLst);
				if (sInfo.pSink == pSink)
				{
					if (sInfo.nRefCount == 0)
					{
						pListSubscribe->erase(iterLst);
					}
					else
					{
						sInfo.bRemoveFlag = true;
					}
					break;
				}
			}
			if (0 == pListSubscribe->size())
			{
				m_mapAllSubscribeObj.erase(iter);
				NFSafeDelete(pListSubscribe);
			}
		}

		return true;
	}

	bool Fire(const SEventKey &skey, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContex)
	{
		m_nFireLayer++;
		if (m_nFireLayer >= EVENT_FIRE_MAX_LAYER)
		{
			//���ش�����ѭ������....������Ҫ��ӡ������־
			std::cerr << "[Event] m_nFireLayer >= EVENT_FIRE_MAX_LAYER.....nEventID:" <<
				nEventID << ",nSrcID:" << nSrcID << ",bySrcType:" << bySrcType << ", firelayer:" << m_nFireLayer;
			m_nFireLayer--;
			return false;
		}

		typename TMAP_ALL_SUBSCRIBEOBJ::iterator iterLst = m_mapAllSubscribeObj.find(skey);
		if (iterLst != m_mapAllSubscribeObj.end())
		{
			TLIST_SUBCRIBLE *pLstSubscribe = iterLst->second;
			typename TLIST_SUBCRIBLE::iterator iter = pLstSubscribe->begin();
			for (; iter != pLstSubscribe->end();)
			{
				SubscribeInfo *pSubscribeInfo = &(*iter);
				if (pSubscribeInfo->nRefCount >= EVENT_REF_MAX_CNT)
				{
					//�������⣬ͬһ�¼���Ƕ�׵���̫���,������Ҫ��ӡ������־
					std::cerr << "[Event] pSubscribeInfo->nRefCount >= EVENT_REF_MAX_CNT....eventid:" << nEventID << ",srcid:" << nSrcID << ",type:" << bySrcType << ",refcount:" << pSubscribeInfo->nRefCount << ",removeflag:" << (int32_t)pSubscribeInfo->bRemoveFlag << ",szdesc:" << pSubscribeInfo->szDesc;
					m_nFireLayer--;
					return false;
				}
				if (!pSubscribeInfo->bRemoveFlag)
				{
					bool bRes = false;
					try
					{
						pSubscribeInfo->Add();
						bRes = m_FireEventObj(pSubscribeInfo->pSink, nEventID, nSrcID, bySrcType, pEventContex);
						pSubscribeInfo->Sub();
					}
					catch (...)
					{
						//�쳣���¼������쳣,��Ҫ��ӡ�쳣��־
						std::cerr << "[Event] pSubscribeInfo->nRefCount >= EVENT_REF_MAX_CNT....eventid:" << nEventID << ",srcid:" << nSrcID << ",type:" << bySrcType << ",refcount:" << pSubscribeInfo->nRefCount << ",removeflag:" << (int32_t)pSubscribeInfo->bRemoveFlag << ",szdesc:" << pSubscribeInfo->szDesc;
						m_nFireLayer--;
						return false;
					}
					if (pSubscribeInfo->bRemoveFlag && 0 == pSubscribeInfo->nRefCount)
					{
						iter = pLstSubscribe->erase(iter);
					}
					else
					{
						++iter;
					}
					if (!bRes)
					{
						m_nFireLayer--;
						return false;
					}
				} // end of if (!subInfo.bRemoveFlag)
				else
				{
					if (0 == pSubscribeInfo->nRefCount)
					{
						iter = pLstSubscribe->erase(iter);
					}
					else
					{
						++iter;
					}
				}
			} // end of for (; iter != pLstSubscribe->end();)

			if (0 == pLstSubscribe->size())
			{
				m_mapAllSubscribeObj.erase(iterLst);
				NFSafeDelete(pLstSubscribe);
			}

		} // enf of if (iterLst != m_mapAllSubscribeObj.end())

		m_nFireLayer--;

		return true;
	}

private:
	//�����¼�����
	TEventObj m_FireEventObj;
	//�¼�����map
	TMAP_ALL_SUBSCRIBEOBJ m_mapAllSubscribeObj;
	//�����¼�key map
	TMAP_ALL_SUBSCRIBEKEY m_mapAllSubscribeKey;
	//���Ͳ���
	int32_t m_nFireLayer;
};
