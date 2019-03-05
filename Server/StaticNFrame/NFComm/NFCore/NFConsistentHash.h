// -------------------------------------------------------------------------
//    @FileName         :    NFConsistentHash.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "NFPlatform.h"
#include "NFMD5.h"

/////////////////////////////////////////////////
/**
* @file NFConsistentHash.h
* @brief һ����hash�㷨��.
*/
/////////////////////////////////////////////////
struct NFNode_T
{
	/**
	*�ڵ�hashֵ
	*/
	unsigned int iHashCode;

	/**
	*�ڵ��±�
	*/
	unsigned int iIndex;
};

/**
*  @brief һ����hash�㷨��
*/
class NFConsistentHash
{
public:

	/**
	*  @brief ���캯��
	*/
	NFConsistentHash()
	{
	}

	/**
	* @brief �ڵ�Ƚ�.
	*
	* @param m1 NFNode_T���͵Ķ��󣬱ȽϽڵ�֮һ
	* @param m2 NFNode_T���͵Ķ��󣬱ȽϽڵ�֮һ
	* @return less or not �ȽϽ����less����ture�����򷵻�false
	*/
	static bool less_hash(const NFNode_T & m1, const NFNode_T & m2)
	{
		return m1.iHashCode < m2.iHashCode;
	}

	/**
	* @brief ���ӽڵ�.
	*
	* @param node  �ڵ�����
	* @param index �ڵ���±�ֵ
	* @return      �ڵ��hashֵ
	*/
	unsigned addNode(const std::string & node, unsigned int index)
	{
		NFNode_T stItem;
		stItem.iHashCode = hash_md5(NFMD5::md5bin(node));
		stItem.iIndex = index;
		vHashList.push_back(stItem);

		std::sort(vHashList.begin(), vHashList.end(), less_hash);

		return stItem.iHashCode;
	}

	/**
	* @brief ɾ���ڵ�.
	*
	* @param node  �ڵ�����
	* @return       0 : ɾ���ɹ�  -1 : û�ж�Ӧ�ڵ�
	*/
	int removeNode(const std::string & node)
	{
		unsigned iHashCode = hash_md5(NFMD5::md5bin(node));
		std::vector<NFNode_T>::iterator it;
		for (it = vHashList.begin(); it != vHashList.end(); it++)
		{
			if (it->iHashCode == iHashCode)
			{
				vHashList.erase(it);
				return 0;
			}
		}
		return -1;
	}

	/**
	* @brief ��ȡĳkey��Ӧ���Ľڵ�node���±�.
	*
	* @param key      key����
	* @param iIndex  ��Ӧ���Ľڵ��±�
	* @return        0:��ȡ�ɹ�   -1:û�б���ӵĽڵ�
	*/
	int getIndex(const std::string & key, unsigned int & iIndex)
	{
		unsigned iCode = hash_md5(NFMD5::md5bin(key));
		if (vHashList.size() == 0)
		{
			iIndex = 0;
			return -1;
		}

		unsigned low = 0;
		unsigned high = vHashList.size();

		if (iCode <= vHashList[0].iHashCode || iCode > vHashList[high - 1].iHashCode)
		{
			iIndex = vHashList[0].iIndex;
			return 0;
		}

		while (low < high - 1)
		{
			unsigned mid = (low + high) / 2;
			if (vHashList[mid].iHashCode > iCode)
			{
				high = mid;
			}
			else
			{
				low = mid;
			}
		}
		iIndex = vHashList[low + 1].iIndex;
		return 0;
	}

protected:
	/**
	* @brief ����md5ֵ��hash���ֲ���Χ�� 0 -- 2^32-1.
	*
	* @param  sMd5 md5ֵ
	* @return      hashֵ
	*/
	unsigned int hash_md5(const std::string & sMd5)
	{
		char *p = (char *)sMd5.c_str();
		return (*(int*)(p)) ^ (*(int*)(p + 4)) ^ (*(int*)(p + 8)) ^ (*(int*)(p + 12));
	}

	std::vector<NFNode_T> vHashList;
};
