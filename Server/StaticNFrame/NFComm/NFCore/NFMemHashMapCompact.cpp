// -------------------------------------------------------------------------
//    @FileName         :    NFHashMapCompact.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFMemHashMapCompact.h"
#include "NFCommon.h"

int NFMemHashMapCompact::Block::getBlockData(NFMemHashMapCompact::BlockData &data)
{
	data._dirty = isDirty();
	data._synct = getSyncTime();
	data._expiret = getExpireTime();
	data._ver = getVersion();

	std::string s;
	int ret = get(s);

	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	try
	{
		TC_PackOut po(s.c_str(), s.length());
		po >> data._key;

		//�������ֻ��Key
		if (!isOnlyKey())
		{
			po >> data._value;
		}
		else
		{
			return NFMemHashMapCompact::RT_ONLY_KEY;
		}
	}
	catch (std::exception &ex)
	{
		ret = NFMemHashMapCompact::RT_DECODE_ERR;
	}

	return ret;
}

uint32_t NFMemHashMapCompact::Block::getLastBlockHead()
{
	uint32_t iHead = _iHead;

	while (getBlockHead(iHead)->_iBlockNext != 0)
	{
		iHead = getBlockHead(iHead)->_iBlockNext;
	}
	return iHead;
}

int NFMemHashMapCompact::Block::get(void *pData, uint32_t &iDataLen)
{
	//û����һ��chunk, һ��chunk�Ϳ���װ��������
	if (!getBlockHead()->_bNextChunk)
	{
		memcpy(pData, getBlockHead()->_cData, min(getBlockHead()->_iDataLen, iDataLen));
		iDataLen = getBlockHead()->_iDataLen;
		return NFMemHashMapCompact::RT_OK;
	}
	else
	{
		uint32_t iUseSize = getBlockHead()->_iSize - sizeof(tagBlockHead);
		uint32_t iCopyLen = min(iUseSize, iDataLen);

		//copy����ǰ��block��
		memcpy(pData, getBlockHead()->_cData, iCopyLen);
		if (iDataLen < iUseSize)
		{
			return NFMemHashMapCompact::RT_NOTALL_ERR;   //copy���ݲ���ȫ
		}

		//�Ѿ�copy����
		uint32_t iHasLen = iCopyLen;
		//���ʣ�೤��
		uint32_t iLeftLen = iDataLen - iCopyLen;

		tagChunkHead *pChunk = getChunkHead(getBlockHead()->_iNextChunk);
		while (iHasLen < iDataLen)
		{
			iUseSize = pChunk->_iSize - sizeof(tagChunkHead);
			if (!pChunk->_bNextChunk)
			{
				//copy����ǰ��chunk��
				uint32_t iCopyLen = min(pChunk->_iDataLen, iLeftLen);
				memcpy((char*)pData + iHasLen, pChunk->_cData, iCopyLen);
				iDataLen = iHasLen + iCopyLen;

				if (iLeftLen < pChunk->_iDataLen)
				{
					return NFMemHashMapCompact::RT_NOTALL_ERR;       //copy����ȫ
				}

				return NFMemHashMapCompact::RT_OK;
			}
			else
			{
				uint32_t iCopyLen = min(iUseSize, iLeftLen);
				//copy��ǰ��chunk
				memcpy((char*)pData + iHasLen, pChunk->_cData, iCopyLen);
				if (iLeftLen <= iUseSize)
				{
					iDataLen = iHasLen + iCopyLen;
					return NFMemHashMapCompact::RT_NOTALL_ERR;   //copy����ȫ
				}

				//copy��ǰchunk��ȫ
				iHasLen += iCopyLen;
				iLeftLen -= iCopyLen;

				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
		}
	}

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::Block::get(std::string &s)
{
	uint32_t iLen = getDataLen();

	char *cData = new char[iLen];
	uint32_t iGetLen = iLen;
	int ret = get(cData, iGetLen);
	if (ret == NFMemHashMapCompact::RT_OK)
	{
		s.assign(cData, iGetLen);
	}

	delete[] cData;

	return ret;
}

int NFMemHashMapCompact::Block::set(const std::string& k, const std::string& v, uint32_t iExpireTime, uint8_t iVersion, bool bNewBlock, bool bOnlyKey, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	if (iVersion != 0 && getBlockHead()->_iVersion != iVersion)
	{
		// ���ݰ汾��ƥ��
		return NFMemHashMapCompact::RT_DATA_VER_MISMATCH;
	}

	TC_PackIn pi;
	pi << k;

	if (!bOnlyKey)
	{
		pi << v;
	}

	const char *pData = pi.topacket().c_str();

	uint32_t iDataLen = pi.topacket().length();

	//���ȷ���ոչ��ĳ���, ���ܶ�һ��chunk, Ҳ������һ��chunk
	int ret = allocate(iDataLen, vtData);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	if (bOnlyKey)
	{
		//ԭʼ������������
		if (getBlockHead()->_bDirty)
		{
			_pMap->delDirtyCount();
		}

		//���ݱ��޸�, ����Ϊ������
		getBlockHead()->_bDirty = false;

		//ԭʼ���ݲ���OnlyKey����
		if (!getBlockHead()->_bOnlyKey)
		{
			_pMap->incOnlyKeyCount();
		}
	}
	else
	{
		//ԭʼ���ݲ���������
		if (!getBlockHead()->_bDirty)
		{
			_pMap->incDirtyCount();
		}

		//���ݱ��޸�, ����Ϊ������
		getBlockHead()->_bDirty = true;

		//ԭʼ������OnlyKey����
		if (getBlockHead()->_bOnlyKey)
		{
			_pMap->delOnlyKeyCount();
		}

		// ���ù���ʱ��
		if (iExpireTime != 0)
		{
			setExpireTime(iExpireTime);
		}

		// �������ݰ汾
		iVersion = getBlockHead()->_iVersion;
		iVersion++;
		if (iVersion == 0)
		{
			// 0�Ǳ����汾����Ч�汾��Χ��1-255
			iVersion = 1;
		}
		getBlockHead()->_iVersion = iVersion;
	}

	//�����Ƿ�ֻ��Key
	getBlockHead()->_bOnlyKey = bOnlyKey;

	uint32_t iUseSize = getBlockHead()->_iSize - sizeof(tagBlockHead);
	//û����һ��chunk, һ��chunk�Ϳ���װ��������
	if (!getBlockHead()->_bNextChunk)
	{
		memcpy(getBlockHead()->_cData, (char*)pData, iDataLen);
		//��copy����, �ٸ������ݳ���
		getBlockHead()->_iDataLen = iDataLen;
	}
	else
	{
		//copy����ǰ��block��
		memcpy(getBlockHead()->_cData, (char*)pData, iUseSize);
		//ʣ��̶�
		uint32_t iLeftLen = iDataLen - iUseSize;
		uint32_t iCopyLen = iUseSize;

		tagChunkHead *pChunk = getChunkHead(getBlockHead()->_iNextChunk);
		while (true)
		{
			//����chunk�Ŀ��ô�С
			iUseSize = pChunk->_iSize - sizeof(tagChunkHead);

			if (!pChunk->_bNextChunk)
			{
				assert(iUseSize >= iLeftLen);
				//copy����ǰ��chunk��
				memcpy(pChunk->_cData, (char*)pData + iCopyLen, iLeftLen);
				//���һ��chunk, �������ݳ���, ��copy�����ٸ�ֵ����
				pChunk->_iDataLen = iLeftLen;
				iCopyLen += iLeftLen;
				iLeftLen -= iLeftLen;
				break;
			}
			else
			{
				//copy����ǰ��chunk��
				memcpy(pChunk->_cData, (char*)pData + iCopyLen, iUseSize);
				iCopyLen += iUseSize;
				iLeftLen -= iUseSize;

				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
		}
		assert(iLeftLen == 0);
	}

	return NFMemHashMapCompact::RT_OK;
}

void NFMemHashMapCompact::Block::setDirty(bool b)
{
	if (getBlockHead()->_bDirty != b)
	{
		if (b)
		{
			_pMap->incDirtyCount();
		}
		else
		{
			_pMap->delDirtyCount();
		}
		_pMap->saveValue(&getBlockHead()->_bDirty, b);
	}
}

bool NFMemHashMapCompact::Block::nextBlock()
{
	_iHead = getBlockHead()->_iBlockNext;
	_pHead = getBlockHead(_iHead);

	return _iHead != 0;
}

bool NFMemHashMapCompact::Block::prevBlock()
{
	_iHead = getBlockHead()->_iBlockPrev;
	_pHead = getBlockHead(_iHead);

	return _iHead != 0;
}

void NFMemHashMapCompact::Block::deallocate()
{

	if (getBlockHead()->_bNextChunk)
	{
		deallocate(getBlockHead()->_iNextChunk);
	}

	_pMap->_pDataAllocator->deallocateMemBlock(_iHead);
	//_pMap->_pDataAllocator->deallocateMemBlock(v);
}

void NFMemHashMapCompact::Block::insertHashMap()
{
	uint32_t index = getBlockHead()->_iIndex;

	_pMap->incDirtyCount();
	_pMap->incElementCount();
	_pMap->incListCount(index);

	//����block������
	if (_pMap->item(index)->_iBlockAddr == 0)
	{
		//��ǰhashͰû��Ԫ��
		_pMap->saveValue(&_pMap->item(index)->_iBlockAddr, _iHead);
		_pMap->saveValue(&getBlockHead()->_iBlockNext, (uint32_t)0);
		_pMap->saveValue(&getBlockHead()->_iBlockPrev, (uint32_t)0);
	}
	else
	{
		//��ȻhashͰ��Ԫ��, ����Ͱ��ͷ
		_pMap->saveValue(&getBlockHead(_pMap->item(index)->_iBlockAddr)->_iBlockPrev, _iHead);
		_pMap->saveValue(&getBlockHead()->_iBlockNext, _pMap->item(index)->_iBlockAddr);
		_pMap->saveValue(&_pMap->item(index)->_iBlockAddr, _iHead);
		_pMap->saveValue(&getBlockHead()->_iBlockPrev, (uint32_t)0);
	}

	//����Set�����ͷ��
	if (_pMap->_pHead->_iSetHead == 0)
	{
		assert(_pMap->_pHead->_iSetTail == 0);
		_pMap->saveValue(&_pMap->_pHead->_iSetHead, _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iSetTail, _iHead);
	}
	else
	{
		assert(_pMap->_pHead->_iSetTail != 0);
		_pMap->saveValue(&getBlockHead()->_iSetNext, _pMap->_pHead->_iSetHead);
		_pMap->saveValue(&getBlockHead(_pMap->_pHead->_iSetHead)->_iSetPrev, _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iSetHead, _iHead);
	}

	//����Get����ͷ��
	if (_pMap->_pHead->_iGetHead == 0)
	{
		assert(_pMap->_pHead->_iGetTail == 0);
		_pMap->saveValue(&_pMap->_pHead->_iGetHead, _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iGetTail, _iHead);
	}
	else
	{
		assert(_pMap->_pHead->_iGetTail != 0);
		_pMap->saveValue(&getBlockHead()->_iGetNext, _pMap->_pHead->_iGetHead);
		_pMap->saveValue(&getBlockHead(_pMap->_pHead->_iGetHead)->_iGetPrev, _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iGetHead, _iHead);
	}

	_pMap->doUpdate();
}

void NFMemHashMapCompact::Block::makeNew(uint32_t index, uint32_t iAllocSize)
{
	getBlockHead()->_iSize = iAllocSize;
	getBlockHead()->_iIndex = index;
	getBlockHead()->_iSetNext = 0;
	getBlockHead()->_iSetPrev = 0;
	getBlockHead()->_iGetNext = 0;
	getBlockHead()->_iGetPrev = 0;
	getBlockHead()->_iSyncTime = 0;
	getBlockHead()->_iExpireTime = 0;
	getBlockHead()->_iVersion = 1;
	getBlockHead()->_iBlockNext = 0;
	getBlockHead()->_iBlockPrev = 0;
	getBlockHead()->_bNextChunk = false;
	getBlockHead()->_iDataLen = 0;
	getBlockHead()->_bDirty = true;
	getBlockHead()->_bOnlyKey = false;

	//���뵽������
	insertHashMap();
}

void NFMemHashMapCompact::Block::erase()
{
	//////////////////�޸�����������/////////////
	if (_pMap->_pHead->_iDirtyTail == _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iDirtyTail, getBlockHead()->_iSetPrev);
	}

	//////////////////�޸Ļ�д��������/////////////
	if (_pMap->_pHead->_iSyncTail == _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iSyncTail, getBlockHead()->_iSetPrev);
	}

	//////////////////�޸ı�����������/////////////
	if (_pMap->_pHead->_iBackupTail == _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iBackupTail, getBlockHead()->_iGetPrev);
	}

	////////////////////�޸�Set���������//////////
	{
		bool bHead = (_pMap->_pHead->_iSetHead == _iHead);
		bool bTail = (_pMap->_pHead->_iSetTail == _iHead);

		if (!bHead)
		{
			assert(getBlockHead()->_iSetPrev != 0);
			if (bTail)
			{
				assert(getBlockHead()->_iSetNext == 0);
				//��β��, β��ָ��ָ����һ��Ԫ��
				_pMap->saveValue(&_pMap->_pHead->_iSetTail, getBlockHead()->_iSetPrev);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetPrev)->_iSetNext, (uint32_t)0);
			}
			else
			{
				//����ͷ��Ҳ����β��
				assert(getBlockHead()->_iSetNext != 0);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetPrev)->_iSetNext, getBlockHead()->_iSetNext);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetNext)->_iSetPrev, getBlockHead()->_iSetPrev);
			}
		}
		else
		{
			assert(getBlockHead()->_iSetPrev == 0);
			if (bTail)
			{
				assert(getBlockHead()->_iSetNext == 0);
				//ͷ��Ҳ��β��, ָ�붼����Ϊ0
				_pMap->saveValue(&_pMap->_pHead->_iSetHead, (uint32_t)0);
				_pMap->saveValue(&_pMap->_pHead->_iSetTail, (uint32_t)0);
			}
			else
			{
				//ͷ������β��, ͷ��ָ��ָ����һ��Ԫ��
				assert(getBlockHead()->_iSetNext != 0);
				_pMap->saveValue(&_pMap->_pHead->_iSetHead, getBlockHead()->_iSetNext);
				//��һ��Ԫ����ָ��Ϊ0
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetNext)->_iSetPrev, (uint32_t)0);
			}
		}
	}

	////////////////////�޸�Get���������//////////
	//
	{
		bool bHead = (_pMap->_pHead->_iGetHead == _iHead);
		bool bTail = (_pMap->_pHead->_iGetTail == _iHead);

		if (!bHead)
		{
			assert(getBlockHead()->_iGetPrev != 0);
			if (bTail)
			{
				assert(getBlockHead()->_iGetNext == 0);
				//��β��, β��ָ��ָ����һ��Ԫ��
				_pMap->saveValue(&_pMap->_pHead->_iGetTail, getBlockHead()->_iGetPrev);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iGetPrev)->_iGetNext, (uint32_t)0);
			}
			else
			{
				//����ͷ��Ҳ����β��
				assert(getBlockHead()->_iGetNext != 0);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iGetPrev)->_iGetNext, getBlockHead()->_iGetNext);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iGetNext)->_iGetPrev, getBlockHead()->_iGetPrev);
			}
		}
		else
		{
			assert(getBlockHead()->_iGetPrev == 0);
			if (bTail)
			{
				assert(getBlockHead()->_iGetNext == 0);
				//ͷ��Ҳ��β��, ָ�붼����Ϊ0
				_pMap->saveValue(&_pMap->_pHead->_iGetHead, (uint32_t)0);
				_pMap->saveValue(&_pMap->_pHead->_iGetTail, (uint32_t)0);
			}
			else
			{
				//ͷ������β��, ͷ��ָ��ָ����һ��Ԫ��
				assert(getBlockHead()->_iGetNext != 0);
				_pMap->saveValue(&_pMap->_pHead->_iGetHead, getBlockHead()->_iGetNext);
				//��һ��Ԫ����ָ��Ϊ0
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iGetNext)->_iGetPrev, (uint32_t)0);
			}
		}
	}

	///////////////////��block������ȥ��///////////
	//
	//��һ��blockָ����һ��block
	if (getBlockHead()->_iBlockPrev != 0)
	{
		_pMap->saveValue(&getBlockHead(getBlockHead()->_iBlockPrev)->_iBlockNext, getBlockHead()->_iBlockNext);
	}

	//��һ��blockָ����һ��
	if (getBlockHead()->_iBlockNext != 0)
	{
		_pMap->saveValue(&getBlockHead(getBlockHead()->_iBlockNext)->_iBlockPrev, getBlockHead()->_iBlockPrev);
	}

	//////////////////�����hashͷ��, ��Ҫ�޸�hash��������ָ��//////
	//
	_pMap->delListCount(getBlockHead()->_iIndex);
	if (getBlockHead()->_iBlockPrev == 0)
	{
		//�����hashͰ��ͷ��, ����Ҫ����
		NFMemHashMapCompact::tagHashItem *pItem = _pMap->item(getBlockHead()->_iIndex);
		assert(pItem->_iBlockAddr == _iHead);
		if (pItem->_iBlockAddr == _iHead)
		{
			_pMap->saveValue(&pItem->_iBlockAddr, getBlockHead()->_iBlockNext);
		}
	}

	//////////////////������///////////////////
	//
	if (isDirty())
	{
		_pMap->delDirtyCount();
	}

	if (isOnlyKey())
	{
		_pMap->delOnlyKeyCount();
	}

	//Ԫ�ظ�������
	_pMap->delElementCount();

	_pMap->saveAddr(_iHead, -2);

	/*
	//����ͷ��ָ����ֳ�
	_pMap->saveValue(&getBlockHead()->_iSize, 0, false);
	_pMap->saveValue(&getBlockHead()->_iIndex, 0, false);
	_pMap->saveValue(&getBlockHead()->_iBlockNext, 0, false);
	_pMap->saveValue(&getBlockHead()->_iBlockPrev, 0, false);
	_pMap->saveValue(&getBlockHead()->_iSetNext, 0, false);
	_pMap->saveValue(&getBlockHead()->_iSetPrev, 0, false);
	_pMap->saveValue(&getBlockHead()->_iGetNext, 0, false);
	_pMap->saveValue(&getBlockHead()->_iGetPrev, 0, false);
	_pMap->saveValue(&getBlockHead()->_iSyncTime, 0, false);
	_pMap->saveValue(&getBlockHead()->_bDirty, 0, false);
	_pMap->saveValue(&getBlockHead()->_bOnlyKey, 0, false);
	_pMap->saveValue(&getBlockHead()->_bNextChunk, 0, false);
	_pMap->saveValue(&getBlockHead()->_iNextChunk, 0, false);

	//ʹ�޸���Ч, ���ͷ��ڴ�֮ǰ��Ч, ��ʹ�黹�ڴ�ʧ��Ҳֻ�Ƕ������ڴ��
	_pMap->doUpdate();

	//�黹���ڴ���
	deallocate();
	*/

	//ʹ�޸���Ч�����ͷ��ڴ�֮���ڲ��ƻ�����map��ͬʱ���ܾ����ٵض�ʧ�ڴ��
	_pMap->doUpdate4();
}

void NFMemHashMapCompact::Block::refreshSetList()
{
	assert(_pMap->_pHead->_iSetHead != 0);
	assert(_pMap->_pHead->_iSetTail != 0);

	//�޸�ͬ������
	if (_pMap->_pHead->_iSyncTail == _iHead && _pMap->_pHead->_iSetHead != _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iSyncTail, getBlockHead()->_iSetPrev);
	}

	//�޸�������β������ָ��, ����һ��Ԫ��
	if (_pMap->_pHead->_iDirtyTail == _iHead && _pMap->_pHead->_iSetHead != _iHead)
	{
		//������β��λ��ǰ��
		_pMap->saveValue(&_pMap->_pHead->_iDirtyTail, getBlockHead()->_iSetPrev);
	}
	else if (_pMap->_pHead->_iDirtyTail == 0)
	{
		//ԭ��û��������
		_pMap->saveValue(&_pMap->_pHead->_iDirtyTail, _iHead);
	}

	//��ͷ�����ݻ���set������ʱ�ߵ������֧
	if (_pMap->_pHead->_iSetHead == _iHead)
	{
		//ˢ��Get��
		refreshGetList();
		return;
	}

	uint32_t iPrev = getBlockHead()->_iSetPrev;
	uint32_t iNext = getBlockHead()->_iSetNext;

	assert(iPrev != 0);

	//��������ͷ��
	_pMap->saveValue(&getBlockHead()->_iSetNext, _pMap->_pHead->_iSetHead);
	_pMap->saveValue(&getBlockHead(_pMap->_pHead->_iSetHead)->_iSetPrev, _iHead);
	_pMap->saveValue(&_pMap->_pHead->_iSetHead, _iHead);
	_pMap->saveValue(&getBlockHead()->_iSetPrev, (uint32_t)0);

	//��һ��Ԫ�ص�Nextָ��ָ����һ��Ԫ��
	_pMap->saveValue(&getBlockHead(iPrev)->_iSetNext, iNext);

	//��һ��Ԫ�ص�Prevָ����һ��Ԫ��
	if (iNext != 0)
	{
		_pMap->saveValue(&getBlockHead(iNext)->_iSetPrev, iPrev);
	}
	else
	{
		//�ı�β��ָ��
		assert(_pMap->_pHead->_iSetTail == _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iSetTail, iPrev);
	}

	//ˢ��Get��
	refreshGetList();
}

void NFMemHashMapCompact::Block::refreshGetList()
{
	assert(_pMap->_pHead->_iGetHead != 0);
	assert(_pMap->_pHead->_iGetTail != 0);

	//��ͷ������
	if (_pMap->_pHead->_iGetHead == _iHead)
	{
		return;
	}

	uint32_t iPrev = getBlockHead()->_iGetPrev;
	uint32_t iNext = getBlockHead()->_iGetNext;

	assert(iPrev != 0);

	//�����ڱ��ݵ�����
	if (_pMap->_pHead->_iBackupTail == _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iBackupTail, iPrev);
	}

	//��������ͷ��
	_pMap->saveValue(&getBlockHead()->_iGetNext, _pMap->_pHead->_iGetHead);
	_pMap->saveValue(&getBlockHead(_pMap->_pHead->_iGetHead)->_iGetPrev, _iHead);
	_pMap->saveValue(&_pMap->_pHead->_iGetHead, _iHead);
	_pMap->saveValue(&getBlockHead()->_iGetPrev, (uint32_t)0);

	//��һ��Ԫ�ص�Nextָ��ָ����һ��Ԫ��
	_pMap->saveValue(&getBlockHead(iPrev)->_iGetNext, iNext);

	//��һ��Ԫ�ص�Prevָ����һ��Ԫ��
	if (iNext != 0)
	{
		_pMap->saveValue(&getBlockHead(iNext)->_iGetPrev, iPrev);
	}
	else
	{
		//�ı�β��ָ��
		assert(_pMap->_pHead->_iGetTail == _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iGetTail, iPrev);
	}
}

void NFMemHashMapCompact::Block::deallocate(uint32_t iChunk)
{
	tagChunkHead *pChunk = getChunkHead(iChunk);
	std::vector<uint32_t> v;
	v.push_back(iChunk);

	//��ȡ���к�����chunk��ַ
	while (true)
	{
		if (pChunk->_bNextChunk)
		{
			v.push_back(pChunk->_iNextChunk);
			pChunk = getChunkHead(pChunk->_iNextChunk);
		}
		else
		{
			break;
		}
	}

	//�ռ�ȫ���ͷŵ�
	_pMap->_pDataAllocator->deallocateMemBlock(v);
}

int NFMemHashMapCompact::Block::allocate(uint32_t iDataLen, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	uint32_t fn = 0;

	//һ�������������������
	fn = getBlockHead()->_iSize - sizeof(tagBlockHead);
	if (fn >= iDataLen)
	{
		//һ��block�Ϳ�����, ������chunk��Ҫ�ͷŵ�
		if (getBlockHead()->_bNextChunk)
		{
			uint32_t iNextChunk = getBlockHead()->_iNextChunk;

			//�����һ��chunk�ĵ�ַ
			_pMap->saveAddr(iNextChunk, -1);
			//Ȼ���޸��Լ�������
			_pMap->saveValue(&getBlockHead()->_bNextChunk, false);
			_pMap->saveValue(&getBlockHead()->_iDataLen, (uint32_t)0);
			//�޸ĳɹ������ͷ�����, �Ӷ���֤, ����core��ʱ���������ڴ�鲻����
			_pMap->doUpdate3();
		}
		return NFMemHashMapCompact::RT_OK;
	}

	//���㻹��Ҫ���ٳ���
	fn = iDataLen - fn;

	if (getBlockHead()->_bNextChunk)
	{
		tagChunkHead *pChunk = getChunkHead(getBlockHead()->_iNextChunk);

		while (true)
		{
			if (fn <= (pChunk->_iSize - sizeof(tagChunkHead)))
			{
				//�Ѿ�����Ҫchunk��, �ͷŶ����chunk
				if (pChunk->_bNextChunk)
				{
					uint32_t iNextChunk = pChunk->_iNextChunk;

					//�����һ��chunk�ĵ�ַ
					_pMap->saveAddr(iNextChunk, -1);
					//Ȼ���޸��Լ�������
					_pMap->saveValue(&pChunk->_bNextChunk, false);
					_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);
					//�޸ĳɹ������ͷ�����, �Ӷ���֤, ����core��ʱ���������ڴ�鲻����
					_pMap->doUpdate3();
				}
				return NFMemHashMapCompact::RT_OK;
			}

			//����, ����Ҫ���ٳ���
			fn -= pChunk->_iSize - sizeof(tagChunkHead);

			if (pChunk->_bNextChunk)
			{
				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
			else
			{
				//û�к���chunk��, ��Ҫ�·���fn�Ŀռ�
				std::vector<uint32_t> chunks;
				int ret = allocateChunk(fn, chunks, vtData);
				if (ret != NFMemHashMapCompact::RT_OK)
				{
					//û���ڴ���Է���
					return ret;
				}

				_pMap->saveValue(&pChunk->_bNextChunk, true);
				_pMap->saveValue(&pChunk->_iNextChunk, chunks[0]);
				//chunkָ�����ĵ�һ��chunk
				pChunk = getChunkHead(chunks[0]);

				//�����һ��chunk����Ե�ַ����֤�����쳣��ֹʱ���ܹ��ͷž������chunk
				pChunk->_bNextChunk = false;
				pChunk->_iDataLen = (uint32_t)0;
				_pMap->saveAddr(chunks[0]);

				//����ÿ��chunk
				return joinChunk(pChunk, chunks);
			}
		}
	}
	else
	{
		//û�к���chunk��, ��Ҫ�·���fn�ռ�
		std::vector<uint32_t> chunks;
		int ret = allocateChunk(fn, chunks, vtData);
		if (ret != NFMemHashMapCompact::RT_OK)
		{
			//û���ڴ���Է���
			return ret;
		}

		_pMap->saveValue(&getBlockHead()->_bNextChunk, true);
		_pMap->saveValue(&getBlockHead()->_iNextChunk, chunks[0]);
		//chunkָ�����ĵ�һ��chunk
		tagChunkHead *pChunk = getChunkHead(chunks[0]);

		//�����һ��chunk����Ե�ַ����֤�����쳣��ֹʱ���ܹ��ͷž������chunk
		pChunk->_bNextChunk = false;
		pChunk->_iDataLen = (uint32_t)0;
		_pMap->saveAddr(chunks[0]);

		//����ÿ��chunk
		return joinChunk(pChunk, chunks);
	}

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::Block::joinChunk(tagChunkHead *pChunk, const std::vector<uint32_t> chunks)
{
	tagChunkHead* pNextChunk = NULL;
	for (size_t i = 0; i<chunks.size(); ++i)
	{
		if (i == chunks.size() - 1)
		{
			_pMap->saveValue(&pChunk->_bNextChunk, false);
		}
		else
		{
			pNextChunk = getChunkHead(chunks[i + 1]);
			pNextChunk->_bNextChunk = false;
			pNextChunk->_iDataLen = (uint32_t)0;

			_pMap->saveValue(&pChunk->_bNextChunk, true);
			_pMap->saveValue(&pChunk->_iNextChunk, chunks[i + 1]);
			if (i>0 && i % 10 == 0)
			{
				_pMap->doUpdate2();
			}

			pChunk = pNextChunk;
		}
	}

	_pMap->doUpdate2();

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::Block::allocateChunk(uint32_t fn, std::vector<uint32_t> &chunks, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	assert(fn > 0);

	while (true)
	{
		uint32_t iAllocSize = fn;
		//����ռ�
		uint32_t t = _pMap->_pDataAllocator->allocateChunk(_iHead, iAllocSize, vtData);
		if (t == 0)
		{
			//û���ڴ���Է�����, �Ȱѷ���Ĺ黹
			_pMap->_pDataAllocator->deallocateMemBlock(chunks);
			chunks.clear();
			return NFMemHashMapCompact::RT_NO_MEMORY;
		}

		//���÷�������ݿ�Ĵ�С
		getChunkHead(t)->_iSize = iAllocSize;

		chunks.push_back(t);

		//���乻��
		if (fn <= (iAllocSize - sizeof(tagChunkHead)))
		{
			break;
		}

		//����Ҫ���ٿռ�
		fn -= iAllocSize - sizeof(tagChunkHead);
	}

	return NFMemHashMapCompact::RT_OK;
}

uint32_t NFMemHashMapCompact::Block::getDataLen()
{
	uint32_t n = 0;
	if (!getBlockHead()->_bNextChunk)
	{
		n += getBlockHead()->_iDataLen;
		return n;
	}

	//��ǰ��Ĵ�С
	n += getBlockHead()->_iSize - sizeof(tagBlockHead);
	tagChunkHead *pChunk = getChunkHead(getBlockHead()->_iNextChunk);

	while (true)
	{
		if (pChunk->_bNextChunk)
		{
			//��ǰ��Ĵ�С
			n += pChunk->_iSize - sizeof(tagChunkHead);
			pChunk = getChunkHead(pChunk->_iNextChunk);
		}
		else
		{
			n += pChunk->_iDataLen;
			break;
		}
	}

	return n;
}

////////////////////////////////////////////////////////

uint32_t NFMemHashMapCompact::BlockAllocator::allocateMemBlock(uint32_t index, uint32_t &iAllocSize, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
begin:
	size_t iAddr;
	size_t iTmp1 = (size_t)iAllocSize;
	_pChunkAllocator->allocate2(iTmp1, iTmp1, iAddr);
	iAllocSize = (uint32_t)iTmp1;

	if (iAddr == 0)
	{
		uint32_t ret = _pMap->eraseExcept(0, vtData);
		if (ret == 0)
			return 0;     //û�пռ�����ͷ���
		goto begin;
	}

	_pMap->incChunkCount();

	//������µ�MemBlock, ��ʼ��һ��
	Block block(_pMap, (uint32_t)iAddr);
	block.makeNew(index, iAllocSize);

	return (uint32_t)iAddr;
}

uint32_t NFMemHashMapCompact::BlockAllocator::allocateChunk(uint32_t iAddr, uint32_t &iAllocSize, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
begin:
	size_t iIndex;

	size_t iTmp1 = (size_t)iAllocSize;
	_pChunkAllocator->allocate2(iTmp1, iTmp1, iIndex);
	iAllocSize = (uint32_t)iTmp1;

	if (iIndex == 0)
	{
		uint32_t ret = _pMap->eraseExcept(iAddr, vtData);
		if (ret == 0)
			return 0;     //û�пռ�����ͷ���
		goto begin;
	}

	_pMap->incChunkCount();

	return (uint32_t)iIndex;
}

void NFMemHashMapCompact::BlockAllocator::deallocateMemBlock(const std::vector<uint32_t> &v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		_pChunkAllocator->deallocate(_pMap->getAbsolute(v[i]));
		_pMap->delChunkCount();
	}
}

void NFMemHashMapCompact::BlockAllocator::deallocateMemBlock(uint32_t v)
{
	_pChunkAllocator->deallocate(_pMap->getAbsolute(v));
	_pMap->delChunkCount();
}

///////////////////////////////////////////////////////////////////

NFMemHashMapCompact::HashMapLockItem::HashMapLockItem(NFMemHashMapCompact *pMap, uint32_t iAddr)
	: _pMap(pMap)
	, _iAddr(iAddr)
{
}

NFMemHashMapCompact::HashMapLockItem::HashMapLockItem(const HashMapLockItem &mcmdi)
	: _pMap(mcmdi._pMap)
	, _iAddr(mcmdi._iAddr)
{
}

NFMemHashMapCompact::HashMapLockItem &NFMemHashMapCompact::HashMapLockItem::operator=(const NFMemHashMapCompact::HashMapLockItem &mcmdi)
{
	if (this != &mcmdi)
	{
		_pMap = mcmdi._pMap;
		_iAddr = mcmdi._iAddr;
	}
	return (*this);
}

bool NFMemHashMapCompact::HashMapLockItem::operator==(const NFMemHashMapCompact::HashMapLockItem &mcmdi)
{
	return _pMap == mcmdi._pMap && _iAddr == mcmdi._iAddr;
}

bool NFMemHashMapCompact::HashMapLockItem::operator!=(const NFMemHashMapCompact::HashMapLockItem &mcmdi)
{
	return _pMap != mcmdi._pMap || _iAddr != mcmdi._iAddr;
}

bool NFMemHashMapCompact::HashMapLockItem::isDirty()
{
	Block block(_pMap, _iAddr);
	return block.isDirty();
}

bool NFMemHashMapCompact::HashMapLockItem::isOnlyKey()
{
	Block block(_pMap, _iAddr);
	return block.isOnlyKey();
}

uint32_t NFMemHashMapCompact::HashMapLockItem::getSyncTime()
{
	Block block(_pMap, _iAddr);
	return block.getSyncTime();
}

int NFMemHashMapCompact::HashMapLockItem::get(std::string& k, std::string& v)
{
	std::string s;

	Block block(_pMap, _iAddr);

	int ret = block.get(s);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	try
	{
		TC_PackOut po(s.c_str(), s.length());
		po >> k;
		if (!block.isOnlyKey())
		{
			po >> v;
		}
		else
		{
			v = "";
			return NFMemHashMapCompact::RT_ONLY_KEY;
		}
	}
	catch (std::exception &ex)
	{
		return NFMemHashMapCompact::RT_EXCEPTION_ERR;
	}

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::HashMapLockItem::get(std::string& k)
{
	std::string s;

	Block block(_pMap, _iAddr);

	int ret = block.get(s);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	try
	{
		TC_PackOut po(s.c_str(), s.length());
		po >> k;
	}
	catch (std::exception &ex)
	{
		return NFMemHashMapCompact::RT_EXCEPTION_ERR;
	}

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::HashMapLockItem::set(const std::string& k, const std::string& v, uint32_t iExpireTime, uint8_t iVersion, bool bNewBlock, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	Block block(_pMap, _iAddr);

	return block.set(k, v, iExpireTime, iVersion, bNewBlock, false, vtData);
}


int NFMemHashMapCompact::HashMapLockItem::set(const std::string& k, uint8_t iVersion, bool bNewBlock, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	Block block(_pMap, _iAddr);

	return block.set(k, "", 0, iVersion, bNewBlock, true, vtData);
}

bool NFMemHashMapCompact::HashMapLockItem::equal(const std::string &k, std::string &v, int &ret)
{
	std::string k1;
	ret = get(k1, v);

	if ((ret == NFMemHashMapCompact::RT_OK || ret == NFMemHashMapCompact::RT_ONLY_KEY) && k == k1)
	{
		return true;
	}

	return false;
}

bool NFMemHashMapCompact::HashMapLockItem::equal(const std::string& k, int &ret)
{
	std::string k1;
	ret = get(k1);

	if (ret == NFMemHashMapCompact::RT_OK && k == k1)
	{
		return true;
	}

	return false;
}

void NFMemHashMapCompact::HashMapLockItem::nextItem(int iType)
{
	Block block(_pMap, _iAddr);

	if (iType == HashMapLockIterator::IT_BLOCK)
	{
		uint32_t index = block.getBlockHead()->_iIndex;

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
	else if (iType == HashMapLockIterator::IT_SET)
	{
		_iAddr = block.getBlockHead()->_iSetNext;
	}
	else if (iType == HashMapLockIterator::IT_GET)
	{
		_iAddr = block.getBlockHead()->_iGetNext;
	}
}

void NFMemHashMapCompact::HashMapLockItem::prevItem(int iType)
{
	Block block(_pMap, _iAddr);

	if (iType == HashMapLockIterator::IT_BLOCK)
	{
		uint32_t index = block.getBlockHead()->_iIndex;
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
	else if (iType == HashMapLockIterator::IT_SET)
	{
		_iAddr = block.getBlockHead()->_iSetPrev;
	}
	else if (iType == HashMapLockIterator::IT_GET)
	{
		_iAddr = block.getBlockHead()->_iGetPrev;
	}
}

///////////////////////////////////////////////////////////////////

NFMemHashMapCompact::HashMapLockIterator::HashMapLockIterator()
	: _pMap(NULL), _iItem(NULL, 0), _iType(IT_BLOCK), _iOrder(IT_NEXT)
{
}

NFMemHashMapCompact::HashMapLockIterator::HashMapLockIterator(NFMemHashMapCompact *pMap, uint32_t iAddr, int iType, int iOrder)
	: _pMap(pMap), _iItem(_pMap, iAddr), _iType(iType), _iOrder(iOrder)
{
}

NFMemHashMapCompact::HashMapLockIterator::HashMapLockIterator(const HashMapLockIterator &it)
	: _pMap(it._pMap), _iItem(it._iItem), _iType(it._iType), _iOrder(it._iOrder)
{
}

NFMemHashMapCompact::HashMapLockIterator& NFMemHashMapCompact::HashMapLockIterator::operator=(const HashMapLockIterator &it)
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

bool NFMemHashMapCompact::HashMapLockIterator::operator==(const HashMapLockIterator& mcmi)
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

bool NFMemHashMapCompact::HashMapLockIterator::operator!=(const HashMapLockIterator& mcmi)
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

NFMemHashMapCompact::HashMapLockIterator& NFMemHashMapCompact::HashMapLockIterator::operator++()
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

NFMemHashMapCompact::HashMapLockIterator NFMemHashMapCompact::HashMapLockIterator::operator++(int)
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

///////////////////////////////////////////////////////////////////

NFMemHashMapCompact::HashMapItem::HashMapItem(NFMemHashMapCompact *pMap, uint32_t iIndex)
	: _pMap(pMap)
	, _iIndex(iIndex)
{
}

NFMemHashMapCompact::HashMapItem::HashMapItem(const HashMapItem &mcmdi)
	: _pMap(mcmdi._pMap)
	, _iIndex(mcmdi._iIndex)
{
}

NFMemHashMapCompact::HashMapItem &NFMemHashMapCompact::HashMapItem::operator=(const NFMemHashMapCompact::HashMapItem &mcmdi)
{
	if (this != &mcmdi)
	{
		_pMap = mcmdi._pMap;
		_iIndex = mcmdi._iIndex;
	}
	return (*this);
}

bool NFMemHashMapCompact::HashMapItem::operator==(const NFMemHashMapCompact::HashMapItem &mcmdi)
{
	return _pMap == mcmdi._pMap && _iIndex == mcmdi._iIndex;
}

bool NFMemHashMapCompact::HashMapItem::operator!=(const NFMemHashMapCompact::HashMapItem &mcmdi)
{
	return _pMap != mcmdi._pMap || _iIndex != mcmdi._iIndex;
}

void NFMemHashMapCompact::HashMapItem::get(std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	uint32_t iAddr = _pMap->item(_iIndex)->_iBlockAddr;

	while (iAddr != 0)
	{
		Block block(_pMap, iAddr);
		NFMemHashMapCompact::BlockData data;

		int ret = block.getBlockData(data);
		if (ret == NFMemHashMapCompact::RT_OK)
		{
			vtData.push_back(data);
		}

		iAddr = block.getBlockHead()->_iBlockNext;
	}
}

void NFMemHashMapCompact::HashMapItem::getExpire(uint32_t t, std::vector<NFMemHashMapCompact::BlockData> &vtData)
{
	uint32_t iAddr = _pMap->item(_iIndex)->_iBlockAddr;

	while (iAddr != 0)
	{
		Block block(_pMap, iAddr);
		NFMemHashMapCompact::BlockData data;

		int ret = block.getBlockData(data);
		if (ret == NFMemHashMapCompact::RT_OK)
		{
			if (data._expiret != 0 && t >= data._expiret)
				vtData.push_back(data);
		}

		iAddr = block.getBlockHead()->_iBlockNext;
	}
}

void NFMemHashMapCompact::HashMapItem::nextItem()
{
	if (_iIndex == (uint32_t)(-1))
	{
		return;
	}

	if (_iIndex >= _pMap->getHashCount() - 1)
	{
		_iIndex = (uint32_t)(-1);
		return;
	}
	_iIndex++;
}

int NFMemHashMapCompact::HashMapItem::setDirty()
{
	if (_pMap->getMapHead()._bReadOnly) return RT_READONLY;

	uint32_t iAddr = _pMap->item(_iIndex)->_iBlockAddr;

	while (iAddr != 0)
	{
		Block block(_pMap, iAddr);

		if (!block.isOnlyKey())
		{
			NFMemHashMapCompact::FailureRecover check(_pMap);

			block.setDirty(true);
			block.refreshSetList();
		}

		iAddr = block.getBlockHead()->_iBlockNext;
	}

	return RT_OK;
}

///////////////////////////////////////////////////////////////////

NFMemHashMapCompact::HashMapIterator::HashMapIterator()
	: _pMap(NULL), _iItem(NULL, 0)
{
}

NFMemHashMapCompact::HashMapIterator::HashMapIterator(NFMemHashMapCompact *pMap, uint32_t iIndex)
	: _pMap(pMap), _iItem(_pMap, iIndex)
{
}

NFMemHashMapCompact::HashMapIterator::HashMapIterator(const HashMapIterator &it)
	: _pMap(it._pMap), _iItem(it._iItem)
{
}

NFMemHashMapCompact::HashMapIterator& NFMemHashMapCompact::HashMapIterator::operator=(const HashMapIterator &it)
{
	if (this != &it)
	{
		_pMap = it._pMap;
		_iItem = it._iItem;
	}

	return (*this);
}

bool NFMemHashMapCompact::HashMapIterator::operator==(const HashMapIterator& mcmi)
{
	if (_iItem.getIndex() != (uint32_t)-1 || mcmi._iItem.getIndex() != (uint32_t)-1)
	{
		return _pMap == mcmi._pMap && _iItem == mcmi._iItem;
	}

	return _pMap == mcmi._pMap;
}

bool NFMemHashMapCompact::HashMapIterator::operator!=(const HashMapIterator& mcmi)
{
	if (_iItem.getIndex() != (uint32_t)-1 || mcmi._iItem.getIndex() != (uint32_t)-1)
	{
		return _pMap != mcmi._pMap || _iItem != mcmi._iItem;
	}

	return _pMap != mcmi._pMap;
}

NFMemHashMapCompact::HashMapIterator& NFMemHashMapCompact::HashMapIterator::operator++()
{
	_iItem.nextItem();
	return (*this);
}

NFMemHashMapCompact::HashMapIterator NFMemHashMapCompact::HashMapIterator::operator++(int)
{
	HashMapIterator it(*this);
	_iItem.nextItem();
	return it;
}

//////////////////////////////////////////////////////////////////////////////////

void NFMemHashMapCompact::init(void *pAddr)
{
	_pHead = static_cast<tagMapHead*>(pAddr);
	_pstModifyHead = static_cast<tagModifyHead*>((void*)((char*)pAddr + sizeof(tagMapHead)));
}

void NFMemHashMapCompact::initDataBlockSize(uint32_t iMinDataSize, uint32_t iMaxDataSize, float fFactor)
{
	_iMinDataSize = iMinDataSize;
	_iMaxDataSize = iMaxDataSize;
	_fFactor = fFactor;
}

void NFMemHashMapCompact::create(void *pAddr, size_t iSize)
{
	if (sizeof(tagHashItem) * 1
		+ sizeof(tagMapHead)
		+ sizeof(tagModifyHead)
		+ sizeof(NFMemMultiChunkAllocator::tagChunkAllocatorHead)
		+ 10 > iSize)
	{
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::create] mem size not enougth.");
	}

	if (_iMinDataSize == 0 || _iMaxDataSize == 0 || _fFactor < 1.0)
	{
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::create] init data size error:" + lexical_cast<std::string>(_iMinDataSize) + "|" + lexical_cast<std::string>(_iMaxDataSize) + "|" + lexical_cast<std::string>(_fFactor));
	}

	init(pAddr);

	//block size��2���ֽڴ洢��
	if (sizeof(Block::tagBlockHead) + _iMaxDataSize >(uint16_t)(-1))
	{
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::create] init block size error, block size must be less then " + lexical_cast<std::string>((uint16_t)(-1) - sizeof(Block::tagBlockHead)));
	}

	_pHead->_bInit = false;
	_pHead->_cMaxVersion = MAX_VERSION;
	_pHead->_cMinVersion = MIN_VERSION;
	_pHead->_bReadOnly = false;
	_pHead->_bAutoErase = true;
	_pHead->_cEraseMode = ERASEBYGET;
	_pHead->_iMemSize = iSize;
	_pHead->_iMinDataSize = _iMinDataSize;
	_pHead->_iMaxDataSize = _iMaxDataSize;
	_pHead->_fFactor = _fFactor;
	_pHead->_fRadio = _fRadio;
	_pHead->_iElementCount = 0;
	_pHead->_iEraseCount = 10;
	_pHead->_iSetHead = 0;
	_pHead->_iSetTail = 0;
	_pHead->_iGetHead = 0;
	_pHead->_iGetTail = 0;
	_pHead->_iDirtyCount = 0;
	_pHead->_iDirtyTail = 0;
	_pHead->_iSyncTime = 60 * 10;  //ȱʡʮ���ӻ�дһ��
	_pHead->_iUsedChunk = 0;
	_pHead->_iGetCount = 0;
	_pHead->_iHitCount = 0;
	_pHead->_iBackupTail = 0;
	_pHead->_iSyncTail = 0;
	memset(_pHead->_cReserve, 0, sizeof(_pHead->_cReserve));

	_pstModifyHead->_cModifyStatus = 0;
	_pstModifyHead->_iNowIndex = 0;

	//����ƽ��block��С
	uint32_t iBlockSize = (_pHead->_iMinDataSize + _pHead->_iMaxDataSize) / 2 + sizeof(Block::tagBlockHead);

	//Hash����
	uint32_t iHashCount = (iSize - sizeof(NFMemChunkAllocator::tagChunkAllocatorHead)) / ((uint32_t)(iBlockSize*_fRadio) + sizeof(tagHashItem));
	//���������������Ϊhashֵ
	iHashCount = getMinPrimeNumber(iHashCount);

	void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);

	uint32_t iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iHashCount);
	_hash.create(pHashAddr, iHashMemSize);

	void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

	_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead), sizeof(Block::tagBlockHead) + _pHead->_iMinDataSize, sizeof(Block::tagBlockHead) + _pHead->_iMaxDataSize, _pHead->_fFactor);

	_pHead->_bInit = true;
}

void NFMemHashMapCompact::connect(void *pAddr, size_t iSize)
{
	init(pAddr);

	if (_pHead->_cMaxVersion == 3)
	{
		_pHead->_cMaxVersion = MAX_VERSION;
		_pHead->_bInit = true;
	}

	if (!_pHead->_bInit)
	{
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::connect] hash map created unsuccessfully, so can not be connected");
	}

	if (_pHead->_cMaxVersion != MAX_VERSION || _pHead->_cMinVersion != MIN_VERSION)
	{
		std::ostringstream os;
		os << (int)_pHead->_cMaxVersion << "." << (int)_pHead->_cMinVersion << " != " << ((int)MAX_VERSION) << "." << ((int)MIN_VERSION);
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::connect] hash map version not equal:" + os.str() + " (data != code)");
	}

	if (_pHead->_iMemSize != iSize)
	{
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::connect] hash map size not equal:" + lexical_cast<std::string>(_pHead->_iMemSize) + "!=" + lexical_cast<std::string>(iSize));
	}

	void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);
	_hash.connect(pHashAddr);

	void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();

	_pDataAllocator->connect(pDataAddr);

	_iMinDataSize = _pHead->_iMinDataSize;
	_iMaxDataSize = _pHead->_iMaxDataSize;
	_fFactor = _pHead->_fFactor;
	_fRadio = _pHead->_fRadio;
}

int NFMemHashMapCompact::append(void *pAddr, size_t iSize)
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
		throw NF_HashMapCompact_Exception("[TC_HashMapCompact::append] hash map version not equal:" + os.str() + " (data != code)");
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

void NFMemHashMapCompact::clear()
{
	assert(_pHead);
	FailureRecover check(this);

	_pHead->_iElementCount = 0;
	_pHead->_iSetHead = 0;
	_pHead->_iSetTail = 0;
	_pHead->_iGetHead = 0;
	_pHead->_iGetTail = 0;
	_pHead->_iDirtyCount = 0;
	_pHead->_iDirtyTail = 0;
	_pHead->_iUsedChunk = 0;
	_pHead->_iGetCount = 0;
	_pHead->_iHitCount = 0;
	_pHead->_iBackupTail = 0;
	_pHead->_iSyncTail = 0;

	_hash.clear();

	_pDataAllocator->rebuild();
}

int NFMemHashMapCompact::dump2file(const std::string &sFile)
{
	FILE *fp = fopen(sFile.c_str(), "wb");
	if (fp == NULL)
	{
		return RT_DUMP_FILE_ERR;
	}

	size_t ret = fwrite((void*)_pHead, 1, _pHead->_iMemSize, fp);
	fclose(fp);

	if (ret == _pHead->_iMemSize)
	{
		return RT_OK;
	}
	return RT_DUMP_FILE_ERR;
}

int NFMemHashMapCompact::load5file(const std::string &sFile)
{
	FILE *fp = fopen(sFile.c_str(), "rb");
	if (fp == NULL)
	{
		return RT_LOAL_FILE_ERR;
	}
	fseek(fp, 0L, SEEK_END);
	size_t fs = ftell(fp);
	if (fs != _pHead->_iMemSize)
	{
		fclose(fp);
		return RT_LOAL_FILE_ERR;
	}

	fseek(fp, 0L, SEEK_SET);

	size_t iSize = 1024 * 1024 * 10;
	size_t iLen = 0;
	char *pBuffer = new char[iSize];
	bool bEof = false;
	while (true)
	{
		int ret = fread(pBuffer, 1, iSize, fp);
		if (ret != (int)iSize)
		{
			if (feof(fp))
			{
				bEof = true;
			}
			else
			{
				fclose(fp);
				delete[] pBuffer;
				return RT_LOAL_FILE_ERR;
			}

		}
		//���汾
		if (iLen == 0)
		{
			if (pBuffer[0] != MAX_VERSION || pBuffer[1] != MIN_VERSION)
			{
				fclose(fp);
				delete[] pBuffer;
				return RT_VERSION_MISMATCH_ERR;
			}
		}

		memcpy((char*)_pHead + iLen, pBuffer, ret);
		iLen += ret;
		if (bEof)
			break;
	}
	fclose(fp);
	delete[] pBuffer;
	if (iLen == _pHead->_iMemSize)
	{
		return RT_OK;
	}

	return RT_LOAL_FILE_ERR;
}

int NFMemHashMapCompact::recover(size_t i, bool bRepair)
{
	doUpdate();

	if (i >= _hash.size())
	{
		return 0;
	}

	size_t e = 0;

check:
	size_t iAddr = item(i)->_iBlockAddr;

	Block block(this, iAddr);

	while (block.getHead() != 0)
	{
		BlockData data;
		int ret = block.getBlockData(data);
		if (ret != NFMemHashMapCompact::RT_OK && ret != NFMemHashMapCompact::RT_ONLY_KEY)
		{
			//����ɾ��block��
			++e;

			if (bRepair)
			{
				block.erase();
				goto check;
			}
		}

		if (!block.nextBlock())
		{
			break;
		}
	}

	return e;
}

int NFMemHashMapCompact::checkDirty(const std::string &k)
{
	FailureRecover check(this);
	incGetCount();

	int ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, ret);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	//û������
	if (it == end())
	{
		return NFMemHashMapCompact::RT_NO_DATA;
	}

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemHashMapCompact::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	if (block.isDirty())
	{
		return NFMemHashMapCompact::RT_DIRTY_DATA;
	}

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::setDirty(const std::string& k)
{
	FailureRecover check(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	incGetCount();

	int ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, ret);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	//û�����ݻ�ֻ��Key
	if (it == end())
	{
		return NFMemHashMapCompact::RT_NO_DATA;
	}

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemHashMapCompact::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	block.setDirty(true);
	block.refreshSetList();

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::setDirtyAfterSync(const std::string& k)
{
	FailureRecover check(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	incGetCount();

	int ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, ret);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	//û�����ݻ�ֻ��Key
	if (it == end())
	{
		return NFMemHashMapCompact::RT_NO_DATA;
	}

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemHashMapCompact::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	block.setDirty(true);
	if (_pHead->_iDirtyTail == block.getBlockHead()->_iSetPrev)
	{
		_pHead->_iDirtyTail = block.getHead();
	}

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::setClean(const std::string& k)
{
	FailureRecover check(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	incGetCount();

	int ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, ret);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	//û�����ݻ�ֻ��Key
	if (it == end())
	{
		return NFMemHashMapCompact::RT_NO_DATA;
	}

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemHashMapCompact::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	block.setDirty(false);
	block.refreshSetList();

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::get(const std::string& k, std::string &v, uint32_t &iSyncTime, uint32_t& iExpireTime, uint8_t& iVersion)
{
	FailureRecover check(this);
	incGetCount();

	int ret = NFMemHashMapCompact::RT_OK;

	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, v, ret);

	if (ret != NFMemHashMapCompact::RT_OK && ret != NFMemHashMapCompact::RT_ONLY_KEY)
	{
		return ret;
	}

	//û������
	if (it == end())
	{
		return NFMemHashMapCompact::RT_NO_DATA;
	}

	Block block(this, it->getAddr());
	//ֻ��Key
	if (block.isOnlyKey())
	{
		iVersion = block.getVersion();
		return NFMemHashMapCompact::RT_ONLY_KEY;
	}

	iSyncTime = block.getSyncTime();
	iExpireTime = block.getExpireTime();
	iVersion = block.getVersion();

	//���ֻ��, ��ˢ��get����
	if (!_pHead->_bReadOnly)
	{
		block.refreshGetList();
	}
	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::get(const std::string& k, std::string &v, uint32_t &iSyncTime)
{
	uint32_t iExpireTime;
	uint8_t iVersion;
	return get(k, v, iSyncTime, iExpireTime, iVersion);
}

int NFMemHashMapCompact::get(const std::string& k, std::string &v)
{
	uint32_t iSyncTime;
	uint32_t iExpireTime;
	uint8_t iVersion;
	return get(k, v, iSyncTime, iExpireTime, iVersion);
}

int NFMemHashMapCompact::set(const std::string& k, const std::string& v, bool bDirty, std::vector<BlockData> &vtData)
{
	return set(k, v, 0, 0, bDirty, vtData);
}

int NFMemHashMapCompact::set(const std::string& k, const std::string& v, uint32_t iExpireTime, uint8_t iVersion, bool bDirty, std::vector<BlockData> &vtData)
{
	FailureRecover check(this);
	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, ret);
	bool bNewBlock = false;

	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	if (it == end())
	{
		TC_PackIn pi;
		pi << k;
		pi << v;
		uint32_t iAllocSize = sizeof(Block::tagBlockHead) + pi.length();

		//�ȷ���ռ�, �������̭������
		uint32_t iAddr = _pDataAllocator->allocateMemBlock(index, iAllocSize, vtData);
		if (iAddr == 0)
		{
			return NFMemHashMapCompact::RT_NO_MEMORY;
		}

		it = HashMapLockIterator(this, iAddr, HashMapLockIterator::IT_BLOCK, HashMapLockIterator::IT_NEXT);
		bNewBlock = true;
	}

	ret = it->set(k, v, iExpireTime, iVersion, bNewBlock, vtData);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		//�¼�¼, дʧ����, Ҫɾ���ÿ�
		if (bNewBlock)
		{
			Block block(this, it->getAddr());
			block.erase();
		}
		return ret;
	}

	Block block(this, it->getAddr());
	if (bNewBlock)
	{
		block.setSyncTime(time(NULL));
	}
	block.setDirty(bDirty);
	block.refreshSetList();

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::set(const std::string& k, std::vector<BlockData> &vtData)
{
	return set(k, 0, vtData);
}

int NFMemHashMapCompact::set(const std::string& k, uint8_t iVersion, std::vector<BlockData>& vtData)
{
	FailureRecover check(this);
	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);
	lock_iterator it = find(k, index, ret);
	bool bNewBlock = false;

	if (ret != NFMemHashMapCompact::RT_OK)
	{
		return ret;
	}

	if (it == end())
	{
		TC_PackIn pi;
		pi << k;
		uint32_t iAllocSize = sizeof(Block::tagBlockHead) + pi.length();

		//�ȷ���ռ�, �������̭������
		uint32_t iAddr = _pDataAllocator->allocateMemBlock(index, iAllocSize, vtData);
		if (iAddr == 0)
		{
			return NFMemHashMapCompact::RT_NO_MEMORY;
		}

		it = HashMapLockIterator(this, iAddr, HashMapLockIterator::IT_BLOCK, HashMapLockIterator::IT_NEXT);
		bNewBlock = true;
	}

	ret = it->set(k, iVersion, bNewBlock, vtData);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		//�¼�¼, дʧ����, Ҫɾ���ÿ�
		if (bNewBlock)
		{
			Block block(this, it->getAddr());
			block.erase();
		}
		return ret;
	}

	Block block(this, it->getAddr());
	if (bNewBlock)
	{
		block.setSyncTime(time(NULL));
	}
	block.refreshSetList();

	return NFMemHashMapCompact::RT_OK;
}

int NFMemHashMapCompact::del(const std::string& k, BlockData &data)
{
	FailureRecover check(this);
	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int      ret = NFMemHashMapCompact::RT_OK;
	uint32_t index = hashIndex(k);

	data._key = k;

	lock_iterator it = find(k, index, data._value, ret);
	if (ret != NFMemHashMapCompact::RT_OK && ret != NFMemHashMapCompact::RT_ONLY_KEY)
	{
		return ret;
	}

	if (it == end())
	{
		return NFMemHashMapCompact::RT_NO_DATA;
	}

	Block block(this, it->getAddr());
	ret = block.getBlockData(data);
	block.erase();

	return ret;
}

int NFMemHashMapCompact::erase(int radio, BlockData &data, bool bCheckDirty)
{
	FailureRecover check(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	if (radio <= 0)   radio = 1;
	if (radio >= 100) radio = 100;

	uint32_t iAddr = _pHead->_iGetTail;
	//������ͷ��
	if (iAddr == 0)
	{
		return RT_OK;
	}

	//ɾ����ָ��������
	if ((_pHead->_iUsedChunk * 100. / allBlockChunkCount()) < radio)
	{
		return RT_OK;
	}

	Block block(this, iAddr);
	if (bCheckDirty)
	{
		if (block.isDirty())
		{
			if (_pHead->_iDirtyTail == 0)
			{
				_pHead->_iDirtyTail = iAddr;
			}
			block.refreshGetList();
			return RT_DIRTY_DATA;
		}
	}
	int ret = block.getBlockData(data);
	block.erase();
	if (ret == NFMemHashMapCompact::RT_OK)
	{
		return NFMemHashMapCompact::RT_ERASE_OK;
	}
	return ret;
}

void NFMemHashMapCompact::sync()
{
	FailureRecover check(this);

	_pHead->_iSyncTail = _pHead->_iDirtyTail;
}

int NFMemHashMapCompact::sync(uint32_t iNowTime, BlockData &data)
{
	FailureRecover check(this);

	uint32_t iAddr = _pHead->_iSyncTail;

	//������ͷ����, ����RT_OK
	if (iAddr == 0)
	{
		return RT_OK;
	}

	Block block(this, iAddr);

	_pHead->_iSyncTail = block.getBlockHead()->_iSetPrev;

	//��ǰ�����Ǹɾ�����
	if (!block.isDirty())
	{
		if (_pHead->_iDirtyTail == iAddr)
		{
			_pHead->_iDirtyTail = block.getBlockHead()->_iSetPrev;
		}
		return RT_NONEED_SYNC;
	}

	int ret = block.getBlockData(data);
	if (ret != NFMemHashMapCompact::RT_OK)
	{
		//û�л�ȡ�����ļ�¼
		if (_pHead->_iDirtyTail == iAddr)
		{
			_pHead->_iDirtyTail = block.getBlockHead()->_iSetPrev;
		}
		return ret;
	}

	//�������ҳ���_pHead->_iSyncTimeû�л�д, ��Ҫ��д
	if (_pHead->_iSyncTime + data._synct < iNowTime)
	{
		block.setDirty(false);
		block.setSyncTime(iNowTime);

		if (_pHead->_iDirtyTail == iAddr)
		{
			_pHead->_iDirtyTail = block.getBlockHead()->_iSetPrev;
		}

		return RT_NEED_SYNC;
	}

	//������, ���ǲ���Ҫ��д, ��������ǰ��
	return RT_NONEED_SYNC;
}

void NFMemHashMapCompact::backup(bool bForceFromBegin)
{
	FailureRecover check(this);

	if (bForceFromBegin || _pHead->_iBackupTail == 0)
	{
		//�ƶ�����ָ�뵽Get��β��, ׼����ʼ��������
		_pHead->_iBackupTail = _pHead->_iGetTail;
	}
}

int NFMemHashMapCompact::backup(BlockData &data)
{
	FailureRecover check(this);

	uint32_t iAddr = _pHead->_iBackupTail;

	//������ͷ����, ����RT_OK
	if (iAddr == 0)
	{
		return RT_OK;
	}

	Block block(this, iAddr);
	int ret = block.getBlockData(data);

	//Ǩ��һ��
	_pHead->_iBackupTail = block.getBlockHead()->_iGetPrev;

	if (ret == NFMemHashMapCompact::RT_OK)
	{
		return NFMemHashMapCompact::RT_NEED_BACKUP;
	}
	return ret;
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::find(const std::string& k)
{
	FailureRecover check(this);

	uint32_t index = hashIndex(k);
	int ret = NFMemHashMapCompact::RT_OK;

	if (item(index)->_iBlockAddr == 0)
	{
		return end();
	}

	Block mb(this, item(index)->_iBlockAddr);
	while (true)
	{
		HashMapLockItem mcmdi(this, mb.getHead());
		if (mcmdi.equal(k, ret))
		{
			incHitCount();
			return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
		}

		if (!mb.nextBlock())
		{
			return end();
		}
	}

	return end();
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::begin()
{
	FailureRecover check(this);

	for (size_t i = 0; i < _hash.size(); ++i)
	{
		tagHashItem &hashItem = _hash[i];
		if (hashItem._iBlockAddr != 0)
		{
			return lock_iterator(this, hashItem._iBlockAddr, lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
		}
	}

	return end();
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::rbegin()
{
	FailureRecover check(this);

	for (size_t i = _hash.size(); i > 0; --i)
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

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::beginSetTime()
{
	FailureRecover check(this);
	return lock_iterator(this, _pHead->_iSetHead, lock_iterator::IT_SET, lock_iterator::IT_NEXT);
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::rbeginSetTime()
{
	FailureRecover check(this);
	return lock_iterator(this, _pHead->_iSetTail, lock_iterator::IT_SET, lock_iterator::IT_PREV);
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::beginGetTime()
{
	FailureRecover check(this);
	return lock_iterator(this, _pHead->_iGetHead, lock_iterator::IT_GET, lock_iterator::IT_NEXT);
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::rbeginGetTime()
{
	FailureRecover check(this);
	return lock_iterator(this, _pHead->_iGetTail, lock_iterator::IT_GET, lock_iterator::IT_PREV);
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::beginDirty()
{
	FailureRecover check(this);
	return lock_iterator(this, _pHead->_iDirtyTail, lock_iterator::IT_SET, lock_iterator::IT_PREV);
}

NFMemHashMapCompact::hash_iterator NFMemHashMapCompact::hashBegin()
{
	FailureRecover check(this);
	return hash_iterator(this, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string NFMemHashMapCompact::desc()
{
	std::ostringstream s;
	{
		s << "[Version          = " << (int)_pHead->_cMaxVersion << "." << (int)_pHead->_cMinVersion << "]" << std::endl;
		s << "[ReadOnly         = " << _pHead->_bReadOnly << "]" << std::endl;
		s << "[AutoErase        = " << _pHead->_bAutoErase << "]" << std::endl;
		s << "[MemSize          = " << _pHead->_iMemSize << "]" << std::endl;
		s << "[Capacity         = " << _pDataAllocator->getCapacity() << "]" << std::endl;
		s << "[SingleBlockCount = " << NFCommon::tostr(singleBlockChunkCount()) << "]" << std::endl;
		s << "[AllBlockChunk    = " << allBlockChunkCount() << "]" << std::endl;
		s << "[UsedChunk        = " << _pHead->_iUsedChunk << "]" << std::endl;
		s << "[FreeChunk        = " << allBlockChunkCount() - _pHead->_iUsedChunk << "]" << std::endl;
		s << "[MinDataSize      = " << _pHead->_iMinDataSize << "]" << std::endl;
		s << "[MaxDataSize      = " << _pHead->_iMaxDataSize << "]" << std::endl;
		s << "[HashCount        = " << _hash.size() << "]" << std::endl;
		s << "[HashRadio        = " << _pHead->_fRadio << "]" << std::endl;
		s << "[ElementCount     = " << _pHead->_iElementCount << "]" << std::endl;
		s << "[SetHead          = " << _pHead->_iSetHead << "]" << std::endl;
		s << "[SetTail          = " << _pHead->_iSetTail << "]" << std::endl;
		s << "[GetHead          = " << _pHead->_iGetHead << "]" << std::endl;
		s << "[GetTail          = " << _pHead->_iGetTail << "]" << std::endl;
		s << "[DirtyTail        = " << _pHead->_iDirtyTail << "]" << std::endl;
		s << "[SyncTail         = " << _pHead->_iSyncTail << "]" << std::endl;
		s << "[SyncTime         = " << _pHead->_iSyncTime << "]" << std::endl;
		s << "[BackupTail       = " << _pHead->_iBackupTail << "]" << std::endl;
		s << "[DirtyCount       = " << _pHead->_iDirtyCount << "]" << std::endl;
		s << "[GetCount         = " << _pHead->_iGetCount << "]" << std::endl;
		s << "[HitCount         = " << _pHead->_iHitCount << "]" << std::endl;
		s << "[ModifyStatus     = " << (int)_pstModifyHead->_cModifyStatus << "]" << std::endl;
		s << "[ModifyIndex      = " << _pstModifyHead->_iNowIndex << "]" << std::endl;
		s << "[OnlyKeyCount     = " << _pHead->_iOnlyKeyCount << "]" << std::endl;
	}

	uint32_t iMaxHash;
	uint32_t iMinHash;
	float fAvgHash;

	analyseHash(iMaxHash, iMinHash, fAvgHash);
	{
		s << "[MaxHash          = " << iMaxHash << "]" << std::endl;
		s << "[MinHash          = " << iMinHash << "]" << std::endl;
		s << "[AvgHash          = " << fAvgHash << "]" << std::endl;
	}

	std::vector<NFMemChunk::tagChunkHead> vChunkHead = _pDataAllocator->getBlockDetail();

	s << "*************************************************************************" << std::endl;
	s << "[DiffBlockCount   = " << vChunkHead.size() << "]" << std::endl;
	for (size_t i = 0; i < vChunkHead.size(); i++)
	{
		s << "[BlockSize        = " << vChunkHead[i]._iBlockSize << "]";
		s << "[BlockCount       = " << vChunkHead[i]._iBlockCount << "]";
		s << "[BlockAvailable   = " << vChunkHead[i]._blockAvailable << "]" << std::endl;
	}

	return s.str();
}

uint32_t NFMemHashMapCompact::eraseExcept(uint32_t iNowAddr, std::vector<BlockData> &vtData)
{
	//������������̭ǰʹ�޸���Ч
	doUpdate();

	//���ܱ���̭
	if (!_pHead->_bAutoErase) return 0;

	uint32_t n = _pHead->_iEraseCount;
	if (n == 0) n = 10;
	uint32_t d = n;

	while (d != 0)
	{
		uint32_t iAddr;

		//�жϰ������ַ�ʽ��̭
		if (_pHead->_cEraseMode == NFMemHashMapCompact::ERASEBYSET)
		{
			iAddr = _pHead->_iSetTail;
		}
		else
		{
			iAddr = _pHead->_iGetTail;
		}

		if (iAddr == 0)
		{
			break;
		}

		Block block(this, iAddr);

		//��ǰblock���ڷ���ռ�, ����ɾ��, �Ƶ���һ������
		if (iNowAddr == iAddr)
		{
			if (_pHead->_cEraseMode == NFMemHashMapCompact::ERASEBYSET)
			{
				iAddr = block.getBlockHead()->_iSetPrev;
			}
			else
			{
				iAddr = block.getBlockHead()->_iGetPrev;
			}
		}
		if (iAddr == 0)
		{
			break;
		}

		Block block1(this, iAddr);
		BlockData data;
		int ret = block1.getBlockData(data);
		if (ret == NFMemHashMapCompact::RT_OK)
		{
			vtData.push_back(data);
			d--;
		}
		else if (ret == NFMemHashMapCompact::RT_NO_DATA)
		{
			d--;
		}

		//���ֳ�����
		FailureRecover check(this);
		//ɾ������
		block1.erase();
	}

	return n - d;
}

uint32_t NFMemHashMapCompact::hashIndex(const std::string& k)
{
	return _hashf(k) % _hash.size();
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::find(const std::string& k, uint32_t index, std::string &v, int &ret)
{
	ret = NFMemHashMapCompact::RT_OK;

	if (item(index)->_iBlockAddr == 0)
	{
		return end();
	}

	Block mb(this, item(index)->_iBlockAddr);
	while (true)
	{
		HashMapLockItem mcmdi(this, mb.getHead());
		if (mcmdi.equal(k, v, ret))
		{
			incHitCount();
			return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
		}

		if (!mb.nextBlock())
		{
			return end();
		}
	}

	return end();
}

NFMemHashMapCompact::lock_iterator NFMemHashMapCompact::find(const std::string& k, uint32_t index, int &ret)
{
	ret = NFMemHashMapCompact::RT_OK;

	if (item(index)->_iBlockAddr == 0)
	{
		return end();
	}

	Block mb(this, item(index)->_iBlockAddr);
	while (true)
	{
		HashMapLockItem mcmdi(this, mb.getHead());
		if (mcmdi.equal(k, ret))
		{
			incHitCount();
			return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
		}

		if (!mb.nextBlock())
		{
			return end();
		}
	}

	return end();
}

void NFMemHashMapCompact::analyseHash(uint32_t &iMaxHash, uint32_t &iMinHash, float &fAvgHash)
{
	iMaxHash = 0;
	iMinHash = (uint32_t)-1;

	fAvgHash = 0;

	uint32_t n = 0;
	for (uint32_t i = 0; i < _hash.size(); i++)
	{
		iMaxHash = max(_hash[i]._iListCount, iMaxHash);
		iMinHash = min(_hash[i]._iListCount, iMinHash);
		//ƽ��ֵֻͳ�Ʒ�0��
		if (_hash[i]._iListCount != 0)
		{
			n++;
			fAvgHash += _hash[i]._iListCount;
		}
	}

	if (n != 0)
	{
		fAvgHash = fAvgHash / n;
	}
}

void NFMemHashMapCompact::saveAddr(uint32_t iAddr, char cByte)
{
	_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._iModifyAddr = iAddr;
	_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex]._cBytes = cByte;// 0;
	_pstModifyHead->_cModifyStatus = 1;

	_pstModifyHead->_iNowIndex++;

	assert(_pstModifyHead->_iNowIndex < sizeof(_pstModifyHead->_stModifyData) / sizeof(tagModifyData));
}

void NFMemHashMapCompact::doRecover()
{
	//==1 copy������, ����ʧ��, �ָ�ԭ����
	if (_pstModifyHead->_cModifyStatus == 1)
	{
		for (int i = _pstModifyHead->_iNowIndex - 1; i >= 0; i--)
		{
			if (_pstModifyHead->_stModifyData[i]._cBytes == sizeof(uint64_t))
			{
				*(uint64_t*)((char*)_pHead + _pstModifyHead->_stModifyData[i]._iModifyAddr) = _pstModifyHead->_stModifyData[i]._iModifyValue;
			}
			else if (_pstModifyHead->_stModifyData[i]._cBytes == sizeof(uint32_t))
			{
				*(uint32_t*)((char*)_pHead + _pstModifyHead->_stModifyData[i]._iModifyAddr) = (uint32_t)_pstModifyHead->_stModifyData[i]._iModifyValue;
			}
			else if (_pstModifyHead->_stModifyData[i]._cBytes == sizeof(uint16_t))
			{
				*(uint16_t*)((char*)_pHead + _pstModifyHead->_stModifyData[i]._iModifyAddr) = (uint16_t)_pstModifyHead->_stModifyData[i]._iModifyValue;
			}
			else if (_pstModifyHead->_stModifyData[i]._cBytes == sizeof(uint8_t))
			{
				*(uint8_t*)((char*)_pHead + _pstModifyHead->_stModifyData[i]._iModifyAddr) = (bool)_pstModifyHead->_stModifyData[i]._iModifyValue;
			}
			else if (_pstModifyHead->_stModifyData[i]._cBytes == 0)
			{
				deallocate(_pstModifyHead->_stModifyData[i]._iModifyAddr, i);
			}
			else if (_pstModifyHead->_stModifyData[i]._cBytes == -1)
			{
				continue;
			}
			else if (_pstModifyHead->_stModifyData[i]._cBytes == -2)
			{
				deallocate2(_pstModifyHead->_stModifyData[i]._iModifyAddr);
				break;
			}
			else
			{
				assert(true);
			}
		}
		_pstModifyHead->_iNowIndex = 0;
		_pstModifyHead->_cModifyStatus = 0;
	}
}

void NFMemHashMapCompact::doUpdate()
{
	if (_pstModifyHead->_cModifyStatus == 1)
	{
		_pstModifyHead->_iNowIndex = 0;
		_pstModifyHead->_cModifyStatus = 0;
	}
}

void NFMemHashMapCompact::doUpdate2()
{
	if (_pstModifyHead->_cModifyStatus == 1)
	{
		for (size_t i = _pstModifyHead->_iNowIndex - 1; i >= 0; --i)
		{
			if (_pstModifyHead->_stModifyData[i]._cBytes == 0)
			{
				_pstModifyHead->_iNowIndex = i + 1;
				return;
			}
		}
	}
}

void NFMemHashMapCompact::doUpdate3()
{
	assert(_pstModifyHead->_cModifyStatus == 1);
	assert(_pstModifyHead->_iNowIndex >= 3);
	assert(_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex - 3]._cBytes == -1);

	saveValue(&_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex - 3]._cBytes, 0);
	_pstModifyHead->_iNowIndex -= 3;
	deallocate(_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex - 1]._iModifyAddr, _pstModifyHead->_iNowIndex - 1);
	_pstModifyHead->_iNowIndex -= 1;
}

void NFMemHashMapCompact::doUpdate4()
{
	assert(_pstModifyHead->_cModifyStatus == 1);
	assert(_pstModifyHead->_iNowIndex >= 1);
	assert(_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex - 1]._cBytes == -2);

	deallocate2(_pstModifyHead->_stModifyData[_pstModifyHead->_iNowIndex - 1]._iModifyAddr);
}

uint32_t NFMemHashMapCompact::getMinPrimeNumber(uint32_t n)
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

void NFMemHashMapCompact::deallocate(uint32_t iChunk, uint32_t iIndex)
{
	Block::tagChunkHead *pChunk = (Block::tagChunkHead*)getAbsolute(iChunk);

	//����ͷŵ�chunk
	while (pChunk->_bNextChunk)
	{
		//ע��˳�򣬱�֤��ʹ������ʱ���˳�Ҳ����Ƕ�ʧһ��chunk
		uint32_t iNextChunk = pChunk->_iNextChunk;
		Block::tagChunkHead *pNextChunk = (Block::tagChunkHead*)getAbsolute(iNextChunk);
		pChunk->_bNextChunk = pNextChunk->_bNextChunk;
		pChunk->_iNextChunk = pNextChunk->_iNextChunk;

		_pDataAllocator->deallocateMemBlock(iNextChunk);
	}
	_pstModifyHead->_stModifyData[iIndex]._cBytes = -1;
	_pDataAllocator->deallocateMemBlock(iChunk);
}

void NFMemHashMapCompact::deallocate2(uint32_t iHead)
{
	Block::tagBlockHead *pHead = (Block::tagBlockHead*)getAbsolute(iHead);

	//����ͷŵ�chunk
	while (pHead->_bNextChunk)
	{
		//ע��˳�򣬱�֤��ʹ������ʱ���˳�Ҳ����Ƕ�ʧһ��chunk
		uint32_t iNextChunk = pHead->_iNextChunk;
		Block::tagChunkHead *pNextChunk = (Block::tagChunkHead*)getAbsolute(iNextChunk);
		pHead->_bNextChunk = pNextChunk->_bNextChunk;
		pHead->_iNextChunk = pNextChunk->_iNextChunk;

		_pDataAllocator->deallocateMemBlock(iNextChunk);
	}
	doUpdate();
	_pDataAllocator->deallocateMemBlock(iHead);
}

