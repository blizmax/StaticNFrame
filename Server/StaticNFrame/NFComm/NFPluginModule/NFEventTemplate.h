// -------------------------------------------------------------------------
//    @FileName         :    NFEventTemplate.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once
#include <stdint.h>
#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFHash.hpp"
#include "google/protobuf/message.h"

/**
 *@brief  �������¼����Ƕ�ײ���֧��20��.
 * ������Fireһ���¼����Fire�˱���¼�������Ƕ��
 */
#define EVENT_FIRE_MAX_LAYER 20

/**
*@brief  �Ե�һ�¼����Ƕ�ײ���֧��5��.
* ������Fireһ���¼���ֶ�����¼�������Fire
*/
#define EVENT_REF_MAX_CNT 5

/**
*@brief �¼�key��
*/
struct SEventKey
{
	/**
	*@brief �¼���Ҫ��key����Ҫָ��ң�����Ψһid
	*/
	uint64_t nSrcID;

	/**
	*@brief �¼�Id
	*/
	uint16_t nEventID;

	/**
	*@brief src����, ����������ң����������
	*/
	uint8_t bySrcType;

	/**
	*@brief ���캯��
	*/
	SEventKey()
	{
		nSrcID = 0;
		nEventID = 0;
		bySrcType = 0;
	}

	/**
	*@brief �ж��Ƿ����
	*/
	bool operator ==(const SEventKey& eventKey) const
	{
		return ((nSrcID == eventKey.nSrcID) &&
			(nEventID == eventKey.nEventID) &&
			(bySrcType == eventKey.bySrcType));
	}

	/**
	*@brief �ж��Ƿ�С��, ��֪����û�и��õ��ж�С�ڵķ���
	*/
	bool operator <(const SEventKey& eventKey) const
	{
		if (nSrcID < eventKey.nSrcID)
		{
			return true;
		}
		else if (nSrcID > eventKey.nSrcID)
		{
			return false;
		}
		else
		{
			if (nEventID < eventKey.nEventID)
			{
				return true;
			}
			else if (nEventID > eventKey.nEventID)
			{
				return false;
			}
			else
			{
				if (bySrcType < eventKey.bySrcType)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
};

/**
*@brief ��hashֵ
*/
namespace std
{
	template <>
	struct hash<SEventKey>
	{
		size_t operator()(const SEventKey& eventKey) const
		{
			return NFHash::hash_combine(eventKey.nSrcID, eventKey.nEventID, eventKey.bySrcType);
		}
	};
}

/**
 *@brief �¼�ϵͳģ����
 */
template <class TEventSink, class TEventObj>
class NFEventTemplate
{
private:
	/**
	 *@brief �¼�������Ϣ
	 */
	struct SubscribeInfo
	{
		/**
		*@brief �¼�����
		*/
		TEventSink* pSink;

		/**
		*@brief ������Ϣ
		*/
		std::string szDesc;

		/**
		*@brief ���ô���
		*/
		int32_t nRefCount;

		/**
		*@brief �Ƴ���־
		*/
		bool bRemoveFlag;

		/**
		*@brief ���캯��
		*/
		SubscribeInfo(TEventSink* pParamSink, const std::string& desc) : szDesc(desc)
		{
			pSink = pParamSink;
			szDesc = desc;
			nRefCount = 0;
			bRemoveFlag = false;
		}

		/**
		*@brief ��������
		*/
		void Add()
		{
			nRefCount++;
		}

		/**
		*@brief ��������
		*/
		void Sub()
		{
			--nRefCount;
		}
	};

public:
	/**
	*@brief ���캯��
	*/
	NFEventTemplate()
	{
		m_nFireLayer = 0;
	}

	/**
	*@brief ��������
	*/
	virtual ~NFEventTemplate()
	{
		m_mapAllSubscribeKey.clear();
		m_mapAllSubscribeObj.clear();
	}

	/**
	* @brief �����¼�
	*
	* @param pSink		���Ķ���
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @param desc		�¼����������ڴ�ӡ����ȡ��Ϣ���鿴BUG֮���
	* @return			�����¼��Ƿ�ɹ�
	*/
	bool Subscribe(TEventSink* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const std::string& desc)
	{
		if (nullptr == pSink) return false;

		SEventKey skey;
		skey.nEventID = nEventID;
		skey.nSrcID = nSrcID;
		skey.bySrcType = bySrcType;

		/**
		*@brief ���ж�ָ��pSink������û��ע�ᣬȻ���skey����
		*       ���ָ��ĵļ�������skey�Ѿ����ڣ�
		*       ˵���Ѿ����룬ֱ���˳�
		*/
		auto iter = m_mapAllSubscribeKey.find(pSink);
		if (iter == m_mapAllSubscribeKey.end())
		{
			m_mapAllSubscribeKey[pSink].insert(skey);
		}
		else
		{
			auto iterSet = iter->second.find(skey);
			if (iterSet != iter->second.end())
			{
				return true;
			}
			iter->second.insert(skey);
		}

		/**
		*@brief �ж�skey��û�д��ڣ��Ѷ������skey��������
		*/
		SubscribeInfo info(pSink, desc);
		auto iterObj = m_mapAllSubscribeObj.find(skey);
		if (iterObj != m_mapAllSubscribeObj.end())
		{
			iterObj->second.push_front(info);
		}
		else
		{
			m_mapAllSubscribeObj[skey].push_front(info);
		}
		return true;
	}

	/**
	* @brief ȡ�������¼�
	*
	* @param pSink		���Ķ���
	* @param nEventID	�¼�ID
	* @param nSrcID		�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType	�¼�Դ���ͣ�������ͣ���������֮���
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	bool UnSubscribe(TEventSink* pSink, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType)
	{
		if (nullptr == pSink) return false;

		SEventKey skey;
		skey.nEventID = nEventID;
		skey.nSrcID = nSrcID;
		skey.bySrcType = bySrcType;

		/**
		*@brief �ж�pSinkָ�������û�д��ڣ�������ֱ���˳�
		*		���ڵĻ���ɾ����Ӧ��key, ���pSink����Ϊ�յĻ���
		*       ɾ��pSink
		*/
		auto iter = m_mapAllSubscribeKey.find(pSink);
		if (iter == m_mapAllSubscribeKey.end())
		{
			return false;
		}

		if (iter->second.find(skey) == iter->second.end())
		{
			return false;
		}

		iter->second.erase(skey);
		if (iter->second.empty())
		{
			m_mapAllSubscribeKey.erase(iter);
		}

		/**
		*@brief ɾ��skey�������pSink
		*/
		DelSubcribeInfo(pSink, skey);

		return true;
	}

	/**
	* @brief ȡ��pSink���ж����¼�
	*
	* @param pSink		���Ķ���
	* @return			ȡ�������¼��Ƿ�ɹ�
	*/
	bool UnSubscribeAll(TEventSink* pSink)
	{
		if (nullptr == pSink) return false;

		auto iter = m_mapAllSubscribeKey.find(pSink);
		if (iter == m_mapAllSubscribeKey.end())
		{
			return false;
		}

		for (auto iterSet = iter->second.begin(); iterSet != iter->second.end(); ++iterSet)
		{
			DelSubcribeInfo(pSink, (*iterSet));
		}
		m_mapAllSubscribeKey.erase(iter);

		return true;
	}

	/**
	* @brief �����¼�,��ִ���յ��¼��Ķ���Ķ�Ӧ����
	*
	* @param nEventID		�¼�ID
	* @param nSrcID			�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType		�¼�Դ���ͣ�������ͣ���������֮���
	* @param pEventContext	�¼����������
	* @return				ִ���Ƿ�ɹ�
	*/
	/*
	* ������в�����ܵ�������, �����ᵼ�±���, ��������Ԥ��Ĳ�һ��:
	* ����1:��������Fire�¼����ͬ��key��ɾ����ͬ��pSink,
	*		���ܵ��½�Ҫִ�е��¼���ɾ�������������Ԥ�����Ʋ�һ��
	* ����2:��������Fire�¼����ͬ��key��ɾ����ͬ��pSink, �����¼�ϵͳ����SubscribeInfo��Add,Sub���ü�������Ԥ����
	*       ��������������ɾ�������ᵼ��std::list������ʧЧ�� ����ɾ�����ᵼ������
	* ����3:��������Fire�¼�� Fire�˱���¼����ᵼ�µ������⣬�¼�ϵͳ�Ѿ�������Ԥ���� ��ͬ���¼���������5�Σ�
	*       ���е�Fire�¼�������20��
	*/
	bool Fire(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message)
	{
		SEventKey skey;
		skey.nEventID = nEventID;
		skey.nSrcID = nSrcID;
		skey.bySrcType = bySrcType;

		/**
		* @brief ��ִ����ȫƥ���
		*/
		if (skey.nSrcID != 0)
		{
			bool bRes = Fire(skey, nEventID, nSrcID, bySrcType, message);
			if (!bRes)
			{
				return false;
			}
		}

		/**
		* @brief ��ִ�У� ��������¼�nEventID,����ΪbySrcType
		* ���綩��ʱ�������������������¼��������Ƕ�һ����ҵ��¼���
		* ����ʱ��nSrcId=0�����ܵ�������Ҳ����ĸ����¼�
		*/
		skey.nSrcID = 0;
		bool bRes = Fire(skey, nEventID, nSrcID, bySrcType, message);
		if (!bRes)
		{
			return false;
		}
		return true;
	}

private:
	/**
	* @brief ɾ��skey���������pSink
	*
	* @param pSink		���Ķ���
	* @param SEventKey	�¼��ϳ�key
	* @return			ɾ��skey���������pSink�Ƿ�ɹ�
	*/
	bool DelSubcribeInfo(TEventSink* pSink, const SEventKey& skey)
	{
		auto iter = m_mapAllSubscribeObj.find(skey);
		if (iter != m_mapAllSubscribeObj.end())
		{
			for (auto iterLst = iter->second.begin(); iterLst != iter->second.end(); ++iterLst)
			{
				SubscribeInfo& sInfo = (*iterLst);
				if (sInfo.pSink == pSink)
				{
					if (sInfo.nRefCount == 0)
					{
						iter->second.erase(iterLst);
					}
					else
					{
						sInfo.bRemoveFlag = true;
					}
					break;
				}
			}
			if (iter->second.empty())
			{
				m_mapAllSubscribeObj.erase(iter);
			}
		}

		return true;
	}

	/**
	* @brief ִ�����ж����¼�key�ĺ���
	*
	* @param skey			�¼��ϳ�key��skey.nsrcid����Ϊ0������=nEventID
	* @param nEventID		�¼�ID
	* @param nSrcID			�¼�ԴID��һ�㶼����ң�����Ψһid
	* @param bySrcType		�¼�Դ���ͣ�������ͣ���������֮���
	* @param pEventContext	�¼����������
	* @return				ִ���Ƿ�ɹ�
	*/
	bool Fire(const SEventKey& skey, uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message)
	{
		m_nFireLayer++;
		if (m_nFireLayer >= EVENT_FIRE_MAX_LAYER)
		{
			std::cerr << "[Event] m_nFireLayer >= EVENT_FIRE_MAX_LAYER.....nEventID:"
				<< nEventID << ",nSrcID:" << nSrcID << ",bySrcType:"
				<< bySrcType << ", firelayer:" << m_nFireLayer << std::endl;;
			m_nFireLayer--;
			return false;
		}

		auto iterLst = m_mapAllSubscribeObj.find(skey);
		if (iterLst != m_mapAllSubscribeObj.end())
		{
			for (auto iter = iterLst->second.begin(); iter != iterLst->second.end();)
			{
				SubscribeInfo* pSubscribeInfo = &(*iter);
				if (pSubscribeInfo->nRefCount >= EVENT_REF_MAX_CNT)
				{
					std::cerr << "[Event] pSubscribeInfo->nRefCount >= EVENT_REF_MAX_CNT....eventid:"
						<< nEventID << ",srcid:" << nSrcID << ",type:" << bySrcType << ",refcount:"
						<< pSubscribeInfo->nRefCount << ",removeflag:" << static_cast<int32_t>(pSubscribeInfo->bRemoveFlag) << ",szdesc:" << pSubscribeInfo->szDesc << std::endl;
					m_nFireLayer--;
					return false;
				}
				if (!pSubscribeInfo->bRemoveFlag)
				{
					bool bRes = false;
					try
					{
						pSubscribeInfo->Add();
						bRes = m_FireEventObj(pSubscribeInfo->pSink, nEventID, nSrcID, bySrcType, message);
						pSubscribeInfo->Sub();
					}
					catch (...)
					{
						//
						std::cerr << "[Event] pSubscribeInfo->nRefCount >= EVENT_REF_MAX_CNT....eventid:" << nEventID << ",srcid:" << nSrcID << ",type:" << bySrcType << ",refcount:" << pSubscribeInfo->nRefCount << ",removeflag:" << static_cast<int32_t>(pSubscribeInfo->bRemoveFlag) << ",szdesc:" << pSubscribeInfo->szDesc;
						m_nFireLayer--;
						return false;
					}
					if (pSubscribeInfo->bRemoveFlag && 0 == pSubscribeInfo->nRefCount)
					{
						iter = iterLst->second.erase(iter);
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
						iter = iterLst->second.erase(iter);
					}
					else
					{
						++iter;
					}
				}
			} // end of for (; iter != pLstSubscribe->end();)

			if (iterLst->second.empty())
			{
				m_mapAllSubscribeObj.erase(iterLst);
			}
		} // enf of if (iterLst != m_mapAllSubscribeObj.end())

		m_nFireLayer--;

		return true;
	}

private:
	//
	TEventObj m_FireEventObj;
	//
	std::unordered_map<SEventKey, std::list<SubscribeInfo>> m_mapAllSubscribeObj;
	//
	std::unordered_map<void*, std::unordered_set<SEventKey>> m_mapAllSubscribeKey;
	//
	int32_t m_nFireLayer;
};

