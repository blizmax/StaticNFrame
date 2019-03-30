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


template<typename KEY, typename VALUE>
class _NFExport NFMemHashMap
{
public:
	class NFMemHashPair
	{
	public:
		NFMemHashPair()
		{

		}

		~NFMemHashPair()
		{

		}

		NFMemHashPair(const NFMemHashPair& pair)
		{
			_key = pair._key;
			_value = pair._value;
		}

		KEY	_key;
		VALUE _value;
	};

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
			getBlockHead()->_iDataLen = iAllocSize - sizeof(tagBlockHead);

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

		void* getData()
		{
			return getBlockHead()->_cData;
		}

		void erase()
		{
			///////////////////从block链表中去掉///////////
			//
			//上一个block指向下一个block
			if (getBlockHead()->_iBlockPrev != 0)
			{
				getBlockHead(getBlockHead()->_iBlockPrev)->_iBlockNext = getBlockHead()->_iBlockNext;
			}

			//下一个block指向上一个
			if (getBlockHead()->_iBlockNext != 0)
			{
				getBlockHead(getBlockHead()->_iBlockNext)->_iBlockPrev = getBlockHead()->_iBlockPrev;
			}

			//////////////////如果是hash头部, 需要修改hash索引数据指针//////
			//
			_pMap->delListCount(getBlockHead()->_iIndex);
			if (getBlockHead()->_iBlockPrev == 0)
			{
				//如果是hash桶的头部, 则还需要处理
				NFMemHashMap::tagHashItem *pItem = _pMap->item(getBlockHead()->_iIndex);
				assert(pItem->_iBlockAddr == _iHead);
				if (pItem->_iBlockAddr == _iHead)
				{
					pItem->_iBlockAddr = getBlockHead()->_iBlockNext;
				}
			}

			//元素个数减少
			_pMap->delElementCount();

			//归还到内存中
			deallocate();
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
		* @param fFactor, 因子
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
		NFMemChunk::tagChunkHead getBlockDetail() const { return _pChunkAllocator->getBlockDetail(); }

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
		* @brief 每个block的大小
		*
		* @return block的大小
		*/
		size_t getBlockSize() const { return _pChunkAllocator->getBlockSize(); }

		/**
		* @brief 在内存中分配一个新的Block
		*
		* @param index, block hash索引
		* @param iAllocSize: in/需要分配的大小, out/分配的块大小
		* @return size_t, 相对地址,0表示没有空间可以分配
		*/
		size_t allocateMemBlock(size_t index)
		{
			void *pAddr = _pChunkAllocator->allocate();
			if (pAddr == NULL)
			{
				return 0;
			}

			//分配的新的MemBlock, 初始化一下
			size_t iAddr = _pMap->getRelative(pAddr);
			Block block(_pMap, iAddr);
			block.makeNew(index, _pChunkAllocator->getBlockSize());

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
		NFMemHashMap                  *_pMap;

		/**
		* chunk分配器
		*/
		NFMemChunkAllocator   *_pChunkAllocator;
	};

	////////////////////////////////////////////////////////////////
	/**
	* @brief map的数据项
	*/
	class _NFExport HashMapItem
	{
	public:

		/**
		*
		* @param pMap
		* @param iAddr
		*/
		HashMapItem(NFMemHashMap *pMap, size_t iAddr)
			: _pMap(pMap)
			, _iAddr(iAddr)
		{
		}

		/**
		*
		* @param mcmdi
		*/
		HashMapItem(const HashMapItem &mcmdi)
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
		HashMapItem &operator=(const HashMapItem &mcmdi)
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
		bool operator==(const HashMapItem &mcmdi)
		{
			return _pMap == mcmdi._pMap && _iAddr == mcmdi._iAddr;
		}

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator!=(const HashMapItem &mcmdi)
		{
			return _pMap != mcmdi._pMap || _iAddr != mcmdi._iAddr;
		}

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		VALUE& getValue()
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			return ptr->_value;
		}

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		const VALUE& getValue() const
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			return ptr->_value;
		}

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		const KEY& getKey() const
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			return ptr->_key;
		}

		void Insert(const KEY& k, const VALUE& v)
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			ptr->_key = k;
			ptr->_value = v;
		}

		/**
		* @brief 获取数据块相对地址
		*
		* @return size_t
		*/
		size_t getAddr() const { return _iAddr; }
	protected:

		/**
		* @brief 获取值
		* @return int
		*          RT_OK:数据获取OK
		*          其他值, 异常
		*/
		NFMemHashPair* getData()
		{
			Block block(_pMap, _iAddr);
			return (NFMemHashPair*)block.getData();
		}

		/**
		* @brief 下一个item
		*
		* @return HashMapLockItem
		*/
		void nextItem(int iType)
		{
			Block block(_pMap, _iAddr);

			if (iType == HashMapIterator::IT_BLOCK)
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

			if (iType == HashMapIterator::IT_BLOCK)
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

		bool equal(const KEY& k)
		{
			const KEY& key = getKey();

			if (key == k)
			{
				return true;
			}

			return false;
		}

		friend class NFMemHashMap;
		friend struct NFMemHashMap::HashMapIterator;
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
	struct _NFExport HashMapIterator
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
		HashMapIterator()
			: _pMap(NULL), _iItem(NULL, 0), _iType(IT_BLOCK), _iOrder(IT_NEXT)
		{
		}

		/**
		* @brief 构造函数
		* @param iAddr, 地址
		* @param type
		*/
		HashMapIterator(NFMemHashMap *pMap, size_t iAddr, int iType, int iOrder)
			: _pMap(pMap), _iItem(_pMap, iAddr), _iType(iType), _iOrder(iOrder)
		{
		}

		/**
		* @brief copy
		* @param it
		*/
		HashMapIterator(const HashMapIterator &it)
			: _pMap(it._pMap), _iItem(it._iItem), _iType(it._iType), _iOrder(it._iOrder)
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
		bool operator==(const HashMapIterator& mcmi)
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
		bool operator!=(const HashMapIterator& mcmi)
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
		HashMapIterator& operator++()
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
		HashMapIterator operator++(int)
		{
			HashMapIterator it(*this);

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
		HashMapItem& operator*() 
		{
			if (_iItem._pMap == NULL || _iItem._iAddr == 0)
			{
				throw NFException("_iItem._pMap == NULL || _iItem._iAddr == 0");
			}
			return _iItem; 
		}

		/**
		*
		*
		* @return HashMapLockItem*
		*/
		HashMapItem* operator->() 
		{
			if (_iItem._pMap == NULL || _iItem._iAddr == 0)
			{
				throw NFException("_iItem._pMap == NULL || _iItem._iAddr == 0");
			}
			return &_iItem; 
		}

	public:
		/**
		*
		*/
		NFMemHashMap  *_pMap;

		/**
		*
		*/
		HashMapItem _iItem;

		/**
		* 迭代器的方式
		*/
		int        _iType;

		/**
		* 迭代器的顺序
		*/
		int        _iOrder;

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
		size_t _iBlockSize;         /**最小数据块大小*/
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
		size_t _iBlockSize;         /**最小数据块大小*/
		float   _fRadio;              /**chunks个数/hash个数*/
		size_t _iElementCount;       /**总元素个数*/
		size_t _iUsedChunk;          /**已经使用的内存块*/
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
	* @brief 定义迭代器
	*/
	typedef HashMapIterator     iterator;

	/**
	* @brief 定义hash处理器
	*/
	using hash_functor = std::function<size_t(const KEY&)>;

	/**
	* @brief 构造函数
	*/
	NFMemHashMap()
		:_iBlockSize(sizeof(NFMemHashPair) * 2 + sizeof(typename NFMemHashMap<KEY, VALUE>::Block::tagBlockHead))
		, _fRadio(2)
		, _pDataAllocator(new BlockAllocator(this))
		, _end(this, 0, 0, 0)
		, _hashf(NFHash::hash<KEY>())
	{
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
			throw NFException("[NFMemHashMap::create] mem size not enougth.");
		}

		init(pAddr);

		_pHead->_cMaxVersion = MAX_VERSION;
		_pHead->_cMinVersion = MIN_VERSION;
		_pHead->_iMemSize = iSize;
		//分配的空间比需要的稍微大点
		_pHead->_iBlockSize = _iBlockSize;
		_pHead->_fRadio = _fRadio;
		_pHead->_iElementCount = 0;
		_pHead->_iUsedChunk = 0;

		//Hash个数
		size_t iHashCount = (iSize - sizeof(NFMemChunkAllocator::tagChunkAllocatorHead)) / ((size_t)(_iBlockSize*_fRadio) + sizeof(tagHashItem));
		//采用最近的素数作为hash值
		iHashCount = getMinPrimeNumber(iHashCount);

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);

		size_t iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iHashCount);
		_hash.create(pHashAddr, iHashMemSize);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead), _pHead->_iBlockSize);
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
			throw NFException("[NFMemHashMap::connect] hash map version not equal:" + os.str() + " (data != code)");
		}

		if (_pHead->_iMemSize != iSize)
		{
			throw NFException("[NFMemHashMap::connect] hash map size not equal:" + NFCommon::tostr(_pHead->_iMemSize) + "!=" + NFCommon::tostr(iSize));
		}

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);
		_hash.connect(pHashAddr);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->connect(pDataAddr);
		_iBlockSize = _pHead->_iBlockSize;
		_fRadio = _pHead->_fRadio;
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
	size_t hashIndex(const KEY& k)
	{
		return _hashf(k) % _hash.size();
	}

	/**
	* @brief  结束
	*
	* @return
	*/
	iterator end() { return _end; }

	/**
	* @brief  block正序
	*
	* @return iterator
	*/
	iterator begin()
	{
		for (size_t i = 0; i < _hash.size(); i++)
		{
			tagHashItem &hashItem = _hash[i];
			if (hashItem._iBlockAddr != 0)
			{
				return iterator(this, hashItem._iBlockAddr, iterator::IT_BLOCK, iterator::IT_NEXT);
			}
		}

		return end();
	}

	/**
	* @brief  block逆序
	*
	* @return iterator
	*/
	iterator rbegin()
	{
		for (size_t i = _hash.size(); i > 0; i--)
		{
			tagHashItem &hashItem = _hash[i - 1];
			if (hashItem._iBlockAddr != 0)
			{
				Block block(this, hashItem._iBlockAddr);
				return iterator(this, block.getLastBlockHead(), iterator::IT_BLOCK, iterator::IT_PREV);
			}
		}

		return end();
	}


	/**
	* @brief  根据Key查找数据
	* @param string
	*/
	iterator find(const KEY& k)
	{
		size_t index = hashIndex(k);
		if (item(index)->_iBlockAddr == 0)
		{
			return end();
		}

		Block mb(this, item(index)->_iBlockAddr);
		while (true)
		{
			HashMapItem mcmdi(this, mb.getHead());
			if (mcmdi.equal(k))
			{
				return iterator(this, mb.getHead(), iterator::IT_BLOCK, iterator::IT_NEXT);
			}

			if (!mb.nextBlock())
			{
				return end();
			}
		}

		return end();
	}

	iterator find(const KEY& k, size_t index)
	{
		if (item(index)->_iBlockAddr == 0)
		{
			return end();
		}

		Block mb(this, item(index)->_iBlockAddr);
		while (true)
		{
			HashMapItem mcmdi(this, mb.getHead());
			if (mcmdi.equal(k))
			{
				return iterator(this, mb.getHead(), iterator::IT_BLOCK, iterator::IT_NEXT);
			}

			if (!mb.nextBlock())
			{
				return end();
			}
		}

		return end();
	}

	iterator insert(const KEY& k, const VALUE& v)
	{
		size_t index = hashIndex(k);
		iterator it = find(k, index);

		if (it == end())
		{
			//分配空间
			size_t iAddr = _pDataAllocator->allocateMemBlock(index);
			if (iAddr == 0)
			{
				return end();
			}

			Block block(this, iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			new (ptr) NFMemHashPair();

			it = iterator(this, iAddr, iterator::IT_BLOCK, iterator::IT_NEXT);
		}

		it->Insert(k, v);
		return it;
	}

	/**
	* @brief  删除数据
	* @param k, 关键字
	* @param data, 被删除的记录
	* @return int:
	*          RT_READONLY: map只读
	*          RT_NO_DATA: 没有当前数据
	*          RT_ONLY_KEY:只有Key, 删除成功
	*          RT_OK: 删除数据成功
	*         其他返回值: 错误
	*/
	void erase(const KEY& k)
	{
		size_t index = hashIndex(k);
		iterator it = find(k, index);

		if (it == end())
		{
			return;
		}

		Block block(this, it->getAddr());
		block.erase();

		return;
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
	* 数据块大小
	*/
	size_t                      _iBlockSize;

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
	iterator					_end;

	/**
	* hash值计算公式
	*/
	hash_functor                _hashf;
};
