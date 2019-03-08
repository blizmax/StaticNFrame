// -------------------------------------------------------------------------
//    @FileName         :    NFMemHashMap2.h
//    @Author           :    GaoYi
//    @Date             :    2019-3-8
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <functional>
#include "NFPlatform.h"
#include "NFException.hpp"
#include "NFMemVector.h"
#include "NFMemChunk.h"
#include "NFHash.hpp"
#include "NFCommon.h"


template<KEY, VALUE>
class _NFExport NFMemHashMap2
{
public:
	struct HashMapIterator;
	struct HashMapLockIterator;

	/**
	* @brief ��ȡ����
	*/
	struct NFMemHashPair
	{
		KEY		_key;
		VALUE	_value;
	};

	////////////////////////////////////////////////////////////////////////
	/*
	* �ڴ����ݿ������
	*
	*/
	class BlockAllocator
	{
	public:

		/**
		* @brief ���캯��
		*/
		BlockAllocator(NFMemHashMap *pMap)
			: _pMap(pMap)
			, _pChunkAllocator(new NFMemChunkAllocator())
		{
		}

		/**
		* @brief ��������
		*/
		~BlockAllocator()
		{
			if (_pChunkAllocator != NULL)
			{
				delete _pChunkAllocator;
			}
			_pChunkAllocator = NULL;
		}


		/**
		* @brief ��ʼ��
		* @param pHeadAddr, ��ַ, ����Ӧ�ó���ľ��Ե�ַ
		* @param iSize, �ڴ��С
		* @param iBlockSize, ���ݿ��С
		*/
		void create(void *pHeadAddr, size_t iSize, size_t iBlockSize)
		{
			_pChunkAllocator->create(pHeadAddr, iSize, iBlockSize);
		}

		/**
		* @brief ������
		* @param pAddr, ��ַ, ����Ӧ�ó���ľ��Ե�ַ
		*/
		void connect(void *pHeadAddr)
		{
			_pChunkAllocator->connect(pHeadAddr);
		}

		/**
		* @brief �ؽ�
		*/
		void rebuild()
		{
			_pChunkAllocator->rebuild();
		}

		/**
		* @brief ��ȡÿ�����ݿ�ͷ����Ϣ
		*
		* @return NFMemChunk::tagChunkHead
		*/
		std::vector<NFMemChunk::tagChunkHead> getBlockDetail() const { return _pChunkAllocator->getBlockDetail(); }

		/**
		* @brief ��ȡ�ڴ��С
		*
		* @return size_t
		*/
		size_t getMemSize() const { return _pChunkAllocator->getMemSize(); }

		/**
		* @brief ��ȡ��������������
		*
		* @return size_t
		*/
		size_t getCapacity() const { return _pChunkAllocator->getCapacity(); }

		/**
		* @brief ÿ��block�е�chunk����(ÿ��block�е�chunk������ͬ)
		*
		* @return vector<size_t>
		*/
		std::vector<size_t> blockCount() const { return _pChunkAllocator->blockCount(); }

		/**
		* @brief ���ڴ��з���һ���µ�Block
		*
		* @param index, block hash����
		* @param iAllocSize: in/��Ҫ����Ĵ�С, out/����Ŀ��С
		* @param vtData, �����ͷŵ��ڴ������
		* @return size_t, ��Ե�ַ,0��ʾû�пռ���Է���
		*/
		size_t allocateMemBlock(size_t index, size_t &iAllocSize)
		{

		}

		/**
		* @brief Ϊ��ַΪiAddr��Block����һ��chunk
		*
		* @param iAddr,�����Block�ĵ�ַ
		* @param iAllocSize, in/��Ҫ����Ĵ�С, out/����Ŀ��С
		* @param vtData �����ͷŵ��ڴ������
		* @return size_t, ��Ե�ַ,0��ʾû�пռ���Է���
		*/
		size_t allocateChunk(size_t iAddr, size_t &iAllocSize, std::vector<NFMemHashMap::BlockData> &vtData);

		/**
		* @brief �ͷ�Block
		* @param v
		*/
		void deallocateMemBlock(const std::vector<size_t> &v);

		/**
		* @brief �ͷ�Block
		* @param v
		*/
		void deallocateMemBlock(size_t v);

	protected:
		//������copy����
		BlockAllocator(const BlockAllocator &);
		//������ֵ
		BlockAllocator& operator=(const BlockAllocator &);
		bool operator==(const BlockAllocator &mba) const;
		bool operator!=(const BlockAllocator &mba) const;
	public:
		/**
		* map
		*/
		NFMemHashMap                  *_pMap;

		/**
		* chunk������
		*/
		NFMemChunkAllocator   *_pChunkAllocator;
	};
};