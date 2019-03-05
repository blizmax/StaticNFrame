// -------------------------------------------------------------------------
//    @FileName         :    NFHashMapCompact.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
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
#include "NFTCPack.h"
#include "NFMemChunk.h"
#include "NFHash.hpp"
#include "NFCommon.h"


/////////////////////////////////////////////////
/**
* @file tc_hashmap_compact.h
* @brief  hashmap��(������, 64λos��, ÿ��chunk�Ķ����ڴ�ռ�ñ�tc_hashmapҪС30���ֽ�)
*
*/
/////////////////////////////////////////////////
/**
* @brief Hash map�쳣��
*/
struct NF_HashMapCompact_Exception : public NFException
{
	NF_HashMapCompact_Exception(const std::string &buffer) : NFException(buffer) {};
	NF_HashMapCompact_Exception(const std::string &buffer, int err) : NFException(buffer, err) {};
	~NF_HashMapCompact_Exception() throw() {};
};

////////////////////////////////////////////////////////////////////////////////////
/**
* @brief ������hashmap.
* �����ڴ��hashmap,���в�����Ҫ�Լ����� , ʹ�÷�ʽͬTC_Hashmap������64λϵͳ�¸���Լ����Ĺ���ռ�
*/
class NFMemHashMapCompact
{
public:
	struct HashMapIterator;
	struct HashMapLockIterator;

	/**
	* @brief ��������
	* string  _key      ����Key
	* string  _value    ����value
	* bool    _dirty    �Ƿ���������
	* uint32_t  _synct  sync time ��һ���������Ļ�дʱ��
	* uint32_t _expiret ���ݹ��ڵľ���ʱ��
	*                   �����û��������ʱ�ṩ��0��ʾ�����Ĵ�ʱ�� uint8_t
	* uint8_t _ver      ���ݰ汾��1Ϊ��ʼ�汾��0Ϊ����
	*/
	struct BlockData
	{
		std::string  _key;       /**����Key*/
		std::string  _value;     /**����value*/
		bool    _dirty;     /**�Ƿ���������*/
		uint32_t  _synct;   /**sync time, ��һ���������Ļ�дʱ��*/
		uint32_t _expiret;    /**���ݹ��ڵľ���ʱ�䣬�����û��������ʱ�ṩ��0��ʾ�����Ĵ�ʱ��*/
		uint8_t    _ver;        /**���ݰ汾��1Ϊ��ʼ�汾��0Ϊ����*/
		BlockData()
			: _dirty(false)
			, _synct(0)
			, _expiret(0)
			, _ver(1)
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
			uint16_t    _iSize;         /**block��������С*/
			uint32_t    _iIndex;        /**hash������*/
			uint32_t    _iBlockNext;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			uint32_t    _iBlockPrev;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			uint32_t    _iSetNext;      /**Set���ϵ���һ��Block*/
			uint32_t    _iSetPrev;      /**Set���ϵ���һ��Block*/
			uint32_t    _iGetNext;      /**Get���ϵ���һ��Block*/
			uint32_t    _iGetPrev;      /**Get���ϵ���һ��Block*/
			uint32_t    _iSyncTime;     /**�ϴλ�дʱ��*/
			uint32_t    _iExpireTime;    /** ���ݹ��ڵľ���ʱ�䣬�����û��������ʱ�ṩ��0��ʾ�����Ĵ�ʱ��*/
			uint8_t        _iVersion;        /** ���ݰ汾��1Ϊ��ʼ�汾��0Ϊ����*/
			bool        _bDirty;        /**�Ƿ���������*/
			bool        _bOnlyKey;      /**�Ƿ�ֻ��key, û������*/
			bool        _bNextChunk;    /**�Ƿ�����һ��chunk*/
			union
			{
				uint32_t  _iNextChunk;    /**��һ��Chunk��, _bNextChunk=trueʱ��Ч, tagChunkHead*/
				uint32_t  _iDataLen;      /**��ǰ���ݿ���ʹ���˵ĳ���, _bNextChunk=falseʱ��Ч*/
			};
			char        _cData[0];      /**���ݿ�ʼ����*/
		};

		/**
		* @brief ��ͷ����block, ��Ϊchunk
		*/
		struct tagChunkHead
		{
			uint16_t    _iSize;         /**block��������С*/
			bool        _bNextChunk;    /**�Ƿ�����һ��chunk*/
			union
			{
				uint32_t  _iNextChunk;    /**��һ�����ݿ�, _bNextChunk=trueʱ��Ч, tagChunkHead*/
				uint32_t  _iDataLen;      /**��ǰ���ݿ���ʹ���˵ĳ���, _bNextChunk=falseʱ��Ч*/
			};
			char        _cData[0];      /**���ݿ�ʼ����*/
		};
#pragma pack(pop)
#else
		/**
		* @brief block����ͷ
		*/
		struct tagBlockHead
		{
			uint16_t    _iSize;         /**block��������С*/
			uint32_t    _iIndex;        /**hash������*/
			uint32_t    _iBlockNext;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			uint32_t    _iBlockPrev;    /**��һ��Block,tagBlockHead, û����Ϊ0*/
			uint32_t    _iSetNext;      /**Set���ϵ���һ��Block*/
			uint32_t    _iSetPrev;      /**Set���ϵ���һ��Block*/
			uint32_t    _iGetNext;      /**Get���ϵ���һ��Block*/
			uint32_t    _iGetPrev;      /**Get���ϵ���һ��Block*/
			uint32_t    _iSyncTime;     /**�ϴλ�дʱ��*/
			uint32_t    _iExpireTime;    /** ���ݹ��ڵľ���ʱ�䣬�����û��������ʱ�ṩ��0��ʾ�����Ĵ�ʱ��*/
			uint8_t        _iVersion;        /** ���ݰ汾��1Ϊ��ʼ�汾��0Ϊ����*/
			bool        _bDirty;        /**�Ƿ���������*/
			bool        _bOnlyKey;      /**�Ƿ�ֻ��key, û������*/
			bool        _bNextChunk;    /**�Ƿ�����һ��chunk*/
			union
			{
				uint32_t  _iNextChunk;    /**��һ��Chunk��, _bNextChunk=trueʱ��Ч, tagChunkHead*/
				uint32_t  _iDataLen;      /**��ǰ���ݿ���ʹ���˵ĳ���, _bNextChunk=falseʱ��Ч*/
			};
			char        _cData[0];      /**���ݿ�ʼ����*/
		}__attribute__((packed));

		/**
		* @brief ��ͷ����block, ��Ϊchunk
		*/
		struct tagChunkHead
		{
			uint16_t    _iSize;         /**block��������С*/
			bool        _bNextChunk;    /**�Ƿ�����һ��chunk*/
			union
			{
				uint32_t  _iNextChunk;    /**��һ�����ݿ�, _bNextChunk=trueʱ��Ч, tagChunkHead*/
				uint32_t  _iDataLen;      /**��ǰ���ݿ���ʹ���˵ĳ���, _bNextChunk=falseʱ��Ч*/
			};
			char        _cData[0];      /**���ݿ�ʼ����*/
		}__attribute__((packed));
#endif

		/**
		* @brief ���캯��
		* @param Map
		* @param ��ǰMemBlock�ĵ�ַ
		* @param pAdd
		*/
		Block(NFMemHashMapCompact *pMap, uint32_t iAddr)
			: _pMap(pMap)
			, _iHead(iAddr)
		{
			_pHead = getBlockHead(_iHead);
		}

		/**
		* @brief ��������
		* @param mb
		*/
		Block(const Block &mb)
			: _pMap(mb._pMap)
			, _iHead(mb._iHead)
		{
			_pHead = getBlockHead(_iHead);
		}

		/**
		* @brief ��ȡblockͷ���Ե�ַ
		* @param iAddr
		*
		* @return tagChunkHead*
		*/
		tagBlockHead *getBlockHead(uint32_t iAddr) { return ((tagBlockHead*)_pMap->getAbsolute(iAddr)); }

		/**
		* @brief ��ȡMemBlockͷ��ַ
		*
		* @return void*
		*/
		tagBlockHead *getBlockHead() { return _pHead; }

		/**
		* @brief ͷ��
		*
		* @return uint32_t
		*/
		uint32_t getHead() { return _iHead; }

		/**
		* @brief ��ǰͰ�������һ��block��ͷ��
		*
		* @return uint32_t
		*/
		uint32_t getLastBlockHead();

		/**
		* @brief ��ȡ���ݵĹ���ʱ��
		*
		* @return uint32_t����λΪ�룬����0��ʾ�޹���ʱ��
		*/
		uint32_t getExpireTime() { return getBlockHead()->_iExpireTime; }

		/**
		* @brief �������ݵĹ���ʱ��
		* @param iExpireTime�����ھ���ʱ�䣬��λΪ��
		*/
		void setExpireTime(uint32_t iExpireTime)
		{
			if (iExpireTime != 0)
			{
				getBlockHead()->_iExpireTime = iExpireTime;
			}
		}

		/**
		* @brief ����Getʱ��
		*
		* @return uint32_t
		*/
		uint32_t getSyncTime() { return getBlockHead()->_iSyncTime; }

		/**
		* @brief ���û�дʱ��
		* @param iSyncTime
		*/
		void setSyncTime(uint32_t iSyncTime) { getBlockHead()->_iSyncTime = iSyncTime; }

		/**
		* @brief ��ȡ���ݰ汾
		*/
		uint8_t getVersion() { return getBlockHead()->_iVersion; }

		/**
		* @brief �������ݰ汾
		*/
		void setVersion(uint8_t iVersion) { getBlockHead()->_iVersion = iVersion; }

		/**
		* @brief ��ȡBlock�е�����
		*
		* @return int
		*          TC_HashMapCompact::RT_OK, ����, �����쳣
		*          TC_HashMapCompact::RT_ONLY_KEY, ֻ��Key
		*          �����쳣
		*/
		int getBlockData(NFMemHashMapCompact::BlockData &data);

		/**
		* @brief ��ȡ����
		* @param pData
		* @param iDatalen
		* @return int,
		*          TC_HashMapCompact::RT_OK, ����
		*          �����쳣
		*/
		int get(void *pData, uint32_t &iDataLen);

		/**
		* @brief ��ȡ����
		* @param s
		* @return int
		*          TC_HashMapCompact::RT_OK, ����
		*          �����쳣
		*/
		int get(std::string &s);

		/**
		* @brief ��������
		* @param pData
		* @param iDatalen
		* @param vtData, ��̭������
		*/
		int set(const std::string& k, const std::string& v, uint32_t iExpireTime, uint8_t iVersion, bool bNewBlock, bool bOnlyKey, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief �Ƿ���������
		*
		* @return bool
		*/
		bool isDirty() { return getBlockHead()->_bDirty; }

		/**
		* @brief ��������
		* @param b
		*/
		void setDirty(bool b);

		/**
		* @brief  �Ƿ�ֻ��key
		*
		* @return bool
		*/
		bool isOnlyKey() { return getBlockHead()->_bOnlyKey; }

		/**
		* @brief ��ǰԪ���ƶ�����һ��block
		* @return true, �Ƶ���һ��block��, false, û����һ��block
		*
		*/
		bool nextBlock();

		/**
		* @brief ��ǰԪ���ƶ�����һ��block
		* @return true, �Ƶ���һ��block��, false, û����һ��block
		*
		*/
		bool prevBlock();

		/**
		* @brief �ͷ�block�����пռ�
		*/
		void deallocate();

		/**
		* @brief ��blockʱ���øú���������һ���µ�block
		* @param index, hash����
		* @param iAllocSize, �ڴ��С
		*/
		void makeNew(uint32_t index, uint32_t iAllocSize);

		/**
		* @brief ��Block������ɾ����ǰBlock��ֻ��Block��Ч,
		*        ��Chunk����Ч��
		* @return
		*/
		void erase();

		/**
		* @brief ˢ��set����, ����Set����ͷ��
		*/
		void refreshSetList();

		/**
		* @brief ˢ��get����, ����Get����ͷ��
		*/
		void refreshGetList();

	protected:

		Block& operator=(const Block &mb);
		bool operator==(const Block &mb) const;
		bool operator!=(const Block &mb) const;

		/**
		* @brief ��ȡChunkͷ���Ե�ַ
		*
		* @return tagChunkHead*
		*/
		tagChunkHead *getChunkHead() { return getChunkHead(_iHead); }

		/**
		* @brief ��ȡchunkͷ���Ե�ַ
		* @param iAddr
		*
		* @return tagChunkHead*
		*/
		tagChunkHead *getChunkHead(uint32_t iAddr) { return ((tagChunkHead*)_pMap->getAbsolute(iAddr)); }

		/**
		* @brief �ӵ�ǰ��chunk��ʼ�ͷ�
		* @param iChunk �ͷŵ�ַ
		*/
		void deallocate(uint32_t iChunk);

		/**
		* @brief ���뵽hashmap������
		*/
		void insertHashMap();

		/**
		* @brief ���������������, ��������chunk, ��Ӱ��ԭ������
		*        ʹ�����ӵ�����������iDataLen���ͷŶ����chunk
		* @param iDataLen
		*
		* @return int,
		*/
		int allocate(uint32_t iDataLen, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief �ҽ�chunk, ���core��ҽ�ʧ��, ��֤�ڴ�黹������
		* @param pChunk
		* @param chunks
		*
		* @return int
		*/
		int joinChunk(tagChunkHead *pChunk, const std::vector<uint32_t> chunks);

		/**
		* @brief ����n��chunk��ַ��
		*        ע���ͷ��ڴ��ʱ�����ͷ����ڷ���Ķ���
		* @param fn      ����ռ��С
		* @param chunks  ����ɹ����ص�chunks��ַ�б�
		* @param vtData  ��̭������
		* @return        int
		*/
		int allocateChunk(uint32_t fn, std::vector<uint32_t> &chunks, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief ��ȡ���ݳ���
		*
		* @return uint32_t
		*/
		uint32_t getDataLen();

	private:

		/**
		* Map
		*/
		NFMemHashMapCompact   *_pMap;

		/**
		* block�����׵�ַ, ��Ե�ַ
		*/
		uint32_t            _iHead;

		/**
		* ��ַ
		*/
		tagBlockHead *      _pHead;
	};

	////////////////////////////////////////////////////////////////////////
	/**
	* @brief �ڴ����ݿ������
	*/
	class BlockAllocator
	{
	public:

		/**
		* @brief ���캯��
		*/
		BlockAllocator(NFMemHashMapCompact *pMap)
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
		* @param iMinBlockSize, ��С���ݿ��С
		* @param iMaxBlockSize, ������ݿ��С
		* @param fFactor, ����
		*/
		void create(void *pHeadAddr, size_t iSize, uint32_t iMinBlockSize, uint32_t iMaxBlockSize, float fFactor)
		{
			_pChunkAllocator->create(pHeadAddr, iSize, iMinBlockSize, iMaxBlockSize, fFactor);
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
		* @brief �ڴ��С
		*
		* @return size_t
		*/
		size_t getMemSize() const { return _pChunkAllocator->getMemSize(); }

		/**
		* @brief ��������������
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
		* @brief ����block��chunk����
		*
		* @return size_t
		*/
		size_t allBlockChunkCount() const { return _pChunkAllocator->allBlockChunkCount(); }

		/**
		* @brief ���ڴ��з���һ���µ�Block
		*
		* @param index, block hash����
		* @param iAllocSize: in/��Ҫ����Ĵ�С, out/����Ŀ��С
		* @param vtData, �����ͷŵ��ڴ������
		* @return size_t, ��Ե�ַ,0��ʾû�пռ���Է���
		*/
		uint32_t allocateMemBlock(uint32_t index, uint32_t &iAllocSize, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief Ϊ��ַΪiAddr��Block����һ��chunk
		*
		* @param iAddr,�����Block�ĵ�ַ
		* @param iAllocSize, in/��Ҫ����Ĵ�С, out/����Ŀ��С
		* @param vtData �����ͷŵ��ڴ������
		* @return size_t, ��Ե�ַ,0��ʾû�пռ���Է���
		*/
		uint32_t allocateChunk(uint32_t iAddr, uint32_t &iAllocSize, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief �ͷ�Block
		* @param v
		*/
		void deallocateMemBlock(const std::vector<uint32_t> &v);

		/**
		* @brief �ͷ�Block
		* @param v
		*/
		void deallocateMemBlock(uint32_t v);

	protected:
		/**
		*@brief ������copy����
		* */
		BlockAllocator(const BlockAllocator &);
		//������ֵ
		BlockAllocator& operator=(const BlockAllocator &);
		bool operator==(const BlockAllocator &mba) const;
		bool operator!=(const BlockAllocator &mba) const;

	public:
		/**
		* map
		*/
		NFMemHashMapCompact           *_pMap;

		/**
		* chunk������
		*/
		NFMemMultiChunkAllocator   *_pChunkAllocator;
	};

	////////////////////////////////////////////////////////////////
	/**
	*@brief  map��������
	* */
	class HashMapLockItem
	{
	public:

		/**
		*
		* @param pMap
		* @param iAddr
		*/
		HashMapLockItem(NFMemHashMapCompact *pMap, uint32_t iAddr);

		/**
		*
		* @param mcmdi
		*/
		HashMapLockItem(const HashMapLockItem &mcmdi);

		/**
		*
		* @param mcmdi
		*
		* @return HashMapLockItem&
		*/
		HashMapLockItem &operator=(const HashMapLockItem &mcmdi);

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator==(const HashMapLockItem &mcmdi);

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator!=(const HashMapLockItem &mcmdi);

		/**
		* @brief �Ƿ���������
		*
		* @return bool
		*/
		bool isDirty();

		/**
		* @brief �Ƿ�ֻ��Key
		*
		* @return bool
		*/
		bool isOnlyKey();

		/**
		* @brief ���Syncʱ��
		*
		* @return uint32_t
		*/
		uint32_t getSyncTime();

		/**
		* @brief ��ȡֵ, ���ֻ��Key(isOnlyKey)�������, vΪ��
		* @return int
		*          RT_OK:���ݻ�ȡOK
		*          RT_ONLY_KEY: key��Ч, v��ЧΪ��
		*          ����ֵ, �쳣
		*
		*/
		int get(std::string& k, std::string& v);

		/**
		* @brief ��ȡֵ
		* @return int
		*          RT_OK:���ݻ�ȡOK
		*          ����ֵ, �쳣
		*/
		int get(std::string& k);

		/**
		* @brief ���ݿ���Ե�ַ
		*
		* @return uint32_t
		*/
		uint32_t getAddr() const { return _iAddr; }

	protected:

		/**
		* @brief ��������
		* @param k
		* @param v
		* @param iExpiretime, ���ݹ���ʱ�䣬��λΪ�룬0��ʾ�����ù���ʱ��
		* @param iVersion, ���ݰ汾, Ӧ�ø���get�������ݰ汾д�أ�Ϊ0��ʾ���������ݰ汾
		* @param vtData, ��̭������
		* @return int
		*/
		int set(const std::string& k, const std::string& v, uint32_t iExpireTime, uint8_t iVersion, bool bNewBlock, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief ����Key, ������
		* @param k
		* @param vtData
		*
		* @return int
		*/
		int set(const std::string& k, uint8_t iVersion, bool bNewBlock, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		*
		* @param pKey
		* @param iKeyLen
		*
		* @return bool
		*/
		bool equal(const std::string &k, std::string &v, int &ret);

		/**
		*
		* @param pKey
		* @param iKeyLen
		*
		* @return bool
		*/
		bool equal(const std::string& k, int &ret);

		/**
		* @brief ��һ��item
		*
		* @return HashMapLockItem
		*/
		void nextItem(int iType);

		/**
		* @brief ��һ��item
		* @param iType
		*/
		void prevItem(int iType);

		friend class NFMemHashMapCompact;
		friend struct NFMemHashMapCompact::HashMapLockIterator;

	private:
		/**
		* map
		*/
		NFMemHashMapCompact *_pMap;

		/**
		* block�ĵ�ַ
		*/
		uint32_t          _iAddr;
	};

	/////////////////////////////////////////////////////////////////////////
	/**
	* @brief ���������
	* */
	struct HashMapLockIterator
	{
	public:

		/**
		*@brief ���������ʽ
		* */
		enum
		{
			IT_BLOCK = 0,        /**��ͨ��˳��*/
			IT_SET = 1,        /**Setʱ��˳��*/
			IT_GET = 2,        /**Getʱ��˳��*/
		};

		/**
		* @brief ��������˳��
		*/
		enum
		{
			IT_NEXT = 0,        /**˳��*/
			IT_PREV = 1,        /**����*/
		};

		/**
		*
		*/
		HashMapLockIterator();

		/**
		* @brief ���캯��
		* @param iAddr, ��ַ
		* @param type
		*/
		HashMapLockIterator(NFMemHashMapCompact *pMap, uint32_t iAddr, int iType, int iOrder);

		/**
		* @brief copy
		* @param it
		*/
		HashMapLockIterator(const HashMapLockIterator &it);

		/**
		* @brief ����
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
		* @brief ǰ��++
		*
		* @return HashMapLockIterator&
		*/
		HashMapLockIterator& operator++();

		/**
		* @brief ����++
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
		NFMemHashMapCompact  *_pMap;

		/**
		*
		*/
		HashMapLockItem _iItem;

		/**
		* �������ķ�ʽ
		*/
		int        _iType;

		/**
		* ��������˳��
		*/
		int        _iOrder;

	};

	////////////////////////////////////////////////////////////////
	// map��HashItem��, һ��HashItem��Ӧ���������
	class HashMapItem
	{
	public:

		/**
		*
		* @param pMap
		* @param iIndex
		*/
		HashMapItem(NFMemHashMapCompact *pMap, uint32_t iIndex);

		/**
		*
		* @param mcmdi
		*/
		HashMapItem(const HashMapItem &mcmdi);

		/**
		*
		* @param mcmdi
		*
		* @return HashMapItem&
		*/
		HashMapItem &operator=(const HashMapItem &mcmdi);

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator==(const HashMapItem &mcmdi);

		/**
		*
		* @param mcmdi
		*
		* @return bool
		*/
		bool operator!=(const HashMapItem &mcmdi);

		/**
		* @brief ��ȡ��ǰhashͰ����������, ע��ֻ��ȡ��key/value������,
		*        ����ֻ��key������, ����ȡ
		*
		* @return
		*/
		void get(std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		* @brief ��ȡ��ǰhashͰ�Ĺ�������, ע��ֻ��ȡ��key/value������,
		*        ����ֻ��key������, ����ȡ
		*
		* @return
		*/
		void getExpire(uint32_t t, std::vector<NFMemHashMapCompact::BlockData> &vtData);

		/**
		*
		*
		* @return uint32_t
		*/
		uint32_t getIndex() const { return _iIndex; }

		/**
		* @brief ��һ��item
		*
		*/
		void nextItem();

		/**
		* ���õ�ǰhashͰ����������Ϊ�����ݣ�ע��ֻ������key/value������
		* ����ֻ��key������, ������
		* @param
		* @return int
		*/
		int setDirty();

		friend class NFMemHashMapCompact;
		friend struct NFMemHashMapCompact::HashMapIterator;

	private:
		/**
		* map
		*/
		NFMemHashMapCompact *_pMap;

		/**
		* ���ݿ��ַ
		*/
		uint32_t     _iIndex;
	};

	/////////////////////////////////////////////////////////////////////////

	/**
	* @brief ���������
	*/
	struct HashMapIterator
	{
	public:

		/**
		* @brief ���캯��
		*/
		HashMapIterator();

		/**
		* @brief ���캯��
		* @param iIndex, ��ַ
		* @param type
		*/
		HashMapIterator(NFMemHashMapCompact *pMap, uint32_t iIndex);

		/**
		* @brief ��������
		* @param it
		*/
		HashMapIterator(const HashMapIterator &it);

		/**
		* @brief ����
		* @param it
		*
		* @return HashMapLockIterator&
		*/
		HashMapIterator& operator=(const HashMapIterator &it);

		/**
		*
		* @param mcmi
		*
		* @return bool
		*/
		bool operator==(const HashMapIterator& mcmi);

		/**
		*
		* @param mv
		*
		* @return bool
		*/
		bool operator!=(const HashMapIterator& mcmi);

		/**
		* @brief ǰ��++
		*
		* @return HashMapIterator&
		*/
		HashMapIterator& operator++();

		/**
		* @brief ����++
		*
		* @return HashMapIterator&
		*/
		HashMapIterator operator++(int);

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
		NFMemHashMapCompact  *_pMap;

		/**
		*
		*/
		HashMapItem _iItem;
	};

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  pack (push,1)
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* mapͷ
	*/
	struct tagMapHead
	{
		char        _cMaxVersion;        //��汾
		char        _cMinVersion;        //С�汾
		bool        _bReadOnly;          //�Ƿ�ֻ��
		bool        _bAutoErase;         //�Ƿ�����Զ���̭
		char        _cEraseMode;         //��̭��ʽ:0x00:����Get����̭, 0x01:����Set����̭
		size_t      _iMemSize;           //�ڴ��С
		uint32_t    _iMinDataSize;       //��С���ݿ��С
		uint32_t    _iMaxDataSize;       //������ݿ��С
		float       _fFactor;            //����
		float       _fRadio;             //chunks����/hash����
		uint32_t    _iElementCount;      //��Ԫ�ظ���
		uint32_t    _iEraseCount;        //ÿ��ɾ������
		uint32_t    _iDirtyCount;        //�����ݸ���
		uint32_t    _iSetHead;           //Setʱ������ͷ��
		uint32_t    _iSetTail;           //Setʱ������β��
		uint32_t    _iGetHead;           //Getʱ������ͷ��
		uint32_t    _iGetTail;           //Getʱ������β��
		uint32_t    _iDirtyTail;         //��������β��
		uint32_t    _iSyncTime;          //��дʱ��
		uint32_t    _iUsedChunk;         //�Ѿ�ʹ�õ��ڴ��
		uint32_t    _iGetCount;          //get����
		uint32_t    _iHitCount;          //���д���
		uint32_t    _iBackupTail;        //�ȱ�ָ��
		uint32_t    _iSyncTail;          //��д����
		uint32_t    _iOnlyKeyCount;        // OnlyKey����
		bool         _bInit;                 //�Ƿ��Ѿ���ɳ�ʼ��
		char        _cReserve[15];          //����
											//uint32_t    _iReserve[4];       //����
	};

	/**
	* ��Ҫ�޸ĵĵ�ַ
	*/
	struct tagModifyData
	{
		size_t  _iModifyAddr;       /**�޸ĵĵ�ַ*/
		char    _cBytes;            /**�ֽ���*/
		size_t  _iModifyValue;      /**ֵ*/
	};

	/**
	* �޸����ݿ�ͷ��
	*/
	struct tagModifyHead
	{
		char            _cModifyStatus;         /**�޸�״̬: 0:Ŀǰû�����޸�, 1: ��ʼ׼���޸�, 2:�޸����, û��copy���ڴ���*/
		uint32_t        _iNowIndex;             /**���µ�Ŀǰ������, ���ܲ���10��*/
		tagModifyData   _stModifyData[50000];     /**һ�����50000���޸�*/
	};

	/**
	* HashItem
	*/
	struct tagHashItem
	{
		uint32_t _iBlockAddr;     /**ָ���������ƫ�Ƶ�ַ*/
		uint32_t _iListCount;     /**�������*/
	};
#pragma pack(pop)
#else
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* mapͷ
	*/
	struct tagMapHead
	{
		char        _cMaxVersion;        //��汾
		char        _cMinVersion;        //С�汾
		bool        _bReadOnly;          //�Ƿ�ֻ��
		bool        _bAutoErase;         //�Ƿ�����Զ���̭
		char        _cEraseMode;         //��̭��ʽ:0x00:����Get����̭, 0x01:����Set����̭
		size_t      _iMemSize;           //�ڴ��С
		uint32_t    _iMinDataSize;       //��С���ݿ��С
		uint32_t    _iMaxDataSize;       //������ݿ��С
		float       _fFactor;            //����
		float       _fRadio;             //chunks����/hash����
		uint32_t    _iElementCount;      //��Ԫ�ظ���
		uint32_t    _iEraseCount;        //ÿ��ɾ������
		uint32_t    _iDirtyCount;        //�����ݸ���
		uint32_t    _iSetHead;           //Setʱ������ͷ��
		uint32_t    _iSetTail;           //Setʱ������β��
		uint32_t    _iGetHead;           //Getʱ������ͷ��
		uint32_t    _iGetTail;           //Getʱ������β��
		uint32_t    _iDirtyTail;         //��������β��
		uint32_t    _iSyncTime;          //��дʱ��
		uint32_t    _iUsedChunk;         //�Ѿ�ʹ�õ��ڴ��
		uint32_t    _iGetCount;          //get����
		uint32_t    _iHitCount;          //���д���
		uint32_t    _iBackupTail;        //�ȱ�ָ��
		uint32_t    _iSyncTail;          //��д����
		uint32_t    _iOnlyKeyCount;        // OnlyKey����
		bool         _bInit;                 //�Ƿ��Ѿ���ɳ�ʼ��
		char        _cReserve[15];          //����
											//uint32_t    _iReserve[4];       //����
	}__attribute__((packed));

	/**
	* ��Ҫ�޸ĵĵ�ַ
	*/
	struct tagModifyData
	{
		size_t  _iModifyAddr;       /**�޸ĵĵ�ַ*/
		char    _cBytes;            /**�ֽ���*/
		size_t  _iModifyValue;      /**ֵ*/
	}__attribute__((packed));

	/**
	* �޸����ݿ�ͷ��
	*/
	struct tagModifyHead
	{
		char            _cModifyStatus;         /**�޸�״̬: 0:Ŀǰû�����޸�, 1: ��ʼ׼���޸�, 2:�޸����, û��copy���ڴ���*/
		uint32_t        _iNowIndex;             /**���µ�Ŀǰ������, ���ܲ���10��*/
		tagModifyData   _stModifyData[50000];     /**һ�����50000���޸�*/
	}__attribute__((packed));

	/**
	* HashItem
	*/
	struct tagHashItem
	{
		uint32_t _iBlockAddr;     /**ָ���������ƫ�Ƶ�ַ*/
		uint32_t _iListCount;     /**�������*/
	}__attribute__((packed));
#endif

	/**
	* @brief 64λ����ϵͳ�û����汾��, 32λ����ϵͳ��64λ�汾��
	*  */
	//64λ����ϵͳ�û����汾��, 32λ����ϵͳ��ż���汾��
#if __WORDSIZE == 64

	/**
	* @brief ����汾��
	*/
	enum
	{
		MAX_VERSION = 4,    //��ǰmap�Ĵ�汾��
		MIN_VERSION = 1,    //��ǰmap��С�汾��
	};

#else
	/**
	* @brief ����汾��
	*/
	enum
	{
		MAX_VERSION = 4,    /**��ǰmap�Ĵ�汾��*/
		MIN_VERSION = 0,    /**��ǰmap��С�汾��*/
	};

#endif

	/**
	* @brief ������̭��ʽ
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
		RT_DATA_VER_MISMATCH = 11,   /**д�����ݰ汾��ƥ��*/
		RT_DECODE_ERR = -1,   /**��������*/
		RT_EXCEPTION_ERR = -2,   /**�쳣*/
		RT_LOAD_DATA_ERR = -3,   /**���������쳣*/
		RT_VERSION_MISMATCH_ERR = -4,   /**�汾��һ��*/
		RT_DUMP_FILE_ERR = -5,   /**dump���ļ�ʧ��*/
		RT_LOAL_FILE_ERR = -6,   /**load�ļ����ڴ�ʧ��*/
		RT_NOTALL_ERR = -7,   /**û�и�����ȫ*/
	};

	/**
	* @brief  ���������
	*/
	typedef HashMapIterator     hash_iterator;
	typedef HashMapLockIterator lock_iterator;

	/**
	* @brief ����hash������
	*/
	using hash_functor = std::function<size_t(const std::string&)>;

	//////////////////////////////////////////////////////////////////////////////////////////////
	//map�Ľӿڶ���

	/**
	* @brief ���캯��
	*/
	NFMemHashMapCompact()
		: _iMinDataSize(0)
		, _iMaxDataSize(0)
		, _fFactor(1.0)
		, _fRadio(2)
		, _pDataAllocator(new BlockAllocator(this))
		, _lock_end(this, 0, 0, 0)
		, _end(this, (uint32_t)(-1))
		, _hashf(NFHash::hash_new<std::string>())
	{
	}

	/**
	* @brief ��ʼ�����ݿ�ƽ����С
	* ��ʾ�ڴ�����ʱ�򣬻����n����С�飬 n������С��*�������ӣ�, n������С��*��������*�������ӣ�..., ֱ��n������
	* n��hashmap�Լ����������
	* ���ַ������ͨ�������ݿ��¼�䳤�Ƚ϶��ʹ�ã� ���ڽ�Լ�ڴ棬������ݼ�¼�������Ǳ䳤�ģ� ����С��=���죬��������=1�Ϳ�����
	* @param iMinDataSize: ��С���ݿ��С
	* @param iMaxDataSize: ������ݿ��С
	* @param fFactor: ��������
	*/
	void initDataBlockSize(uint32_t iMinDataSize, uint32_t iMaxDataSize, float fFactor);

	/**
	* @brief ʼ��chunk���ݿ�/hash���ֵ, Ĭ����2,
	*        ����Ҫ���ı�����create֮ǰ����
	*
	* @param fRadio
	*/
	void initHashRadio(float fRadio) { _fRadio = fRadio; }

	/**
	* @brief ��ʼ��, ֮ǰ��Ҫ����:initDataAvgSize��initHashRadio
	* @param pAddr ���Ե�ַ
	* @param iSize ��С
	* @return ʧ�����׳��쳣
	*/
	void create(void *pAddr, size_t iSize);

	/**
	* ���ӵ��ڴ��
	* @param pAddr, ��ַ
	* @param iSize, �ڴ��С
	* @return ʧ�����׳��쳣
	*/
	void connect(void *pAddr, size_t iSize);

	/**
	* @brief ԭ�������ݿ��������չ�ڴ�, ע��ͨ��ֻ�ܶ�mmap�ļ���Ч
	* (���iSize�ȱ������ڴ��С,�򷵻�-1)
	* @param pAddr, ��չ��Ŀռ�
	* @param iSize
	* @return 0:�ɹ�, -1:ʧ��
	*/
	int append(void *pAddr, size_t iSize);

	/**
	* @brief ��ȡÿ�ִ�С�ڴ���ͷ����Ϣ
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
	* @brief ��ȡhashͰ�ĸ���
	*
	* @return size_t
	*/
	uint32_t getHashCount() { return _hash.size(); }

	/**
	* @brief Ԫ�صĸ���
	*
	* @return size_t
	*/
	uint32_t size() { return _pHead->_iElementCount; }

	/**
	* @brief ������Ԫ�ظ���
	*
	* @return uint32_t
	*/
	uint32_t dirtyCount() { return _pHead->_iDirtyCount; }

	/**
	* @brief OnlyKey����Ԫ�ظ���
	*
	* @return uint32_t
	*/
	uint32_t onlyKeyCount() { return _pHead->_iOnlyKeyCount; }

	/**
	* @brief ����ÿ����̭����
	* @param n
	*/
	void setEraseCount(uint32_t n) { _pHead->_iEraseCount = n; }

	/**
	* @brief ��ȡÿ����̭����
	*
	* @return uint32_t
	*/
	uint32_t getEraseCount() { return _pHead->_iEraseCount; }

	/**
	* @brief ����ֻ��
	* @param bReadOnly
	*/
	void setReadOnly(bool bReadOnly) { _pHead->_bReadOnly = bReadOnly; }

	/**
	* @brief �Ƿ�ֻ��
	*
	* @return bool
	*/
	bool isReadOnly() { return _pHead->_bReadOnly; }

	/**
	* @brief �����Ƿ�����Զ���̭
	* @param bAutoErase
	*/
	void setAutoErase(bool bAutoErase) { _pHead->_bAutoErase = bAutoErase; }

	/**
	* @brief �Ƿ�����Զ���̭
	*
	* @return bool
	*/
	bool isAutoErase() { return _pHead->_bAutoErase; }

	/**
	* @brief ������̭��ʽ
	* TC_HashMapCompact::ERASEBYGET
	* TC_HashMapCompact::ERASEBYSET
	* @param cEraseMode
	*/
	void setEraseMode(char cEraseMode) { _pHead->_cEraseMode = cEraseMode; }

	/**
	* @brief ��ȡ��̭��ʽ
	*
	* @return bool
	*/
	char getEraseMode() { return _pHead->_cEraseMode; }

	/**
	* @brief ���û�дʱ��(��)
	* @param iSyncTime
	*/
	void setSyncTime(uint32_t iSyncTime) { _pHead->_iSyncTime = iSyncTime; }

	/**
	* @brief ��ȡ��дʱ��
	*
	* @return uint32_t
	*/
	uint32_t getSyncTime() { return _pHead->_iSyncTime; }

	/**
	* @brief ��ȡͷ��������Ϣ
	*
	* @return tagMapHead&
	*/
	tagMapHead& getMapHead() { return *_pHead; }

	/**
	* @brief ����hash��ʽ
	* @param hash_of
	*/
	void setHashFunctor(hash_functor hashf) { _hashf = hashf; }

	/**
	* @brief ����hash������
	*
	* @return hash_functor&
	*/
	hash_functor &getHashFunctor() { return _hashf; }

	/**
	* @brief hash item
	* @param index
	*
	* @return tagHashItem&
	*/
	tagHashItem *item(uint32_t iIndex) { return &_hash[iIndex]; }

	/**
	* dump���ļ�
	* @param sFile
	*
	* @return int
	*          RT_DUMP_FILE_ERR: dump���ļ�����
	*          RT_OK: dump���ļ��ɹ�
	*/
	int dump2file(const std::string &sFile);

	/**
	* @brief ���ļ�load
	* @param sFile
	*
	* @return int
	*          RT_LOAL_FILE_ERR: load����
	*          RT_VERSION_MISMATCH_ERR: �汾��һ��
	*          RT_OK: load�ɹ�
	*/
	int load5file(const std::string &sFile);

	/**
	* @brief �޸�hash����Ϊi��hash��(i���ܲ���hashmap������ֵ)
	* @param i
	* @param bRepair
	*
	* @return int
	*/
	int recover(size_t i, bool bRepair);

	/**
	* @brief ���hashmap������map�����ݻָ�����ʼ״̬
	*/
	void clear();

	/**
	* @brief ������ݸɾ�״̬
	* @param k
	*
	* @return int
	*          RT_NO_DATA: û�е�ǰ����
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_DIRTY_DATA: ��������
	*          RT_OK: �Ǹɾ�����
	*          ��������ֵ: ����
	*/
	int checkDirty(const std::string &k);

	/**
	* @brief ����Ϊ������, �޸�SETʱ����, �ᵼ�����ݻ�д
	* @param k
	*
	* @return int
	*          RT_READONLY: ֻ��
	*          RT_NO_DATA: û�е�ǰ����
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_OK: ���������ݳɹ�
	*          ��������ֵ: ����
	*/
	int setDirty(const std::string& k);

	/**
	* ���ݻ�дʧ�ܺ���������Ϊ������
	* @param k
	*
	* @return int
	*          RT_READONLY: ֻ��
	*          RT_NO_DATA: û�е�ǰ����
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_OK: ���������ݳɹ�
	*          ��������ֵ: ����
	*/
	int setDirtyAfterSync(const std::string& k);

	/**
	* @brief ����Ϊ�ɾ�����, �޸�SET��, �������ݲ���д
	* @param k
	*
	* @return int
	*          RT_READONLY: ֻ��
	*          RT_NO_DATA: û�е�ǰ����
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_OK: ���óɹ�
	*          ��������ֵ: ����
	*/
	int setClean(const std::string& k);

	/**
	* @brief ��ȡ����, �޸�GETʱ����
	* @param k
	* @param v
	* @param iSyncTime:�����ϴλ�д��ʱ��
	* @param iExpiretime: ���ݹ���ʱ�䣬��λΪ�룬0��ʾ�����ù���ʱ��
	* @param iVersion: ���ݰ汾, Ӧ�ø���get�������ݰ汾д�أ�Ϊ0��ʾ���������ݰ汾
	*
	* @return int:
	*          RT_NO_DATA: û������
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_OK:��ȡ���ݳɹ�
	*          ��������ֵ: ����
	*/
	int get(const std::string& k, std::string &v, uint32_t &iSyncTime, uint32_t& iExpireTime, uint8_t& iVersion);

	/**
	* @brief ��ȡ����, �޸�GETʱ����
	* @param k
	* @param v
	* @param iSyncTime:�����ϴλ�д��ʱ��
	*
	* @return int:
	*          RT_NO_DATA: û������
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_OK:��ȡ���ݳɹ�
	*          ��������ֵ: ����
	*/
	int get(const std::string& k, std::string &v, uint32_t &iSyncTime);

	/**
	* @brief ��ȡ����, �޸�GETʱ����
	* @param k
	* @param v
	*
	* @return int:
	*          RT_NO_DATA: û������
	*          RT_ONLY_KEY:ֻ��Key
	*          RT_OK:��ȡ���ݳɹ�
	*          ��������ֵ: ����
	*/
	int get(const std::string& k, std::string &v);

	/**
	* @brief ��������, �޸�ʱ����, �ڴ治��ʱ���Զ���̭�ϵ�����
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
	int set(const std::string& k, const std::string& v, bool bDirty, std::vector<BlockData> &vtData);

	/**
	* @brief ��������, �޸�ʱ����, �ڴ治��ʱ���Զ���̭�ϵ�����
	* @param k            �ؼ���
	* @param v            ֵ
	* @param iExpiretime ���ݹ���ʱ�䣬��λΪ�룬
	*                    0��ʾ�����ù���ʱ��
	* @param iVersion    ���ݰ汾,
	*                    Ӧ�ø���get�������ݰ汾д�أ�Ϊ0��ʾ���������ݰ� ��
	* @param bDirty     �Ƿ���������
	* @param vtData     ����̭�ļ�¼
	* @return           RT_READONLY: mapֻ��
	*                   RT_NO_MEMORY:û�пռ�(����̭��������»���֣�
	*                   RT_OK: ���óɹ� ��������ֵ: ����
	*/
	int set(const std::string& k, const std::string& v, uint32_t iExpireTime, uint8_t iVersion, bool bDirty, std::vector<BlockData> &vtData);

	/**
	* @brief ����key, ��������
	* @param k
	* @param vtData
	*
	* @return  RT_READONLY: mapֻ��
	*          RT_NO_MEMORY: û�пռ�(����̭��������»����)
	*          RT_OK: ���óɹ�
	*          ��������ֵ: ����
	*/
	int set(const std::string& k, std::vector<BlockData> &vtData);

	/**
	* @brief ����key, ��������
	* @param k
	* @param iVersion: ���ݰ汾, Ӧ�ø���get�������ݰ汾д�أ�Ϊ0��ʾ���������ݰ汾
	* @param vtData
	*
	* @return int
	*          RT_READONLY: mapֻ��
	*          RT_NO_MEMORY: û�пռ�(����̭��������»����)
	*          RT_OK: ���óɹ�
	*          ��������ֵ: ����
	*/
	int set(const std::string& k, uint8_t iVersion, std::vector<BlockData>& vtData);

	/**
	* @brief ɾ������
	* @param k, �ؼ���
	* @param data, ��ɾ���ļ�¼
	* @return int:
	*          RT_READONLY: mapֻ��
	*          RT_NO_DATA: û�е�ǰ����
	*          RT_ONLY_KEY:ֻ��Key, ɾ���ɹ�
	*          RT_OK: ɾ�����ݳɹ�
	*         ��������ֵ: ����
	*/
	int del(const std::string& k, BlockData &data);

	/**
	* @brief ��̭����, ÿ��ɾ��һ��,
	* ����Getʱ����̭���ⲿѭ�����øýӿ���̭���� ֱ��:
	* Ԫ�ظ���/chunks * 100 < radio, bCheckDirty
	* Ϊtrueʱ����������������̭����
	* @param radio �����ڴ�chunksʹ�ñ��� 0< radio < 100
	* @param data  ��ǰ��ɾ����һ����¼
	* @return  RT_READONLY: mapֻ��
	*          RT_OK: �����ټ�����̭��
	*          RT_ONLY_KEY:ֻ��Key, ɾ���ɹ�
	*          RT_DIRTY_DATA:�����������ݣ���bCheckDirty=trueʱ���п��ܲ������ַ���ֵ
	*          RT_ERASE_OK:��̭��ǰ���ݳɹ�, ������̭
	*          ��������ֵ: ����, ͨ������, ��������erase��̭
	*/
	int erase(int radio, BlockData &data, bool bCheckDirty = false);

	/**
	* @brief ��д, ÿ�η�����Ҫ��д��һ��
	* ���ݻ�дʱ���뵱ǰʱ�䳬��_pHead->_iSyncTime����Ҫ��д
	* _pHead->_iSyncTime��setSyncTime�����趨, Ĭ��10����
	* �ⲿѭ�����øú������л�д
	* mapֻ��ʱ��Ȼ���Ի�д
	* @param iNowTime: ��ǰʱ��
	*                  ��дʱ���뵱ǰʱ�����_pHead->_iSyncTime����Ҫ��д
	* @param data : ��д������
	* @return int:
	*          RT_OK: ������������ͷ����, ����sleepһ���ٳ���
	*          RT_ONLY_KEY:ֻ��Key, ɾ���ɹ�, ��ǰ���ݲ�Ҫ��д,��������sync��д
	*          RT_NEED_SYNC:��ǰ���ص�data������Ҫ��д
	*          RT_NONEED_SYNC:��ǰ���ص�data���ݲ���Ҫ��д
	*          ��������ֵ: ����, ͨ������, ��������sync��д
	*/
	int sync(uint32_t iNowTime, BlockData &data);

	/**
	* @brief ��ʼ��д, ������дָ��
	*/
	void sync();

	/**
	* @brief ��ʼ����֮ǰ���øú���
	*
	* @param bForceFromBegin: �Ƿ�ǿ����ͷ��ʼ����
	* @return void
	*/
	void backup(bool bForceFromBegin = false);

	/**
	* @brief ��ʼ��������, ÿ�η�����Ҫ���ݵ�һ������
	* @param data
	*
	* @return int
	*          RT_OK: �������
	*          RT_NEED_BACKUP:��ǰ���ص�data������Ҫ����
	*          RT_ONLY_KEY:ֻ��Key, ��ǰ���ݲ�Ҫ����
	*          ��������ֵ: ����, ͨ������, ��������backup
	*/
	int backup(BlockData &data);

	/////////////////////////////////////////////////////////////////////////////////////////
	// �����Ǳ���map����, ��Ҫ��map����

	/**
	*@brief  ����
	*
	* @return
	*/
	lock_iterator end() { return _lock_end; }


	/**
	* @brief ����Key��������
	* @param string
	*/
	lock_iterator find(const std::string& k);

	/**
	* @brief block����
	*
	* @return lock_iterator
	*/
	lock_iterator begin();

	/**
	*@brief  block����
	*
	* @return lock_iterator
	*/
	lock_iterator rbegin();

	/**
	* @brief ��Setʱ������ĵ�����
	*
	* @return lock_iterator
	*/
	lock_iterator beginSetTime();

	/**
	* @brief Set������ĵ�����
	*
	* @return lock_iterator
	*/
	lock_iterator rbeginSetTime();

	/**
	* @brief ��Getʱ������ĵ�����
	*
	* @return lock_iterator
	*/
	lock_iterator beginGetTime();

	/**
	* @brief Get������ĵ�����
	*
	* @return lock_iterator
	*/
	lock_iterator rbeginGetTime();

	/**
	* @brief ��ȡ������β��������(�ʱ��û�в�����������)��
	* ���صĵ�����++��ʾ����ʱ��˳��==>(���ʱ��û�в�����������)
	*
	* @return lock_iterator
	*/
	lock_iterator beginDirty();

	/////////////////////////////////////////////////////////////////////////////////////////
	// �����Ǳ���map����, ����Ҫ��map����

	/**
	* @brief ����hashͰ����
	*
	* @return hash_iterator
	*/
	hash_iterator hashBegin();

	/**
	* @brief ����
	*
	* @return
	*/
	hash_iterator hashEnd() { return _end; }

	/**
	* @brief ����
	*
	* @return string
	*/
	std::string desc();

public:

	/**
	* @brief ���󱣻�
	*/
	struct FailureRecover
	{
		FailureRecover(NFMemHashMapCompact *pMap) : _pMap(pMap)
		{
			_pMap->doRecover();
		}

		~FailureRecover()
		{
			_pMap->doUpdate();
		}

	protected:
		NFMemHashMapCompact   *_pMap;
	};

protected:

	friend class Block;
	friend class BlockAllocator;
	friend class HashMapIterator;
	friend class HashMapItem;
	friend class HashMapLockIterator;
	friend class HashMapLockItem;

	/**
	* @brief ��ֹcopy����
	*  */
	NFMemHashMapCompact(const NFMemHashMapCompact &mcm);
	/**
	* @brief ��ֹ����
	*  */
	NFMemHashMapCompact &operator=(const NFMemHashMapCompact &mcm);

	/**
	* @brief ��ʼ��
	* @param pAddr
	*/
	void init(void *pAddr);


	/**
	* @brief ���������ݸ���
	*/
	void incDirtyCount() { saveValue(&_pHead->_iDirtyCount, _pHead->_iDirtyCount + 1); }

	/**
	* @brief ���������ݸ���
	*/
	void delDirtyCount() { saveValue(&_pHead->_iDirtyCount, _pHead->_iDirtyCount - 1); }

	/**
	* @brief �������ݸ���
	*/
	void incElementCount() { saveValue(&_pHead->_iElementCount, _pHead->_iElementCount + 1); }

	/**
	* @brief �������ݸ���
	*/
	void delElementCount() { saveValue(&_pHead->_iElementCount, _pHead->_iElementCount - 1); }

	/**
	* @brief ����OnlyKey���ݸ���
	*/
	void incOnlyKeyCount() { saveValue(&_pHead->_iOnlyKeyCount, _pHead->_iOnlyKeyCount + 1); }

	/**
	* @brief ����OnlyKey���ݸ���
	*/
	void delOnlyKeyCount() { saveValue(&_pHead->_iOnlyKeyCount, _pHead->_iOnlyKeyCount - 1); }

	/*
	* @brief ����Chunk��
	* ��ѡ���Ƿ��ֱ�Ӹ���, ��Ϊ�п���һ�η����chunk����
	* �������������ڴ�ռ�, ����Խ�����
	*/
	void incChunkCount(bool bUpdateDirect = true)
	{
		if (bUpdateDirect)
		{
			++_pHead->_iUsedChunk;
		}
		else
			saveValue(&_pHead->_iUsedChunk, _pHead->_iUsedChunk + 1);
	}

	/**
	* @brief ����Chunk��
	* ����ѡ���Ƿ�ֱ�Ӹ���, ��Ϊ�п���һ���ͷŵ�chunk����
	* �������������ڴ�ռ�, ����Խ�����
	*/
	void delChunkCount(bool bUpdateDirect = true)
	{
		if (bUpdateDirect)
		{
			--_pHead->_iUsedChunk;
		}
		else
			saveValue(&_pHead->_iUsedChunk, _pHead->_iUsedChunk - 1);
	}

	/**
	* @brief ����hit����
	*/
	void incGetCount() { saveValue(&_pHead->_iGetCount, _pHead->_iGetCount + 1); }

	/**
	* @brief �������д���
	*/
	void incHitCount() { saveValue(&_pHead->_iHitCount, _pHead->_iHitCount + 1); }

	/**
	* @brief ĳhash�������ݸ���+1
	* @param index
	*/
	void incListCount(uint32_t index) { saveValue(&item(index)->_iListCount, item(index)->_iListCount + 1); }

	/**
	* @brief ĳhash�������ݸ���+1
	* @param index
	*/
	void delListCount(uint32_t index) { saveValue(&item(index)->_iListCount, item(index)->_iListCount - 1); }

	/**
	* @brief ��Ե�ַ���ɾ��Ե�ַ
	* @param iAddr
	*
	* @return void*
	*/
	void *getAbsolute(uint32_t iAddr) { if (iAddr == 0) return NULL; return _pDataAllocator->_pChunkAllocator->getAbsolute(iAddr); }

	/**
	* @brief ��̭iNowAddr֮�������(������̭������̭)
	* @param iNowAddr, ��ǰBlock�������ڷ���ռ�, ���ܱ���̭
	*                  0��ʾ��ֱ�Ӹ�����̭������̭
	* @param vector<BlockData>, ����̭������
	* @return size_t,��̭�����ݸ���
	*/
	uint32_t eraseExcept(uint32_t iNowAddr, std::vector<BlockData> &vtData);

	/**
	* @brief ����Key����hashֵ
	* @param pKey
	* @param iKeyLen
	*
	* @return size_t
	*/
	uint32_t hashIndex(const std::string& k);

	/**
	* @brief ����Key��������
	*
	*/
	lock_iterator find(const std::string& k, uint32_t index, std::string &v, int &ret);

	/**
	* @brief ����Key��������
	* @param mb
	*/
	lock_iterator find(const std::string& k, uint32_t index, int &ret);

	/**
	* @brief ����hash������
	* @param iMaxHash
	* @param iMinHash
	* @param fAvgHash
	*/
	void analyseHash(uint32_t &iMaxHash, uint32_t &iMinHash, float &fAvgHash);

	/**
	* @brief �޸ľ����ֵ
	* @param iModifyAddr
	* @param iModifyValue
	*/
	template<typename T>
	void saveValue(void* iModifyAddr, T iModifyValue, bool bModify = true)
	{
		//��ȡԭʼֵ
		T tmp = *(T*)iModifyAddr;

		//����ԭʼֵ
		_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._iModifyAddr = (char*)iModifyAddr - (char*)_pHead;
		_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._iModifyValue = tmp;
		_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._cBytes = sizeof(iModifyValue);
		_pstModifyHead->_cModifyStatus = 1;

		_pstModifyHead->_iNowIndex++;

		if (bModify)
		{
			//�޸ľ���ֵ
			*(T*)iModifyAddr = iModifyValue;
		}

		assert(_pstModifyHead->_iNowIndex < sizeof(_pstModifyHead->_stModifyData) / sizeof(tagModifyData));
	}

	void saveAddr(uint32_t iAddr, char cByte = 0);

	/**
	* @brief �ָ�����
	*/
	void doRecover();

	/**
	* @brief ȷ�ϴ������
	*/
	void doUpdate();

	/**
	* ȷ�ϴ������
	*/
	void doUpdate2();

	/**
	* ȷ�ϴ������
	*/
	void doUpdate3();

	/**
	* ȷ�ϴ������
	*/
	void doUpdate4();

	/**
	* @brief ��ȡ����n����n���������
	* @param n
	*
	* @return size_t
	*/
	uint32_t getMinPrimeNumber(uint32_t n);

	/**
	* �ͷű������е�chunk
	*/
	void deallocate(uint32_t iChunk, uint32_t iIndex);

	/**
	* �ͷű������е�һ��block
	*/
	void deallocate2(uint32_t iHead);

protected:

	/**
	* ����ָ��
	*/
	tagMapHead                  *_pHead;

	/**
	* ��С�����ݿ��С
	*/
	uint32_t                    _iMinDataSize;

	/**
	* �������ݿ��С
	*/
	uint32_t                    _iMaxDataSize;

	/**
	* �仯����
	*/
	float                       _fFactor;

	/**
	* ����chunk���ݿ�/hash���ֵ
	*/
	float                       _fRadio;

	/**
	* hash����
	*/
	NFMemVector<tagHashItem>   _hash;

	/**
	* �޸����ݿ�
	*/
	tagModifyHead               *_pstModifyHead;

	/**
	* block����������
	*/
	BlockAllocator              *_pDataAllocator;

	/**
	* β��
	*/
	lock_iterator               _lock_end;

	/**
	* β��
	*/
	hash_iterator               _end;

	/**
	* hashֵ���㹫ʽ
	*/
	hash_functor                _hashf;
};

