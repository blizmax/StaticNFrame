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
		* @brief ��ǰԪ���ƶ�����һ��block
		* @return true, �Ƶ���һ��block��, false, û����һ��block
		*
		*/
		bool nextBlock()
		{
			_iHead = getBlockHead()->_iBlockNext;

			return _iHead != 0;
		}

		/**
		* @brief ��ǰԪ���ƶ�����һ��block
		* @return true, �Ƶ���һ��block��, false, û����һ��block
		*
		*/
		bool prevBlock()
		{
			_iHead = getBlockHead()->_iBlockPrev;

			return _iHead != 0;
		}

		/**
		* @brief ��blockʱ���øú���,����һ���µ�block
		* @param index, hash����
		* @param iAllocSize, �ڴ��С
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

			//����block������
			if (_pMap->item(index)->_iBlockAddr == 0)
			{
				//��ǰhashͰû��Ԫ��
				_pMap->item(index)->_iBlockAddr = _iHead;
				getBlockHead()->_iBlockNext = (size_t)0;
				getBlockHead()->_iBlockPrev = (size_t)0;
			}
			else
			{
				//��ȻhashͰ��Ԫ��, ����Ͱ��ͷ
				getBlockHead(_pMap->item(index)->_iBlockAddr)->_iBlockPrev = _iHead;
				getBlockHead()->_iBlockNext = _pMap->item(index)->_iBlockAddr;
				_pMap->item(index)->_iBlockAddr = _iHead;
				getBlockHead()->_iBlockPrev = (size_t)0;
			}
		}

		/**
		* @brief �ͷ�block�����пռ�
		*/
		void deallocate()
		{
			_pMap->_pDataAllocator->deallocateMemBlock(_iHead);
		}

		/**
		* @brief ��ȡ���ݳ���
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
			///////////////////��block������ȥ��///////////
			//
			//��һ��blockָ����һ��block
			if (getBlockHead()->_iBlockPrev != 0)
			{
				getBlockHead(getBlockHead()->_iBlockPrev)->_iBlockNext = getBlockHead()->_iBlockNext;
			}

			//��һ��blockָ����һ��
			if (getBlockHead()->_iBlockNext != 0)
			{
				getBlockHead(getBlockHead()->_iBlockNext)->_iBlockPrev = getBlockHead()->_iBlockPrev;
			}

			//////////////////�����hashͷ��, ��Ҫ�޸�hash��������ָ��//////
			//
			_pMap->delListCount(getBlockHead()->_iIndex);
			if (getBlockHead()->_iBlockPrev == 0)
			{
				//�����hashͰ��ͷ��, ����Ҫ����
				NFMemHashMap::tagHashItem *pItem = _pMap->item(getBlockHead()->_iIndex);
				assert(pItem->_iBlockAddr == _iHead);
				if (pItem->_iBlockAddr == _iHead)
				{
					pItem->_iBlockAddr = getBlockHead()->_iBlockNext;
				}
			}

			//Ԫ�ظ�������
			_pMap->delElementCount();

			//�黹���ڴ���
			deallocate();
		}
	public:

		/**
		* Map
		*/
		NFMemHashMap         *_pMap;

		/**
		* block�����׵�ַ, ��Ե�ַ
		*/
		size_t              _iHead;
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
		* @param fFactor, ����
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
		NFMemChunk::tagChunkHead getBlockDetail() const { return _pChunkAllocator->getBlockDetail(); }

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
		* @brief ÿ��block�Ĵ�С
		*
		* @return block�Ĵ�С
		*/
		size_t getBlockSize() const { return _pChunkAllocator->getBlockSize(); }

		/**
		* @brief ���ڴ��з���һ���µ�Block
		*
		* @param index, block hash����
		* @param iAllocSize: in/��Ҫ����Ĵ�С, out/����Ŀ��С
		* @return size_t, ��Ե�ַ,0��ʾû�пռ���Է���
		*/
		size_t allocateMemBlock(size_t index)
		{
			void *pAddr = _pChunkAllocator->allocate();
			if (pAddr == NULL)
			{
				return 0;
			}

			//������µ�MemBlock, ��ʼ��һ��
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

	////////////////////////////////////////////////////////////////
	/**
	* @brief map��������
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
		* @brief ��ȡֵ
		* @return int
		*          RT_OK:���ݻ�ȡOK
		*          ����ֵ, �쳣
		*/
		VALUE& getValue()
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			return ptr->_value;
		}

		/**
		* @brief ��ȡֵ
		* @return int
		*          RT_OK:���ݻ�ȡOK
		*          ����ֵ, �쳣
		*/
		const VALUE& getValue() const
		{
			Block block(_pMap, _iAddr);
			NFMemHashPair* ptr = (NFMemHashPair*)block.getData();
			return ptr->_value;
		}

		/**
		* @brief ��ȡֵ
		* @return int
		*          RT_OK:���ݻ�ȡOK
		*          ����ֵ, �쳣
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
		* @brief ��ȡ���ݿ���Ե�ַ
		*
		* @return size_t
		*/
		size_t getAddr() const { return _iAddr; }
	protected:

		/**
		* @brief ��ȡֵ
		* @return int
		*          RT_OK:���ݻ�ȡOK
		*          ����ֵ, �쳣
		*/
		NFMemHashPair* getData()
		{
			Block block(_pMap, _iAddr);
			return (NFMemHashPair*)block.getData();
		}

		/**
		* @brief ��һ��item
		*
		* @return HashMapLockItem
		*/
		void nextItem(int iType)
		{
			Block block(_pMap, _iAddr);

			if (iType == HashMapIterator::IT_BLOCK)
			{
				size_t index = block.getBlockHead()->_iIndex;

				//��ǰblock������Ԫ��
				if (block.nextBlock())
				{
					_iAddr = block.getHead();
					return;
				}

				index += 1;

				while (index < _pMap->_hash.size())
				{
					//��ǰ��hashͰҲû������
					if (_pMap->item(index)->_iBlockAddr == 0)
					{
						++index;
						continue;
					}

					_iAddr = _pMap->item(index)->_iBlockAddr;
					return;
				}

				_iAddr = 0;  //��β����
			}
		}

		/**
		* ��һ��item
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
					//��ǰ��hashͰҲû������
					if (_pMap->item(index - 1)->_iBlockAddr == 0)
					{
						--index;
						continue;
					}

					//��Ҫ�����Ͱ��ĩβ
					Block tmp(_pMap, _pMap->item(index - 1)->_iBlockAddr);
					_iAddr = tmp.getLastBlockHead();

					return;
				}

				_iAddr = 0;  //��β����
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
		* block�ĵ�ַ
		*/
		size_t      _iAddr;
	};

	/////////////////////////////////////////////////////////////////////////
	/**
	* @brief ���������
	*/
	struct _NFExport HashMapIterator
	{
	public:

		/**
		*@brief ���������ʽ
		*/
		enum
		{
			IT_BLOCK = 0,        /**��ͨ��˳��*/
			IT_SET = 1,        /**Setʱ��˳��*/
			IT_GET = 2,        /**Getʱ��˳��*/
		};

		/**
		* ��������˳��
		*/
		enum
		{
			IT_NEXT = 0,        /**˳��*/
			IT_PREV = 1,        /**����*/
		};

		/**
		*
		*/
		HashMapIterator()
			: _pMap(NULL), _iItem(NULL, 0), _iType(IT_BLOCK), _iOrder(IT_NEXT)
		{
		}

		/**
		* @brief ���캯��
		* @param iAddr, ��ַ
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
		* @brief ����
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
		* @brief ǰ��++
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
		* @brief ����++
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
		* �������ķ�ʽ
		*/
		int        _iType;

		/**
		* ��������˳��
		*/
		int        _iOrder;

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
		size_t _iBlockSize;         /**��С���ݿ��С*/
		float   _fRadio;              /**chunks����/hash����*/
		size_t _iElementCount;       /**��Ԫ�ظ���*/
		size_t _iUsedChunk;          /**�Ѿ�ʹ�õ��ڴ��*/
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
		size_t _iMemSize;            /**�ڴ��С*/
		size_t _iBlockSize;         /**��С���ݿ��С*/
		float   _fRadio;              /**chunks����/hash����*/
		size_t _iElementCount;       /**��Ԫ�ظ���*/
		size_t _iUsedChunk;          /**�Ѿ�ʹ�õ��ڴ��*/
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
	* @brief ���������
	*/
	typedef HashMapIterator     iterator;

	/**
	* @brief ����hash������
	*/
	using hash_functor = std::function<size_t(const KEY&)>;

	/**
	* @brief ���캯��
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
		//����Ŀռ����Ҫ����΢���
		_pHead->_iBlockSize = _iBlockSize;
		_pHead->_fRadio = _fRadio;
		_pHead->_iElementCount = 0;
		_pHead->_iUsedChunk = 0;

		//Hash����
		size_t iHashCount = (iSize - sizeof(NFMemChunkAllocator::tagChunkAllocatorHead)) / ((size_t)(_iBlockSize*_fRadio) + sizeof(tagHashItem));
		//���������������Ϊhashֵ
		iHashCount = getMinPrimeNumber(iHashCount);

		void *pHashAddr = (char*)_pHead + sizeof(tagMapHead);

		size_t iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iHashCount);
		_hash.create(pHashAddr, iHashMemSize);

		void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

		_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead), _pHead->_iBlockSize);
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
		_pHead->_iUsedChunk = 0;

		_hash.clear();

		_pDataAllocator->rebuild();
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
	size_t hashIndex(const KEY& k)
	{
		return _hashf(k) % _hash.size();
	}

	/**
	* @brief  ����
	*
	* @return
	*/
	iterator end() { return _end; }

	/**
	* @brief  block����
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
	* @brief  block����
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
	* @brief  ����Key��������
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
			//����ռ�
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
	* @brief  ɾ������
	* @param k, �ؼ���
	* @param data, ��ɾ���ļ�¼
	* @return int:
	*          RT_READONLY: mapֻ��
	*          RT_NO_DATA: û�е�ǰ����
	*          RT_ONLY_KEY:ֻ��Key, ɾ���ɹ�
	*          RT_OK: ɾ�����ݳɹ�
	*         ��������ֵ: ����
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

	/**
	* @brief  �������ݸ���
	*/
	void incElementCount() { _pHead->_iElementCount += 1; }

	/**
	* @brief  �������ݸ���
	*/
	void delElementCount() { _pHead->_iElementCount -= 1; }

	/**
	* @brief  ĳhash�������ݸ���+1
	* @param index
	*/
	void incListCount(uint32_t index) { item(index)->_iListCount += 1; }

	/**
	* @brief  ĳhash�������ݸ���+1
	* @param index
	*/
	void delListCount(size_t index) { item(index)->_iListCount -= 1; }

	/**
	* @brief  ����Chunk��
	* ֱ�Ӹ���, ��Ϊ�п���һ�η����chunk����
	* �������������ڴ�ռ�, ����Խ�����
	*/
	void incChunkCount() { _pHead->_iUsedChunk++; }

	/**
	* @brief  ����Chunk��
	* ֱ�Ӹ���, ��Ϊ�п���һ���ͷŵ�chunk����
	* �������������ڴ�ռ�, ����Խ�����
	*/
	void delChunkCount() { _pHead->_iUsedChunk--; }
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
	* β��
	*/
	iterator					_end;

	/**
	* hashֵ���㹫ʽ
	*/
	hash_functor                _hashf;
};
