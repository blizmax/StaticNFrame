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
	* @brief 存取数据
	*/
	struct NFMemHashPair
	{
		KEY		_key;
		VALUE	_value;
	};

	////////////////////////////////////////////////////////////////////////
	/*
	* 内存数据块分配器
	*
	*/
	class BlockAllocator
	{
	public:

		/**
		* @brief 构造函数
		*/
		BlockAllocator(NFMemHashMap *pMap)
			: _pMap(pMap)
			, _pChunkAllocator(new NFMemChunkAllocator())
		{
		}

		/**
		* @brief 析够函数
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
		* @brief 初始化
		* @param pHeadAddr, 地址, 换到应用程序的绝对地址
		* @param iSize, 内存大小
		* @param iBlockSize, 数据块大小
		*/
		void create(void *pHeadAddr, size_t iSize, size_t iBlockSize)
		{
			_pChunkAllocator->create(pHeadAddr, iSize, iBlockSize);
		}

		/**
		* @brief 连接上
		* @param pAddr, 地址, 换到应用程序的绝对地址
		*/
		void connect(void *pHeadAddr)
		{
			_pChunkAllocator->connect(pHeadAddr);
		}

		/**
		* @brief 重建
		*/
		void rebuild()
		{
			_pChunkAllocator->rebuild();
		}

		/**
		* @brief 获取每种数据块头部信息
		*
		* @return NFMemChunk::tagChunkHead
		*/
		std::vector<NFMemChunk::tagChunkHead> getBlockDetail() const { return _pChunkAllocator->getBlockDetail(); }

		/**
		* @brief 获取内存大小
		*
		* @return size_t
		*/
		size_t getMemSize() const { return _pChunkAllocator->getMemSize(); }

		/**
		* @brief 获取真正的数据容量
		*
		* @return size_t
		*/
		size_t getCapacity() const { return _pChunkAllocator->getCapacity(); }

		/**
		* @brief 每种block中的chunk个数(每种block中的chunk个数相同)
		*
		* @return vector<size_t>
		*/
		std::vector<size_t> blockCount() const { return _pChunkAllocator->blockCount(); }

		/**
		* @brief 在内存中分配一个新的Block
		*
		* @param index, block hash索引
		* @param iAllocSize: in/需要分配的大小, out/分配的块大小
		* @param vtData, 返回释放的内存块数据
		* @return size_t, 相对地址,0表示没有空间可以分配
		*/
		size_t allocateMemBlock(size_t index, size_t &iAllocSize)
		{

		}

		/**
		* @brief 为地址为iAddr的Block分配一个chunk
		*
		* @param iAddr,分配的Block的地址
		* @param iAllocSize, in/需要分配的大小, out/分配的块大小
		* @param vtData 返回释放的内存块数据
		* @return size_t, 相对地址,0表示没有空间可以分配
		*/
		size_t allocateChunk(size_t iAddr, size_t &iAllocSize, std::vector<NFMemHashMap::BlockData> &vtData);

		/**
		* @brief 释放Block
		* @param v
		*/
		void deallocateMemBlock(const std::vector<size_t> &v);

		/**
		* @brief 释放Block
		* @param v
		*/
		void deallocateMemBlock(size_t v);

	protected:
		//不允许copy构造
		BlockAllocator(const BlockAllocator &);
		//不允许赋值
		BlockAllocator& operator=(const BlockAllocator &);
		bool operator==(const BlockAllocator &mba) const;
		bool operator!=(const BlockAllocator &mba) const;
	public:
		/**
		* map
		*/
		NFMemHashMap                  *_pMap;

		/**
		* chunk分配器
		*/
		NFMemChunkAllocator   *_pChunkAllocator;
	};
};