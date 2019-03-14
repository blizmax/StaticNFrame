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


template<Key,Value>
class _NFExport NFMemHashMap
{
public:
	class NFMemHashPair
	{
	public:
		NFMemHashPair()
		{

		}

		NFMemHashPair(const NFMemHashPair& pair)
		{
			_key = pair._key;
			_value = pair._value;
		}

		Key	_key;
		Value _value;
	}
	///////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief 内存数据块,读取和存放数据
	*/
	class Block
	{
	public:
#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  pack (push,1)
		/**
		* @brief block数据头
		*/
		struct tagBlockHead
		{
			uint32_t    _iSize;         /**block的容量大小*/
			uint32_t    _iIndex;        /**hash的索引*/
			size_t      _iBlockNext;    /**下一个Block,tagBlockHead, 没有则为0*/
			size_t      _iBlockPrev;    /**上一个Block,tagBlockHead, 没有则为0*/
			size_t		_iDataLen;      /**当前数据块中使用了的长度*/
			char        _cData[0];      /**数据开始部分*/
		};
#pragma pack(pop)
#else
		/**
		* @brief block数据头
		*/
		struct tagBlockHead
		{
			uint32_t    _iSize;         /**block的容量大小*/
			uint32_t    _iIndex;        /**hash的索引*/
			size_t      _iBlockNext;    /**下一个Block,tagBlockHead, 没有则为0*/
			size_t      _iBlockPrev;    /**上一个Block,tagBlockHead, 没有则为0*/
			size_t		_iDataLen;      /**当前数据块中使用了的长度*/
			char        _cData[0];      /**数据开始部分*/
		}__attribute__((packed));
#endif

		/**
		* @brief 构造函数
		* @param Map
		* @param iAddr 当前MemBlock的地址
		*/
		Block(NFMemHashMap *pMap, size_t iAddr)
			: _pMap(pMap)
			, _iHead(iAddr)
		{
		}

		/**
		* @brief copy
		* @param mb
		*/
		Block(const Block &mb)
			: _pMap(mb._pMap)
			, _iHead(mb._iHead)
		{
		}

		/**
		*
		* @param mb
		*
		* @return Block&
		*/
		Block& operator=(const Block &mb)
		{
			_iHead = mb._iHead;
			_pMap = mb._pMap;
			return (*this);
		}

		/**
		*
		* @param mb
		*
		* @return bool
		*/
		bool operator==(const Block &mb) const { return _iHead == mb._iHead && _pMap == mb._pMap; }

		/**
		*
		* @param mb
		*
		* @return bool
		*/
		bool operator!=(const Block &mb) const { return _iHead != mb._iHead || _pMap != mb._pMap; }

		/**
		* 获取block头绝对地址
		* @param iAddr
		*
		* @return tagChunkHead*
		*/
		tagBlockHead *getBlockHead(size_t iAddr) { return ((tagBlockHead*)_pMap->getAbsolute(iAddr)); }

		/**
		* 获取MemBlock头地址
		*
		* @return void*
		*/
		tagBlockHead *getBlockHead() { return getBlockHead(_iHead); }

		/**
		* 头部
		*
		* @return size_t
		*/
		size_t getHead() { return _iHead; }

		/**
		* @brief 当前桶链表最后一个block的头部
		*
		* @return size_t
		*/
		size_t getLastBlockHead()
		{
			size_t iHead = _iHead;

			while (getBlockHead(iHead)->_iBlockNext != 0)
			{
				iHead = getBlockHead(iHead)->_iBlockNext;
			}
			return iHead;
		}

		/**
		* @brief 当前元素移动到下一个block
		* @return true, 移到下一个block了, false, 没有下一个block
		*
		*/
		bool nextBlock()
		{
			_iHead = getBlockHead()->_iBlockNext;

			return _iHead != 0;
		}

		/**
		* @brief 当前元素移动到上一个block
		* @return true, 移到下一个block了, false, 没有下一个block
		*
		*/
		bool prevBlock()
		{
			_iHead = getBlockHead()->_iBlockPrev;

			return _iHead != 0;
		}

		/**
		* @brief 新block时调用该函数,分配一个新的block
		* @param index, hash索引
		* @param iAllocSize, 内存大小
		*/
		void makeNew(size_t index, size_t iAllocSize)
		{
			getBlockHead()->_iSize = iAllocSize;
			getBlockHead()->_iIndex = index;
			getBlockHead()->_iBlockNext = 0;
			getBlockHead()->_iBlockPrev = 0;
			getBlockHead()->_iDataLen = 0;

			_pMap->incElementCount();
			_pMap->incListCount(index);

			//挂在block链表上
			if (_pMap->item(index)->_iBlockAddr == 0)
			{
				//当前hash桶没有元素
				_pMap->item(index)->_iBlockAddr = _iHead;
				getBlockHead()->_iBlockNext = (size_t)0;
				getBlockHead()->_iBlockPrev = (size_t)0;
			}
			else
			{
				//当然hash桶有元素, 挂在桶开头
				getBlockHead(_pMap->item(index)->_iBlockAddr)->_iBlockPrev = _iHead;
				getBlockHead()->_iBlockNext = _pMap->item(index)->_iBlockAddr;
				_pMap->item(index)->_iBlockAddr = _iHead;
				getBlockHead()->_iBlockPrev = (size_t)0;
			}

			NFMemHashPair* pPair = get();
			new (pPair) NFMemHashPair();
		}

		/**
		* @brief 释放block的所有空间
		*/
		void deallocate()
		{
			_pMap->_pDataAllocator->deallocateMemBlock(_iHead);
		}

		/**
		* @brief 获取数据长度
		*
		* @return size_t
		*/
		size_t getDataLen()
		{
			return getBlockHead()->_iDataLen;
		}

		NFMemHashPair* get()
		{
			return (NFMemHashPair*)getBlockHead()->_cData;
		}
	public:

		/**
		* Map
		*/
		NFMemHashMap         *_pMap;

		/**
		* block区块首地址, 相对地址
		*/
		size_t              _iHead;
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
			, _pChunkAllocator(new NFMemMultiChunkAllocator())
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
		* @param fFactor, 因子
		*/
		void create(void *pHeadAddr, size_t iSize, size_t iMinBlockSize, size_t iMaxBlockSize, float fFactor)
		{
			_pChunkAllocator->create(pHeadAddr, iSize, iMinBlockSize, iMaxBlockSize, fFactor);
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
		* @brief 扩展空间
		* @param pAddr
		* @param iSize
		*/
		void append(void *pAddr, size_t iSize)
		{
			_pChunkAllocator->append(pAddr, iSize);
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
		std::vector<size_t> singleBlockChunkCount() const { return _pChunkAllocator->singleBlockChunkCount(); }

		/**
		* @brief 获取所有block的chunk个数
		*
		* @return size_t
		*/
		size_t allBlockChunkCount() const { return _pChunkAllocator->allBlockChunkCount(); }

		/**
		* @brief 在内存中分配一个新的Block
		*
		* @param index, block hash索引
		* @param iAllocSize: in/需要分配的大小, out/分配的块大小
		* @return size_t, 相对地址,0表示没有空间可以分配
		*/
		size_t allocateMemBlock(size_t index)
		{
		begin:
			size_t iAllocSize = sizeof(Block::tagBlockHead) + sizeof(NFMemHashPair);
			void *pAddr = _pChunkAllocator->allocate(iAllocSize, iAllocSize);
			if (pAddr == NULL)
			{
				return 0;
			}

			//分配的新的MemBlock, 初始化一下
			size_t iAddr = _pMap->getRelative(pAddr);
			Block block(_pMap, iAddr);
			block.makeNew(index, iAllocSize);

			_pMap->incChunkCount();

			return iAddr;
		}

		void deallocateMemBlock(const std::vector<size_t> &v)
		{
			for (size_t i = 0; i < v.size(); i++)
			{
				_pChunkAllocator->deallocate(_pMap->getAbsolute(v[i]));
				_pMap->delChunkCount();
			}
		}

		void deallocateMemBlock(size_t v)
		{
			_pChunkAllocator->deallocate(_pMap->getAbsolute(v));
			_pMap->delChunkCount();
		}
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
		NFMemHashMap2                  *_pMap;

		/**
		* chunk分配器
		*/
		NFMemMultiChunkAllocator   *_pChunkAllocator;
	};

	////////////////////////////////////////////////////////////////
	/**
	* @brief map的数据项
	*/
	class _NFExport HashMapLockItem
	{
	public:

		/**
		*
		* @param pMap
		* @param iAddr
		*/
		HashMapLockItem(NFMemHashMap *pMap, size_t iAddr)
			: _pMap(pMap)
			, _iAddr(iAddr)
		{
		}

		/**
		*
		* @param mcmdi
		*/
		HashMapLockItem(const HashMapLockItem &mcmdi)
			: _pMap(mcmdi._pMap)
			, _iAddr(mcmdi._iAddr)
		{
		}

		/**
		*
		* @param mcmdi
		*
		* @return HashMapLockItem&
		*/
		HashMapLockItem &operator=(const HashMapLockItem &mcmdi)
		{
			if (this != &mcmdi)
			{
				_pMap = mcmdi._pMap;
				_iAddr = mcmdi._iAddr;
			}
			return (*this);
		}

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator==(const HashMapLockItem &mcmdi)
		{
			return _pMap == mcmdi._pMap && _iAddr == mcmdi._iAddr;
		}

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator!=(const HashMapLockItem &mcmdi)
		{
			return _pMap != mcmdi._pMap || _iAddr != mcmdi._iAddr;
		}

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		Value* getValue()
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.get();
			return &ptr->_value;
		}

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		Key* getKey()
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.get();
			return &ptr->_key;
		}

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		NFMemHashPair* get()
		{
			Block block(_pMap, _iAddr);
			return (NFMemHashPair*)block.get();
		}

		/**
		* @brief 获取数据块相对地址
		*
		* @return size_t
		*/
		size_t getAddr() const { return _iAddr; }
	protected:

		/**
		* @brief 下一个item
		*
		* @return HashMapLockItem
		*/
		void nextItem(int iType)
		{
			Block block(_pMap, _iAddr);

			if (iType == HashMapLockIterator::IT_BLOCK)
			{
				size_t index = block.getBlockHead()->_iIndex;

				//当前block链表有元素
				if (block.nextBlock())
				{
					_iAddr = block.getHead();
					return;
				}

				index += 1;

				while (index < _pMap->_hash.size())
				{
					//当前的hash桶也没有数据
					if (_pMap->item(index)->_iBlockAddr == 0)
					{
						++index;
						continue;
					}

					_iAddr = _pMap->item(index)->_iBlockAddr;
					return;
				}

				_iAddr = 0;  //到尾部了
			}
		}

		/**
		* 上一个item
		* @param iType
		*/
		void prevItem(int iType)
		{
			Block block(_pMap, _iAddr);

			if (iType == HashMapLockIterator::IT_BLOCK)
			{
				size_t index = block.getBlockHead()->_iIndex;
				if (block.prevBlock())
				{
					_iAddr = block.getHead();
					return;
				}

				while (index > 0)
				{
					//当前的hash桶也没有数据
					if (_pMap->item(index - 1)->_iBlockAddr == 0)
					{
						--index;
						continue;
					}

					//需要到这个桶的末尾
					Block tmp(_pMap, _pMap->item(index - 1)->_iBlockAddr);
					_iAddr = tmp.getLastBlockHead();

					return;
				}

				_iAddr = 0;  //到尾部了
			}
		}

		bool NFMemHashMap::HashMapLockItem::equal(const Key& k)
		{
			NFMemHashPair* ptr = get();

			if (ptr->_key == k)
			{
				return true;
			}

			return false;
		}

		friend class NFMemHashMap;
		friend struct NFMemHashMap::HashMapLockIterator;

	private:
		/**
		* map
		*/
		NFMemHashMap *_pMap;

		/**
		* block的地址
		*/
		size_t      _iAddr;
	};

	/////////////////////////////////////////////////////////////////////////
	/**
	* @brief 定义迭代器
	*/
	struct _NFExport HashMapLockIterator
	{
	public:

		/**
		*@brief 定义遍历方式
		*/
		enum
		{
			IT_BLOCK = 0,        /**普通的顺序*/
			IT_SET = 1,        /**Set时间顺序*/
			IT_GET = 2,        /**Get时间顺序*/
		};

		/**
		* 迭代器的顺序
		*/
		enum
		{
			IT_NEXT = 0,        /**顺序*/
			IT_PREV = 1,        /**逆序*/
		};

		/**
		*
		*/
		HashMapLockIterator()
			: _pMap(NULL), _iItem(NULL, 0), _iType(IT_BLOCK), _iOrder(IT_NEXT)
		{
		}

		/**
		* @brief 构造函数
		* @param iAddr, 地址
		* @param type
		*/
		HashMapLockIterator(NFMemHashMap *pMap, size_t iAddr, int iType, int iOrder)
			: _pMap(pMap), _iItem(_pMap, iAddr), _iType(iType), _iOrder(iOrder)
		{
		}

		/**
		* @brief copy
		* @param it
		*/
		HashMapLockIterator(const HashMapLockIterator &it)
			: _pMap(it._pMap), _iItem(it._iItem), _iType(it._iType), _iOrder(it._iOrder)
		{
		}

		/**
		* @brief 复制
		* @param it
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator& operator=(const HashMapLockIterator &it)
		{
			if (this != &it)
			{
				_pMap = it._pMap;
				_iItem = it._iItem;
				_iType = it._iType;
				_iOrder = it._iOrder;
			}

			return (*this);
		}

		/**
		*
		* @param mcmi
		*
		* @return bool
		*/
		bool operator==(const HashMapLockIterator& mcmi)
		{
			if (_iItem.getAddr() != 0 || mcmi._iItem.getAddr() != 0)
			{
				return _pMap == mcmi._pMap
					&& _iItem == mcmi._iItem
					&& _iType == mcmi._iType
					&& _iOrder == mcmi._iOrder;
			}

			return _pMap == mcmi._pMap;
		}

		/**
		*
		* @param mv
		*
		* @return bool
		*/
		bool operator!=(const HashMapLockIterator& mcmi)
		{
			if (_iItem.getAddr() != 0 || mcmi._iItem.getAddr() != 0)
			{
				return _pMap != mcmi._pMap
					|| _iItem != mcmi._iItem
					|| _iType != mcmi._iType
					|| _iOrder != mcmi._iOrder;
			}

			return _pMap != mcmi._pMap;
		}

		/**
		* @brief 前置++
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator& operator++()
		{
			if (_iOrder == IT_NEXT)
			{
				_iItem.nextItem(_iType);
			}
			else
			{
				_iItem.prevItem(_iType);
			}
			return (*this);

		}

		/**
		* @brief 后置++
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator operator++(int)
		{
			HashMapLockIterator it(*this);

			if (_iOrder == IT_NEXT)
			{
				_iItem.nextItem(_iType);
			}
			else
			{
				_iItem.prevItem(_iType);
			}

			return it;

		}

		/**
		*
		*
		* @return HashMapLockItem&i
		*/
		HashMapLockItem& operator*() { return _iItem; }

		/**
		*
		*
		* @return HashMapLockItem*
		*/
		HashMapLockItem* operator->() { return &_iItem; }

	public:
		/**
		*
		*/
		NFMemHashMap  *_pMap;

		/**
		*
		*/
		HashMapLockItem _iItem;

		/**
		* 迭代器的方式
		*/
		int        _iType;

		/**
		* 迭代器的顺序
		*/
		int        _iOrder;

	};

	/////////////////////////////////////////////////////////////////////////
	/**
	* @brief 定义迭代器
	*/
	struct _NFExport HashMapLockIterator
	{
	public:

		/**
		*@brief 定义遍历方式
		*/
		enum
		{
			IT_BLOCK = 0,        /**普通的顺序*/
			IT_SET = 1,        /**Set时间顺序*/
			IT_GET = 2,        /**Get时间顺序*/
		};

		/**
		* 迭代器的顺序
		*/
		enum
		{
			IT_NEXT = 0,        /**顺序*/
			IT_PREV = 1,        /**逆序*/
		};

		/**
		*
		*/
		HashMapLockIterator();

		/**
		* @brief 构造函数
		* @param iAddr, 地址
		* @param type
		*/
		HashMapLockIterator(NFMemHashMap *pMap, size_t iAddr, int iType, int iOrder);

		/**
		* @brief copy
		* @param it
		*/
		HashMapLockIterator(const HashMapLockIterator &it);

		/**
		* @brief 复制
		* @param it
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator& operator=(const HashMapLockIterator &it);

		/**
		*
		* @param mcmi
		*
		* @return bool
		*/
		bool operator==(const HashMapLockIterator& mcmi);

		/**
		*
		* @param mv
		*
		* @return bool
		*/
		bool operator!=(const HashMapLockIterator& mcmi);

		/**
		* @brief 前置++
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator& operator++();

		/**
		* @brief 后置++
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator operator++(int);

		/**
		*
		*
		* @return HashMapLockItem&i
		*/
		HashMapLockItem& operator*() { return _iItem; }

		/**
		*
		*
		* @return HashMapLockItem*
		*/
		HashMapLockItem* operator->() { return &_iItem; }

	public:
		/**
		*
		*/
		NFMemHashMap  *_pMap;

		/**
		*
		*/
		HashMapLockItem _iItem;

		/**
		* 迭代器的方式
		*/
		int        _iType;

		/**
		* 迭代器的顺序
		*/
		int        _iOrder;

	};

	////////////////////////////////////////////////////////////////
	/**
	* @brief map的HashItem项, 一个HashItem对应多个数据项
	*/
	class HashMapItem
	{
	public:

		/**
		*
		* @param pMap
		* @param iIndex
		*/
		HashMapItem(NFMemHashMap *pMap, size_t iIndex)
			: _pMap(pMap)
			, _iIndex(iIndex)
		{
		}

		/**
		*
		* @param mcmdi
		*/
		HashMapItem(const HashMapItem &mcmdi)
			: _pMap(mcmdi._pMap)
			, _iIndex(mcmdi._iIndex)
		{
		}

		/**
		*
		* @param mcmdi
		*
		* @return HashMapItem&
		*/
		HashMapItem &operator=(const HashMapItem &mcmdi)
		{
			if (this != &mcmdi)
			{
				_pMap = mcmdi._pMap;
				_iIndex = mcmdi._iIndex;
			}
			return (*this);
		}

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator==(const HashMapItem &mcmdi)
		{
			return _pMap == mcmdi._pMap && _iIndex == mcmdi._iIndex;
		}

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator!=(const HashMapItem &mcmdi)
		{
			return _pMap != mcmdi._pMap || _iIndex != mcmdi._iIndex;
		}

		/**
		* @brief 获取当前hash桶的所有数量, 注意只获取有key/value的数据
		* 对于只有key的数据, 不获取
		*
		* @return
		*/
		void get(std::vector<NFMemHashPair*> &vtData)
		{
			size_t iAddr = _pMap->item(_iIndex)->_iBlockAddr;

			while (iAddr != 0)
			{
				Block block(_pMap, iAddr);

				NFMemHashPair* ptr = block.get();
				vtData.push_back(ptr);

				iAddr = block.getBlockHead()->_iBlockNext;
			}
		}

		/**
		*
		*
		* @return int
		*/
		int getIndex() const { return _iIndex; }

		/**
		* @brief 下一个item
		*
		*/
		void nextItem()
		{
			if (_iIndex == (size_t)(-1))
			{
				return;
			}

			if (_iIndex >= _pMap->getHashCount() - 1)
			{
				_iIndex = (size_t)(-1);
				return;
			}
			_iIndex++;
		}

		friend class NFMemHashMap;
		friend struct NFMemHashMap::HashMapIterator;

	private:
		/**
		* map
		*/
		NFMemHashMap *_pMap;

		/**
		* 数据块地址
		*/
		size_t      _iIndex;
	};

	/////////////////////////////////////////////////////////////////////////
	/**
	* @brief 定义迭代器
	*/
	struct _NFExport HashMapIterator
	{
	public:

		/**
		* @brief 构造函数
		*/
		HashMapIterator()
			: _pMap(NULL), _iItem(NULL, 0)
		{
		}

		/**
		* @brief 构造函数
		* @param iIndex, 地址
		* @param type
		*/
		HashMapIterator(NFMemHashMap *pMap, size_t iIndex)
			: _pMap(pMap), _iItem(_pMap, iIndex)
		{
		}

		/**
		* @brief copy
		* @param it
		*/
		HashMapIterator(const HashMapIterator &it)
			: _pMap(it._pMap), _iItem(it._iItem)
		{
		}

		/**
		* @brief 复制
		* @param it
		*
		* @return HashMapLockIterator&
		*/
		HashMapIterator& operator=(const HashMapIterator &it)
		{
			if (this != &it)
			{
				_pMap = it._pMap;
				_iItem = it._iItem;
			}

			return (*this);
		}

		/**
		*
		* @param mcmi
		*
		* @return bool
		*/
		bool operator==(const HashMapIterator& mcmi)
		{
			if (_iItem.getIndex() != -1 || mcmi._iItem.getIndex() != -1)
			{
				return _pMap == mcmi._pMap && _iItem == mcmi._iItem;
			}

			return _pMap == mcmi._pMap;
		}

		/**
		*
		* @param mv
		*
		* @return bool
		*/
		bool operator!=(const HashMapIterator& mcmi)
		{
			if (_iItem.getIndex() != -1 || mcmi._iItem.getIndex() != -1)
			{
				return _pMap != mcmi._pMap || _iItem != mcmi._iItem;
			}

			return _pMap != mcmi._pMap;
		}

		/**
		* @brief 前置++
		*
		* @return HashMapIterator&
		*/
		HashMapIterator& operator++()
		{
			_iItem.nextItem();
			return (*this);
		}

		/**
		* @brief 后置++
		*
		* @return HashMapIterator&
		*/
		HashMapIterator operator++(int)
		{
			HashMapIterator it(*this);
			_iItem.nextItem();
			return it;
		}

		/**
		*
		*
		* @return HashMapItem&i
		*/
		HashMapItem& operator*() { return _iItem; }

		/**
		*
		*
		* @return HashMapItem*
		*/
		HashMapItem* operator->() { return &_iItem; }

	public:
		/**
		*
		*/
		NFMemHashMap  *_pMap;

		/**
		*
		*/
		HashMapItem _iItem;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////
#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  pack (push,1)
	/**
	* @brief map头
	*/
	struct tagMapHead
	{
		char   _cMaxVersion;        /**大版本*/
		char   _cMinVersion;         /**小版本*/
		size_t _iMemSize;            /**内存大小*/
		size_t _iMinDataSize;        /**最小数据块大小*/
		size_t _iMaxDataSize;        /**最大数据块大小*/
		float  _fFactor;             /**因子*/
		float   _fRadio;              /**chunks个数/hash个数*/
		size_t _iElementCount;       /**总元素个数*/
		size_t _iUsedChunk;          /**已经使用的内存块*/
		size_t _iReserve[4];        /**保留*/
	};

	/**
	* HashItem
	*/
	struct tagHashItem
	{
		size_t   _iBlockAddr;     /**指向数据项的偏移地址*/
		uint32_t _iListCount;     /**链表个数*/
	};
#pragma pack(pop)
#else
	/**
	* @brief map头
	*/
	struct tagMapHead
	{
		char   _cMaxVersion;        /**大版本*/
		char   _cMinVersion;         /**小版本*/
		size_t _iMemSize;            /**内存大小*/
		size_t _iMinDataSize;        /**最小数据块大小*/
		size_t _iMaxDataSize;        /**最大数据块大小*/
		float  _fFactor;             /**因子*/
		float   _fRadio;              /**chunks个数/hash个数*/
		size_t _iElementCount;       /**总元素个数*/
		size_t _iReserve[4];        /**保留*/
	}__attribute__((packed));

	/**
	* HashItem
	*/
	struct tagHashItem
	{
		size_t   _iBlockAddr;     /**指向数据项的偏移地址*/
		uint32_t _iListCount;     /**链表个数*/
	}__attribute__((packed));
#endif

	//64位操作系统用基数版本号, 32位操作系统用64位版本号
#if __WORDSIZE == 64

	//定义版本号
	enum
	{
		MAX_VERSION = 0,    /**当前map的大版本号*/
		MIN_VERSION = 3,    /**当前map的小版本号*/
	};

#else
	//定义版本号
	enum
	{
		MAX_VERSION = 0,    /**当前map的大版本号*/
		MIN_VERSION = 2,    /**当前map的小版本号*/
	};

#endif

	/**
	*@brief 定义淘汰方式
	*/
	enum
	{
		ERASEBYGET = 0x00, /**按照Get链表淘汰*/
		ERASEBYSET = 0x01, /**按照Set链表淘汰*/
	};

	/**
	* @brief get, set等int返回值
	*/
	enum
	{
		RT_OK = 0,    /**成功*/
		RT_DIRTY_DATA = 1,    /**脏数据*/
		RT_NO_DATA = 2,    /**没有数据*/
		RT_NEED_SYNC = 3,    /**需要回写*/
		RT_NONEED_SYNC = 4,    /**不需要回写*/
		RT_ERASE_OK = 5,    /**淘汰数据成功*/
		RT_READONLY = 6,    /**map只读*/
		RT_NO_MEMORY = 7,    /**内存不够*/
		RT_ONLY_KEY = 8,    /**只有Key, 没有Value*/
		RT_NEED_BACKUP = 9,    /**需要备份*/
		RT_NO_GET = 10,   /**没有GET过*/
		RT_DECODE_ERR = -1,   /**解析错误*/
		RT_EXCEPTION_ERR = -2,   /**异常*/
		RT_LOAD_DATA_ERR = -3,   /**加载数据异常*/
		RT_VERSION_MISMATCH_ERR = -4,   /**版本不一致*/
		RT_DUMP_FILE_ERR = -5,   /**dump到文件失败*/
		RT_LOAL_FILE_ERR = -6,   /**load文件到内存失败*/
		RT_NOTALL_ERR = -7,   /**没有复制完全*/
	};

	/**
	* @brief 定义hash处理器
	*/
	using hash_functor = std::function<size_t(const Key&)>;

	/**
	* @brief 构造函数
	*/
	NFMemHashMap2()
		: _iMinDataSize(0)
		, _iMaxDataSize(0)
		, _fFactor(1.0)
		, _fRadio(2)
		, _pDataAllocator(new BlockAllocator(this))
		, _hashf(NFHash::hash<Key>())
	{
	}

	/**
	*  @brief 定义hash处理器初始化数据块平均大小
	* 表示内存分配的时候，会分配n个最小块， n个（最小快*增长因子）, n个（最小快*增长因子*增长因子）..., 直到n个最大块
	* n是hashmap自己计算出来的
	* 这种分配策略通常是数据块记录变长比较多的使用， 便于节约内存，如果数据记录基本不是变长的， 那最小块=最大快，增长因子=1就可以了
	* @param iMinDataSize 最小数据块大小
	* @param iMaxDataSize 最大数据块大小
	* @param fFactor      增长因子
	*/
	void initDataBlockSize(size_t iMinDataSize, size_t iMaxDataSize, float fFactor)
	{
		_iMinDataSize = iMinDataSize;
		_iMaxDataSize = iMaxDataSize;
		_fFactor = fFactor;
	}

	/**
	*  @brief 始化chunk数据块/hash项比值,
	*         默认是2,有需要更改必须在create之前调用
	*
	* @param fRadio
	*/
	void initHashRadio(float fRadio) { _fRadio = fRadio; }

	/**
	* @brief  初始化
	* @param pAddr
	*/
	void init(void *pAddr)
	{
		_pHead = static_cast<tagMapHead*>(pAddr);
	}

	/**
	* @brief 初始化, 之前需要调用:initDataAvgSize和initHashRadio
	* @param pAddr 绝对地址
	* @param iSize 大小
	* @return 失败则抛出异常
	*/
	void create(void *pAddr, size_t iSize)
	{
		if (sizeof(tagHashItem) * 1
			+ sizeof(tagMapHead)
			+ sizeof(NFMemMultiChunkAllocator::tagChunkAllocatorHead)
			+ 10 > iSize)
		{
			throw NF_HashMap_Exception("[NFMemHashMap::create] mem size not enougth.");
		}

		//分配的内存比需要的大一点
		initDataBlockSize(sizeof(NFMemHashPair) * 2, sizeof(NFMemHashPair) * 2, 1);
		initHashRadio(2);

		if (_iMinDataSize == 0 || _iMaxDataSize == 0 || _fFactor < 1.0)
		{
			throw NF_HashMap_Exception("[NFMemHashMap::create] init data size error:" + lexical_cast<std::string>(_iMinDataSize) + "|" + lexical_cast<std::string>(_iMaxDataSize) + "|" + lexical_cast<std::string>(_fFactor));
		}

		init(pAddr);

		_pHead->_cMaxVersion = MAX_VERSION;
		_pHead->_cMinVersion = MIN_VERSION;
		_pHead->_iMemSize = iSize;
		_pHead->_iMinDataSize = _iMinDataSize;
		_pHead->_iMaxDataSize = _iMaxDataSize;
		_pHead->_fFactor = _fFactor;
		_pHead->_fRadio = _fRadio;
		_pHead->_iElementCount = 0;

		//计算平均block大小
		size_t iBlockSize = (_pHead->_iMinDataSize + _pHead->_iMaxDataSize) / 2 + sizeof(Block::tagBlockHead);

		//Hash个数
		size_t iHashCount = (iSize - sizeof(NFMemChunkAllocator::tagChunkAllocatorHead)) / ((size_t)(iBlockSize*_fRadio) + sizeof(tagHashItem));
		//采用最近的素数作为hash值
		iHashCount = getMinPrimeNumber(iHashCount);

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);

		size_t iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iHashCount);
		_hash.create(pHashAddr, iHashMemSize);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead), sizeof(Block::tagBlockHead) + _pHead->_iMinDataSize, sizeof(Block::tagBlockHead) + _pHead->_iMaxDataSize, _pHead->_fFactor);
	}

	/**
	* @brief  链接到内存块
	* @param pAddr, 地址
	* @param iSize, 内存大小
	* @return 失败则抛出异常
	*/
	void connect(void *pAddr, size_t iSize)
	{
		init(pAddr);

		if (_pHead->_cMaxVersion != MAX_VERSION || _pHead->_cMinVersion != MIN_VERSION)
		{
			std::ostringstream os;
			os << (int)_pHead->_cMaxVersion << "." << (int)_pHead->_cMinVersion << " != " << ((int)MAX_VERSION) << "." << ((int)MIN_VERSION);
			throw NF_HashMap_Exception("[NFMemHashMap::connect] hash map version not equal:" + os.str() + " (data != code)");
		}

		if (_pHead->_iMemSize != iSize)
		{
			throw NF_HashMap_Exception("[NFMemHashMap::connect] hash map size not equal:" + NFCommon::tostr(_pHead->_iMemSize) + "!=" + NFCommon::tostr(iSize));
		}

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);
		_hash.connect(pHashAddr);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->connect(pDataAddr);
		_iMinDataSize = _pHead->_iMinDataSize;
		_iMaxDataSize = _pHead->_iMaxDataSize;
		_fFactor = _pHead->_fFactor;
		_fRadio = _pHead->_fRadio;
	}

	/**
	*  @brief 原来的数据块基础上扩展内存,注意通常只能对mmap文件生效
	* (如果iSize比本来的内存就小,则返回-1)
	* @param pAddr, 扩展后的空间
	* @param iSize
	* @return 0:成功, -1:失败
	*/
	int append(void *pAddr, size_t iSize)
	{
		if (iSize <= _pHead->_iMemSize)
		{
			return -1;
		}

		init(pAddr);

		if (_pHead->_cMaxVersion != MAX_VERSION || _pHead->_cMinVersion != MIN_VERSION)
		{
			std::ostringstream os;
			os << (int)_pHead->_cMaxVersion << "." << (int)_pHead->_cMinVersion << " != " << ((int)MAX_VERSION) << "." << ((int)MIN_VERSION);
			throw NF_HashMap_Exception("[TC_HashMap::append] hash map version not equal:" + os.str() + " (data != code)");
		}

		_pHead->_iMemSize = iSize;

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);
		_hash.connect(pHashAddr);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();
		_pDataAllocator->append(pDataAddr, iSize - ((size_t)pDataAddr - (size_t)pAddr));

		_iMinDataSize = _pHead->_iMinDataSize;
		_iMaxDataSize = _pHead->_iMaxDataSize;
		_fFactor = _pHead->_fFactor;
		_fRadio = _pHead->_fRadio;

		return 0;
	}

	/**
	*  @brief 获取每种大小内存块的头部信息
	*
	* @return vector<NFMemChunk::tagChunkHead>: 不同大小内存块头部信息
	*/
	std::vector<NFMemChunk::tagChunkHead> getBlockDetail() { return _pDataAllocator->getBlockDetail(); }

	/**
	* @brief 所有block中chunk的个数
	*
	* @return size_t
	*/
	size_t allBlockChunkCount() { return _pDataAllocator->allBlockChunkCount(); }

	/**
	* @brief 每种block中chunk的个数(不同大小内存块的个数相同)
	*
	* @return vector<size_t>
	*/
	std::vector<size_t> singleBlockChunkCount() { return _pDataAllocator->singleBlockChunkCount(); }

	/**
	* @brief  获取hash桶的个数
	*
	* @return size_t
	*/
	size_t getHashCount() { return _hash.size(); }

	/**
	* @brief  获取元素的个数
	*
	* @return size_t
	*/
	size_t size() { return _pHead->_iElementCount; }

	/**
	* @brief  获取头部数据信息
	*
	* @return tagMapHead&
	*/
	tagMapHead& getMapHead() { return *_pHead; }

	/**
	* @brief  设置hash方式
	* @param hash_of
	*/
	void setHashFunctor(hash_functor hashf) { _hashf = hashf; }

	/**
	* @brief  返回hash处理器
	*
	* @return hash_functor&
	*/
	hash_functor &getHashFunctor() { return _hashf; }

	/**
	* @brief  hash item
	* @param index
	*
	* @return tagHashItem&
	*/
	tagHashItem *item(size_t iIndex) { return &_hash[iIndex]; }

	/**
	* @brief  清空hashmap
	* 所有map的数据恢复到初始状态
	*/
	void clear()
	{
		assert(_pHead);

		_pHead->_iElementCount = 0;
		_pHead->_iUsedChunk = 0;

		_hash.clear();

		_pDataAllocator->rebuild();
	}

	/**
	* @brief 相对地址换成绝对地址
	* @param iAddr
	*
	* @return void*
	*/
	void *getAbsolute(size_t iAddr) { return (void *)((char*)_pHead + iAddr); }

	/**
	* @brief  绝对地址换成相对地址
	*
	* @return size_t
	*/
	size_t getRelative(void *pAddr) { return (char*)pAddr - (char*)_pHead; }

	/**
	* @brief  根据Key计算hash值
	* @param pKey
	* @param iKeyLen
	*
	* @return size_t
	*/
	size_t hashIndex(const Key& k)
	{
		return _hashf(k) % _hash.size();
	}

	/**
	* @brief  结束
	*
	* @return
	*/
	lock_iterator end() { return _lock_end; }

	/**
	* @brief  block正序
	*
	* @return lock_iterator
	*/
	lock_iterator begin()
	{
		for (size_t i = 0; i < _hash.size(); i++)
		{
			tagHashItem &hashItem = _hash[i];
			if (hashItem._iBlockAddr != 0)
			{
				return lock_iterator(this, hashItem._iBlockAddr, lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
			}
		}

		return end();
	}

	/**
	* @brief  block逆序
	*
	* @return lock_iterator
	*/
	lock_iterator rbegin()
	{
		for (size_t i = _hash.size(); i > 0; i--)
		{
			tagHashItem &hashItem = _hash[i - 1];
			if (hashItem._iBlockAddr != 0)
			{
				Block block(this, hashItem._iBlockAddr);
				return lock_iterator(this, block.getLastBlockHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_PREV);
			}
		}

		return end();
	}


	/**
	* @brief  根据Key查找数据
	* @param string
	*/
	lock_iterator find(const Key& k)
	{
		size_t index = hashIndex(k);
		if (item(index)->_iBlockAddr == 0)
		{
			return end();
		}

		Block mb(this, item(index)->_iBlockAddr);
		while (true)
		{
			HashMapLockItem mcmdi(this, mb.getHead());
			if (mcmdi.equal(k))
			{
				return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
			}

			if (!mb.nextBlock())
			{
				return end();
			}
		}

		return end();
	}

	lock_iterator find(const Key& k, size_t index)
	{
		if (item(index)->_iBlockAddr == 0)
		{
			return end();
		}

		Block mb(this, item(index)->_iBlockAddr);
		while (true)
		{
			HashMapLockItem mcmdi(this, mb.getHead());
			if (mcmdi.equal(k))
			{
				return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
			}

			if (!mb.nextBlock())
			{
				return end();
			}
		}

		return end();
	}

	lock_iterator insert(const Key& k, const Value& v)
	{
		size_t index = hashIndex(k);
		lock_iterator it = find(k, index);

		if (it == end())
		{
			//先分配空间, 并获得淘汰的数据
			size_t iAddr = _pDataAllocator->allocateMemBlock(index);
			if (iAddr == 0)
			{
				return end();
			}

			it = HashMapLockIterator(this, iAddr, HashMapLockIterator::IT_BLOCK, HashMapLockIterator::IT_NEXT);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// 以下是遍历map函数, 不需要对map加锁

	/**
	* @brief  根据hash桶遍历
	*
	* @return hash_iterator
	*/
	hash_iterator hashBegin()
	{
		return hash_iterator(this, 0);
	}

	/**
	* @brief  结束
	*
	* @return
	*/
	hash_iterator hashEnd() { return _end; }

	/**
	* 获取指定下标的hash_iterator
	* @param iIndex
	*
	* @return hash_iterator
	*/
	hash_iterator hashIndex(size_t iIndex)
	{
		return hash_iterator(this, iIndex);
	}

	/**
	* @brief  获取大于n且离n最近的素数
	* @param n
	*
	* @return size_t
	*/
	size_t getMinPrimeNumber(size_t n)
	{
		while (true)
		{
			if (NFCommon::isPrimeNumber(n))
			{
				return n;
			}
			++n;
		}
		return 3;
	}

	/**
	* @brief  增加数据个数
	*/
	void incElementCount() { _pHead->_iElementCount += 1; }

	/**
	* @brief  减少数据个数
	*/
	void delElementCount() { _pHead->_iElementCount -= 1; }

	/**
	* @brief  某hash链表数据个数+1
	* @param index
	*/
	void incListCount(uint32_t index) { item(index)->_iListCount += 1; }

	/**
	* @brief  某hash链表数据个数+1
	* @param index
	*/
	void delListCount(size_t index) { item(index)->_iListCount -= 1; }

	/**
	* @brief  增加Chunk数
	* 直接更新, 因为有可能一次分配的chunk个数
	* 多余更新区块的内存空间, 导致越界错误
	*/
	void incChunkCount() { _pHead->_iUsedChunk++; }

	/**
	* @brief  减少Chunk数
	* 直接更新, 因为有可能一次释放的chunk个数
	* 多余更新区块的内存空间, 导致越界错误
	*/
	void delChunkCount() { _pHead->_iUsedChunk--; }
protected:

	/**
	* 区块指针
	*/
	tagMapHead                  *_pHead;

	/**
	* 最小的数据块大小
	*/
	size_t                      _iMinDataSize;

	/**
	* 最大的数据块大小
	*/
	size_t                      _iMaxDataSize;

	/**
	* 变化因子
	*/
	float                       _fFactor;

	/**
	* 设置chunk数据块/hash项比值
	*/
	float                       _fRadio;

	/**
	* hash对象
	*/
	NFMemVector<tagHashItem>   _hash;

	/**
	* block分配器对象
	*/
	BlockAllocator              *_pDataAllocator;

	/**
	* 尾部
	*/
	lock_iterator               _lock_end;

	/**
	* 尾部
	*/
	hash_iterator               _end;

	/**
	* hash值计算公式
	*/
	hash_functor                _hashf;
};