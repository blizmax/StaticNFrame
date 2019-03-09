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
	* @brief ��������
	*/
	struct BlockData
	{
		Key  _key;       /**����Key*/
		Value  _value;     /**����value*/
		BlockData()
		{
		}
	};

	///////////////////////////////////////////////////////////////////////////////////
	/**
	* @brief �ڴ����ݿ�,��ȡ�ʹ������
	*/
	class Block
	{
	public:
#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  pack (push,1)
		/**
		* @brief block����ͷ
		*/
		struct tagBlockHead
		{
			uint32_t    _iSize;         /**block��������С*/
			uint32_t    _iIndex;        /**hash������*/
			size_t      _iBlockNext;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			size_t      _iBlockPrev;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			size_t		_iDataLen;      /**��ǰ���ݿ���ʹ���˵ĳ���*/
			char        _cData[0];      /**���ݿ�ʼ����*/
		};
#pragma pack(pop)
#else
		/**
		* @brief block����ͷ
		*/
		struct tagBlockHead
		{
			uint32_t    _iSize;         /**block��������С*/
			uint32_t    _iIndex;        /**hash������*/
			size_t      _iBlockNext;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			size_t      _iBlockPrev;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			size_t		_iDataLen;      /**��ǰ���ݿ���ʹ���˵ĳ���*/
			char        _cData[0];      /**���ݿ�ʼ����*/
		}__attribute__((packed));
#endif

		/**
		* @brief ���캯��
		* @param Map
		* @param iAddr ��ǰMemBlock�ĵ�ַ
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
		* ��ȡblockͷ���Ե�ַ
		* @param iAddr
		*
		* @return tagChunkHead*
		*/
		tagBlockHead *getBlockHead(size_t iAddr) { return ((tagBlockHead*)_pMap->getAbsolute(iAddr)); }

		/**
		* ��ȡMemBlockͷ��ַ
		*
		* @return void*
		*/
		tagBlockHead *getBlockHead() { return getBlockHead(_iHead); }

		/**
		* ͷ��
		*
		* @return size_t
		*/
		size_t getHead() { return _iHead; }

		/**
		* @brief ��ǰͰ�������һ��block��ͷ��
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
		* @brief ��ȡBlock�е�����
		*
		* @return int
		*          TC_HashMap::RT_OK, ����, �����쳣
		*          TC_HashMap::RT_ONLY_KEY, ֻ��Key
		*          �����쳣
		*/
		BlockData* getBlockData()
		{
			return get();
		}

		/**
		* @brief ��ȡ����
		* @param pData
		* @param iDatalen
		* @return int,
		*          TC_HashMap::RT_OK, ����
		*          �����쳣
		*/
		BlockData* get()
		{
			return getBlockHead()->_cData;
		}

		/**
		* @brief ��������
		* @param pData
		* @param iDatalen
		* @param vtData, ��̭������
		*/
		int set(const Key& key, const Value& value)
		{
			void *p = allocate();
			new (BlockData*)p;
		}

		/**
		* @brief ���������������, ��������chunk, ��Ӱ��ԭ������
		* ʹ�����ӵ�����������iDataLen,�ͷŶ����chunk
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
	* �ڴ����ݿ������
	*
	*/
	class BlockAllocator
	{
	public:

		/**
		* @brief ���캯��
		*/
		BlockAllocator(NFMemHashMap2 *pMap)
			: _pMap(pMap)
			, _pChunkAllocator(new NFMemMultiChunkAllocator())
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
		* @param fFactor, ����
		*/
		void create(void *pHeadAddr, size_t iSize, size_t iBlockSize)
		{
			_pChunkAllocator->create(pHeadAddr, iSize, iBlockSize, iBlockSize, 1.0);
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
		* @brief ��չ�ռ�
		* @param pAddr
		* @param iSize
		*/
		void append(void *pAddr, size_t iSize)
		{
			_pChunkAllocator->append(pAddr, iSize);
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
		std::vector<size_t> singleBlockChunkCount() const { return _pChunkAllocator->singleBlockChunkCount(); }

		/**
		* @brief ��ȡ����block��chunk����
		*
		* @return size_t
		*/
		size_t allBlockChunkCount() const { return _pChunkAllocator->allBlockChunkCount(); }

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
		NFMemHashMap2                  *_pMap;

		/**
		* chunk������
		*/
		NFMemMultiChunkAllocator   *_pChunkAllocator;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////
#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  pack (push,1)
	/**
	* @brief mapͷ
	*/
	struct tagMapHead
	{
		char   _cMaxVersion;        /**��汾*/
		char   _cMinVersion;         /**С�汾*/
		size_t _iMemSize;            /**�ڴ��С*/
		size_t _iBlockSize;         /**���ݿ��С*/
		float   _fRadio;              /**chunks����/hash����*/
		size_t _iElementCount;       /**��Ԫ�ظ���*/
		size_t _iReserve[4];        /**����*/
	};

	/**
	* HashItem
	*/
	struct tagHashItem
	{
		size_t   _iBlockAddr;     /**ָ���������ƫ�Ƶ�ַ*/
		uint32_t _iListCount;     /**�������*/
	};
#pragma pack(pop)
#else
	/**
	* @brief mapͷ
	*/
	struct tagMapHead
	{
		char   _cMaxVersion;        /**��汾*/
		char   _cMinVersion;         /**С�汾*/
		bool   _bReadOnly;           /**�Ƿ�ֻ��*/
		bool   _bAutoErase;          /**�Ƿ�����Զ���̭*/
		char   _cEraseMode;          /**��̭��ʽ:0x00:����Get����̭, 0x01:����Set����̭*/
		size_t _iMemSize;            /**�ڴ��С*/
		size_t _iMinDataSize;        /**��С���ݿ��С*/
		size_t _iMaxDataSize;        /**������ݿ��С*/
		float  _fFactor;             /**����*/
		float   _fRadio;              /**chunks����/hash����*/
		size_t _iElementCount;       /**��Ԫ�ظ���*/
		size_t _iReserve[4];        /**����*/
	}__attribute__((packed));

	/**
	* HashItem
	*/
	struct tagHashItem
	{
		size_t   _iBlockAddr;     /**ָ���������ƫ�Ƶ�ַ*/
		uint32_t _iListCount;     /**�������*/
	}__attribute__((packed));
#endif

	//64λ����ϵͳ�û����汾��, 32λ����ϵͳ��64λ�汾��
#if __WORDSIZE == 64

	//����汾��
	enum
	{
		MAX_VERSION = 0,    /**��ǰmap�Ĵ�汾��*/
		MIN_VERSION = 3,    /**��ǰmap��С�汾��*/
	};

#else
	//����汾��
	enum
	{
		MAX_VERSION = 0,    /**��ǰmap�Ĵ�汾��*/
		MIN_VERSION = 2,    /**��ǰmap��С�汾��*/
	};

#endif

	/**
	*@brief ������̭��ʽ
	*/
	enum
	{
		ERASEBYGET = 0x00, /**����Get������̭*/
		ERASEBYSET = 0x01, /**����Set������̭*/
	};

	/**
	* @brief get, set��int����ֵ
	*/
	enum
	{
		RT_OK = 0,    /**�ɹ�*/
		RT_DIRTY_DATA = 1,    /**������*/
		RT_NO_DATA = 2,    /**û������*/
		RT_NEED_SYNC = 3,    /**��Ҫ��д*/
		RT_NONEED_SYNC = 4,    /**����Ҫ��д*/
		RT_ERASE_OK = 5,    /**��̭���ݳɹ�*/
		RT_READONLY = 6,    /**mapֻ��*/
		RT_NO_MEMORY = 7,    /**�ڴ治��*/
		RT_ONLY_KEY = 8,    /**ֻ��Key, û��Value*/
		RT_NEED_BACKUP = 9,    /**��Ҫ����*/
		RT_NO_GET = 10,   /**û��GET��*/
		RT_DECODE_ERR = -1,   /**��������*/
		RT_EXCEPTION_ERR = -2,   /**�쳣*/
		RT_LOAD_DATA_ERR = -3,   /**���������쳣*/
		RT_VERSION_MISMATCH_ERR = -4,   /**�汾��һ��*/
		RT_DUMP_FILE_ERR = -5,   /**dump���ļ�ʧ��*/
		RT_LOAL_FILE_ERR = -6,   /**load�ļ����ڴ�ʧ��*/
		RT_NOTALL_ERR = -7,   /**û�и�����ȫ*/
	};

	/**
	* @brief ����hash������
	*/
	using hash_functor = std::function<size_t(const Key&)>;

	/**
	* @brief ���캯��
	*/
	NFMemHashMap2()
		: _iBlockSize(sizeof(BlockData))
		, _fRadio(2)
		, _pDataAllocator(new BlockAllocator(this))
		, _hashf(NFHash::hash<Key>())
	{
	}

	/**
	*  @brief ʼ��chunk���ݿ�/hash���ֵ,
	*         Ĭ����2,����Ҫ���ı�����create֮ǰ����
	*
	* @param fRadio
	*/
	void initHashRadio(float fRadio) { _fRadio = fRadio; }

	/**
	* @brief  ��ʼ��
	* @param pAddr
	*/
	void init(void *pAddr)
	{
		_pHead = static_cast<tagMapHead*>(pAddr);
	}

	/**
	* @brief ��ʼ��, ֮ǰ��Ҫ����:initDataAvgSize��initHashRadio
	* @param pAddr ���Ե�ַ
	* @param iSize ��С
	* @return ʧ�����׳��쳣
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

		//����ƽ��block��С
		size_t iBlockSize = _pHead->_iBlockSize + sizeof(Block::tagBlockHead);

		//Hash����
		size_t iHashCount = (iSize - sizeof(NFMemChunkAllocator::tagChunkAllocatorHead)) / ((size_t)(iBlockSize*_fRadio) + sizeof(tagHashItem));
		//���������������Ϊhashֵ
		iHashCount = getMinPrimeNumber(iHashCount);

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);

		size_t iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iHashCount);
		_hash.create(pHashAddr, iHashMemSize);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead), sizeof(Block::tagBlockHead) + _pHead->_iBlockSize);
	}

	/**
	* @brief  ���ӵ��ڴ��
	* @param pAddr, ��ַ
	* @param iSize, �ڴ��С
	* @return ʧ�����׳��쳣
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
	*  @brief ԭ�������ݿ��������չ�ڴ�,ע��ͨ��ֻ�ܶ�mmap�ļ���Ч
	* (���iSize�ȱ������ڴ��С,�򷵻�-1)
	* @param pAddr, ��չ��Ŀռ�
	* @param iSize
	* @return 0:�ɹ�, -1:ʧ��
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
	*  @brief ��ȡÿ�ִ�С�ڴ���ͷ����Ϣ
	*
	* @return vector<NFMemChunk::tagChunkHead>: ��ͬ��С�ڴ��ͷ����Ϣ
	*/
	std::vector<NFMemChunk::tagChunkHead> getBlockDetail() { return _pDataAllocator->getBlockDetail(); }

	/**
	* @brief ����block��chunk�ĸ���
	*
	* @return size_t
	*/
	size_t allBlockChunkCount() { return _pDataAllocator->allBlockChunkCount(); }

	/**
	* @brief ÿ��block��chunk�ĸ���(��ͬ��С�ڴ��ĸ�����ͬ)
	*
	* @return vector<size_t>
	*/
	std::vector<size_t> singleBlockChunkCount() { return _pDataAllocator->singleBlockChunkCount(); }

	/**
	* @brief  ��ȡhashͰ�ĸ���
	*
	* @return size_t
	*/
	size_t getHashCount() { return _hash.size(); }

	/**
	* @brief  ��ȡԪ�صĸ���
	*
	* @return size_t
	*/
	size_t size() { return _pHead->_iElementCount; }

	/**
	* @brief  ��ȡͷ��������Ϣ
	*
	* @return tagMapHead&
	*/
	tagMapHead& getMapHead() { return *_pHead; }

	/**
	* @brief  ����hash��ʽ
	* @param hash_of
	*/
	void setHashFunctor(hash_functor hashf) { _hashf = hashf; }

	/**
	* @brief  ����hash������
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
	* @brief  ���hashmap
	* ����map�����ݻָ�����ʼ״̬
	*/
	void clear()
	{
		assert(_pHead);

		_pHead->_iElementCount = 0;

		_hash.clear();

		_pDataAllocator->rebuild();
	}

	/**
	* @brief  ��������, �޸�ʱ����, �ڴ治��ʱ���Զ���̭�ϵ�����
	* @param k: �ؼ���
	* @param v: ֵ
	* @param bDirty: �Ƿ���������
	* @param vtData: ����̭�ļ�¼
	* @return int:
	*          RT_READONLY: mapֻ��
	*          RT_NO_MEMORY: û�пռ�(����̭��������»����)
	*          RT_OK: ���óɹ�
	*          ��������ֵ: ����
	*/
	int set(const Key& k, const Value& v)
	{
		int ret = NFMemHashMap::RT_OK;
		size_t index = hashIndex(k);
	}

	/**
	* @brief ��Ե�ַ���ɾ��Ե�ַ
	* @param iAddr
	*
	* @return void*
	*/
	void *getAbsolute(size_t iAddr) { return (void *)((char*)_pHead + iAddr); }

	/**
	* @brief  ���Ե�ַ������Ե�ַ
	*
	* @return size_t
	*/
	size_t getRelative(void *pAddr) { return (char*)pAddr - (char*)_pHead; }

	/**
	* @brief  ����Key����hashֵ
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
	* @brief  ��ȡ����n����n���������
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
	* ����ָ��
	*/
	tagMapHead                  *_pHead;

	/**
	* ���ݿ��С
	*/
	size_t                      _iBlockSize;

	/**
	* ����chunk���ݿ�/hash���ֵ
	*/
	float                       _fRadio;

	/**
	* hash����
	*/
	NFMemVector<tagHashItem>   _hash;

	/**
	* block����������
	*/
	BlockAllocator              *_pDataAllocator;

	/**
	* hashֵ���㹫ʽ
	*/
	hash_functor                _hashf;
};