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

template<typename Key, typename Value>
class _NFExport NFMemHashMap2
{
public:

	/**
	* @brief 操作数据
	*/
	struct BlockData
	{
		Key  _key;       /**数据Key*/
		Value  _value;     /**数据value*/
		BlockData()
		{
		}
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
		* @brief 获取Block中的数据
		*
		* @return int
		*          TC_HashMap::RT_OK, 正常, 其他异常
		*          TC_HashMap::RT_ONLY_KEY, 只有Key
		*          其他异常
		*/
		BlockData* getBlockData()
		{
			return get();
		}

		/**
		* @brief 获取数据
		* @param pData
		* @param iDatalen
		* @return int,
		*          TC_HashMap::RT_OK, 正常
		*          其他异常
		*/
		BlockData* get()
		{
			return getBlockHead()->_cData;
		}

		/**
		* @brief 设置数据
		* @param pData
		* @param iDatalen
		* @param vtData, 淘汰的数据
		*/
		int set(const Key& key, const Value& value)
		{
			void *p = allocate();
			new (BlockData*)p;
		}

		/**
		* @brief 如果数据容量不够, 则新增加chunk, 不影响原有数据
		* 使新增加的总容量大于iDataLen,释放多余的chunk
		* @param iDataLen
		*
		* @return int,
		*/
		void* allocate()
		{
			return getBlockHead()->_cData;
		}
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
		BlockAllocator(NFMemHashMap2 *pMap)
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
		void create(void *pHeadAddr, size_t iSize, size_t iBlockSize)
		{
			_pChunkAllocator->create(pHeadAddr, iSize, iBlockSize, iBlockSize, 1.0);
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
		size_t _iBlockSize;         /**数据块大小*/
		float   _fRadio;              /**chunks个数/hash个数*/
		size_t _iElementCount;       /**总元素个数*/
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
		bool   _bReadOnly;           /**是否只读*/
		bool   _bAutoErase;          /**是否可以自动淘汰*/
		char   _cEraseMode;          /**淘汰方式:0x00:按照Get链淘汰, 0x01:按照Set链淘汰*/
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
		: _iBlockSize(sizeof(BlockData))
		, _fRadio(2)
		, _pDataAllocator(new BlockAllocator(this))
		, _hashf(NFHash::hash<Key>())
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
	void create(void *pAddr, size_t iSize, float fRadio = 2.0)
	{
		initHashRadio(fRadio);

		if (sizeof(tagHashItem) * 1
			+ sizeof(tagMapHead)
			+ sizeof(NFMemMultiChunkAllocator::tagChunkAllocatorHead)
			+ 10 > iSize)
		{
			throw NF_HashMap_Exception("[NFMemHashMap::create] mem size not enougth.");
		}

		init(pAddr);

		_pHead->_cMaxVersion = MAX_VERSION;
		_pHead->_cMinVersion = MIN_VERSION;
		_pHead->_iMemSize = iSize;
		_pHead->_iBlockSize = _iBlockSize;
		_pHead->_fRadio = _fRadio;
		_pHead->_iElementCount = 0;

		//计算平均block大小
		size_t iBlockSize = _pHead->_iBlockSize + sizeof(Block::tagBlockHead);

		//Hash个数
		size_t iHashCount = (iSize - sizeof(NFMemChunkAllocator::tagChunkAllocatorHead)) / ((size_t)(iBlockSize*_fRadio) + sizeof(tagHashItem));
		//采用最近的素数作为hash值
		iHashCount = getMinPrimeNumber(iHashCount);

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);

		size_t iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iHashCount);
		_hash.create(pHashAddr, iHashMemSize);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead), sizeof(Block::tagBlockHead) + _pHead->_iBlockSize);
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

		if (_pHead->_iBlockSize != _iBlockSize)
		{
			std::ostringstream os;
			os << (int)_pHead->_iBlockSize  << " != " << _iBlockSize;
			throw NF_HashMap_Exception("[NFMemHashMap::connect] hash map blocksize not equal:" + os.str() + " (data != code)");
		}

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);
		_hash.connect(pHashAddr);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->connect(pDataAddr);
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

		if (_pHead->_iBlockSize != _iBlockSize)
		{
			std::ostringstream os;
			os << (int)_pHead->_iBlockSize << " != " << _iBlockSize;
			throw NF_HashMap_Exception("[NFMemHashMap::connect] hash map blocksize not equal:" + os.str() + " (data != code)");
		}

		_pHead->_iMemSize = iSize;

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);
		_hash.connect(pHashAddr);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();
		_pDataAllocator->append(pDataAddr, iSize - ((size_t)pDataAddr - (size_t)pAddr));

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

		_hash.clear();

		_pDataAllocator->rebuild();
	}

	/**
	* @brief  设置数据, 修改时间链, 内存不够时会自动淘汰老的数据
	* @param k: 关键字
	* @param v: 值
	* @param bDirty: 是否是脏数据
	* @param vtData: 被淘汰的记录
	* @return int:
	*          RT_READONLY: map只读
	*          RT_NO_MEMORY: 没有空间(不淘汰数据情况下会出现)
	*          RT_OK: 设置成功
	*          其他返回值: 错误
	*/
	int set(const Key& k, const Value& v)
	{
		int ret = NFMemHashMap::RT_OK;
		size_t index = hashIndex(k);
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
	* hash值计算公式
	*/
	hash_functor                _hashf;
};