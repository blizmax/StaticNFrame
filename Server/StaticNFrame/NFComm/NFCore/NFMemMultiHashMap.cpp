// -------------------------------------------------------------------------
//    @FileName         :    NFMultiHashMap.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFMemMultiHashMap.h"
#include "NFCommon.h"

int NFMemMultiHashMap::FailureRecover::_iRefCount = 0;

bool NFMemMultiHashMap::MainKey::next()
{
	_iHead = getHeadPtr()->_iNext;
	_pHead = _pMap->getAbsolute(_iHead);

	return _iHead != 0;
}

bool NFMemMultiHashMap::MainKey::prev()
{
	_iHead = getHeadPtr()->_iPrev;
	_pHead = _pMap->getAbsolute(_iHead);

	return _iHead != 0;
}

void NFMemMultiHashMap::MainKey::deallocate()
{
	if (HASNEXTCHUNK())
	{
		// �ͷ�����chunk
		deallocate(getHeadPtr()->_iNextChunk);
	}

	// �ͷ���keyͷ
	std::vector<uint32_t> v;
	v.push_back(_iHead);
	_pMap->_pDataAllocator->deallocateMemChunk(v);
}

void NFMemMultiHashMap::MainKey::deallocate(uint32_t iChunk)
{
	std::vector<uint32_t> v;
	v.push_back(iChunk);

	tagChunkHead *pChunk = getChunkHead(iChunk);

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

	// �ռ�ȫ���ͷŵ�
	_pMap->_pDataAllocator->deallocateMemChunk(v);
}

void NFMemMultiHashMap::MainKey::makeNew(uint32_t iIndex, uint32_t iAllocSize)
{
	getHeadPtr()->_iSize = iAllocSize;
	getHeadPtr()->_iIndex = iIndex;
	getHeadPtr()->_iAddr = 0;
	getHeadPtr()->_iNext = 0;
	getHeadPtr()->_iPrev = 0;
	getHeadPtr()->_iGetNext = 0;
	getHeadPtr()->_iGetPrev = 0;
	getHeadPtr()->_iDataLen = 0;
	SETNEXTCHUNK(false);
	SETFULLDATA(false);

	_pMap->incMainKeyListCount(iIndex);

	// �ҵ���key����
	if (_pMap->itemMainKey(iIndex)->_iMainKeyAddr == 0)
	{
		//��ǰhashͰû��Ԫ��
		_pMap->saveValue(&_pMap->itemMainKey(iIndex)->_iMainKeyAddr, _iHead);
		_pMap->saveValue(&getHeadPtr()->_iNext, (uint32_t)0);
		_pMap->saveValue(&getHeadPtr()->_iPrev, (uint32_t)0);
	}
	else
	{
		//��ǰhashͰ��Ԫ��, ����Ͱ��ͷ
		_pMap->saveValue(&getHeadPtr(_pMap->itemMainKey(iIndex)->_iMainKeyAddr)->_iPrev, _iHead);
		_pMap->saveValue(&getHeadPtr()->_iNext, _pMap->itemMainKey(iIndex)->_iMainKeyAddr);
		_pMap->saveValue(&_pMap->itemMainKey(iIndex)->_iMainKeyAddr, _iHead);
		_pMap->saveValue(&getHeadPtr()->_iPrev, (uint32_t)0);
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
		_pMap->saveValue(&getHeadPtr()->_iGetNext, _pMap->_pHead->_iGetHead);
		_pMap->saveValue(&getHeadPtr(_pMap->_pHead->_iGetHead)->_iGetPrev, _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iGetHead, _iHead);
	}
}

int NFMemMultiHashMap::MainKey::erase(std::vector<Value> &vtData)
{
	//////////////////�޸ı�����������/////////////
	if (_pMap->_pHead->_iBackupTail == _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iBackupTail, getHeadPtr()->_iGetPrev);
	}

	////////////////////�޸�Get���������//////////
	//
	{
		bool bHead = (_pMap->_pHead->_iGetHead == _iHead);
		bool bTail = (_pMap->_pHead->_iGetTail == _iHead);

		if (!bHead)
		{
			if (bTail)
			{
				assert(getHeadPtr()->_iGetNext == 0);
				//��β��, β��ָ��ָ����һ��Ԫ��
				_pMap->saveValue(&_pMap->_pHead->_iGetTail, getHeadPtr()->_iGetPrev);
				_pMap->saveValue(&getHeadPtr(getHeadPtr()->_iGetPrev)->_iGetNext, (uint32_t)0);
			}
			else
			{
				//����ͷ��Ҳ����β��
				assert(getHeadPtr()->_iGetNext != 0);
				_pMap->saveValue(&getHeadPtr(getHeadPtr()->_iGetPrev)->_iGetNext, getHeadPtr()->_iGetNext);
				_pMap->saveValue(&getHeadPtr(getHeadPtr()->_iGetNext)->_iGetPrev, getHeadPtr()->_iGetPrev);
			}
		}
		else
		{
			if (bTail)
			{
				assert(getHeadPtr()->_iGetNext == 0);
				assert(getHeadPtr()->_iGetPrev == 0);
				//ͷ��Ҳ��β��, ָ�붼����Ϊ0
				_pMap->saveValue(&_pMap->_pHead->_iGetHead, (uint32_t)0);
				_pMap->saveValue(&_pMap->_pHead->_iGetTail, (uint32_t)0);
			}
			else
			{
				//ͷ������β��, ͷ��ָ��ָ����һ��Ԫ��
				assert(getHeadPtr()->_iGetNext != 0);
				_pMap->saveValue(&_pMap->_pHead->_iGetHead, getHeadPtr()->_iGetNext);
				//��һ��Ԫ����ָ��Ϊ0
				_pMap->saveValue(&getHeadPtr(getHeadPtr()->_iGetNext)->_iGetPrev, (uint32_t)0);
			}
		}
	}

	///////////////////ɾ����key�µ�����block/////////////////////////////
	std::string mk;
	int ret = get(mk);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		// todo, ���������д����⣬ǰ����޸Ķ�Ӧ��ȡ��
		return ret;
	}

	while (getHeadPtr()->_iAddr != 0)
	{
		Block block(_pMap, getHeadPtr()->_iAddr);
		Value value;
		value._mkey = mk;
		int ret = block.getBlockData(value._data);
		if (ret == NFMemMultiHashMap::RT_OK)
		{
			vtData.push_back(value);
		}
		block.erase();
	}

	///////////////////����key������ȥ��///////////
	//
	//��һ����keyָ����һ����key
	if (getHeadPtr()->_iPrev != 0)
	{
		_pMap->saveValue(&getHeadPtr(getHeadPtr()->_iPrev)->_iNext, getHeadPtr()->_iNext);
	}

	//��һ����keyָ����һ��
	if (getHeadPtr()->_iNext != 0)
	{
		_pMap->saveValue(&getHeadPtr(getHeadPtr()->_iNext)->_iPrev, getHeadPtr()->_iPrev);
	}

	//////////////////�����hashͷ��, ��Ҫ�޸�hash��������ָ��//////
	//
	_pMap->delMainKeyListCount(getHeadPtr()->_iIndex);
	if (getHeadPtr()->_iPrev == 0)
	{
		//�����hashͰ��ͷ��, ����Ҫ����
		NFMemMultiHashMap::tagMainKeyHashItem *pItem = _pMap->itemMainKey(getHeadPtr()->_iIndex);
		assert(pItem->_iMainKeyAddr == _iHead);
		if (pItem->_iMainKeyAddr == _iHead)
		{
			_pMap->saveValue(&pItem->_iMainKeyAddr, getHeadPtr()->_iNext);
		}
	}

	// �ֹ��ÿ�һЩ������ʹ�����ݻָ���������¼���Ա�crash��ָ�
	_pMap->saveValue(&getHeadPtr()->_iSize, (uint32_t)0);
	_pMap->saveValue(&getHeadPtr()->_iIndex, (uint32_t)0);
	_pMap->saveValue(&getHeadPtr()->_iBitset, (uint8_t)0);
	_pMap->saveValue(&getHeadPtr()->_iDataLen, (uint32_t)0);

	// �黹�ڴ�ǰ��ȷ�������޸ģ���Ϊ�ڴ汻�ͷź��ǲ��ָܻ���
	// ��ʹ������ڴ��ͷ�ʧ��Ҳ����������ݵ��ƻ�
	_pMap->doUpdate();

	//�黹���ڴ���
	deallocate();

	return RT_OK;
}

void NFMemMultiHashMap::MainKey::refreshGetList()
{
	assert(_pMap->_pHead->_iGetHead != 0);
	assert(_pMap->_pHead->_iGetTail != 0);

	// �����Ѿ���ͷ��������Ҫ����
	if (_pMap->_pHead->_iGetHead == _iHead)
	{
		return;
	}

	uint32_t iPrev = getHeadPtr()->_iGetPrev;
	uint32_t iNext = getHeadPtr()->_iGetNext;

	assert(iPrev != 0);

	//�����ڱ��ݵ�����
	if (_pMap->_pHead->_iBackupTail == _iHead)
	{
		_pMap->saveValue(&_pMap->_pHead->_iBackupTail, iPrev);
	}

	//��������ͷ��
	_pMap->saveValue(&getHeadPtr()->_iGetNext, _pMap->_pHead->_iGetHead);
	_pMap->saveValue(&getHeadPtr(_pMap->_pHead->_iGetHead)->_iGetPrev, _iHead);
	_pMap->saveValue(&_pMap->_pHead->_iGetHead, _iHead);
	_pMap->saveValue(&getHeadPtr()->_iGetPrev, (uint32_t)0);

	//��һ��Ԫ�ص�Nextָ��ָ����һ��Ԫ��
	_pMap->saveValue(&getHeadPtr(iPrev)->_iGetNext, iNext);

	//��һ��Ԫ�ص�Prevָ����һ��Ԫ��
	if (iNext != 0)
	{
		_pMap->saveValue(&getHeadPtr(iNext)->_iGetPrev, iPrev);
	}
	else
	{
		//�ı�β��ָ��
		assert(_pMap->_pHead->_iGetTail == _iHead);
		_pMap->saveValue(&_pMap->_pHead->_iGetTail, iPrev);
	}
}

int NFMemMultiHashMap::MainKey::allocate(uint32_t iDataLen, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	uint32_t fn = 0;
	//һ�������������������
	fn = getHeadPtr()->_iSize - sizeof(tagMainKeyHead);
	if (fn >= iDataLen)
	{
		//һ��chunk�Ϳ�����, ������chunk��Ҫ�ͷŵ�
		if (HASNEXTCHUNK())
		{
			uint32_t iNextChunk = getHeadPtr()->_iNextChunk;
			//���޸��Լ�������
			_pMap->saveValue(&getHeadPtr()->_iBitset, NEXTCHUNK_BIT, false);
			_pMap->saveValue(&getHeadPtr()->_iDataLen, (uint32_t)0);
			//�޸ĳɹ������ͷ�����, �Ӷ���֤, ����core��ʱ���������ڴ�鲻����
			deallocate(iNextChunk);
		}
		return NFMemMultiHashMap::RT_OK;
	}

	//���㻹��Ҫ���ٳ���
	fn = iDataLen - fn;

	if (HASNEXTCHUNK())
	{
		tagChunkHead *pChunk = getChunkHead(getHeadPtr()->_iNextChunk);

		while (true)
		{
			if (fn <= (pChunk->_iSize - sizeof(tagChunkHead)))
			{
				//�Ѿ�����Ҫchunk��, �ͷŶ����chunk
				if (pChunk->_bNextChunk)
				{
					uint32_t iNextChunk = pChunk->_iNextChunk;
					_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
					//һ���쳣core, �������������˿��õ�����, ���������ڴ�ṹ���ǿ��õ�
					deallocate(iNextChunk);
				}
				return NFMemMultiHashMap::RT_OK;
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
				if (ret != NFMemMultiHashMap::RT_OK)
				{
					//û���ڴ���Է���
					return ret;
				}

				_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)true);
				_pMap->saveValue(&pChunk->_iNextChunk, chunks[0]);
				//chunkָ�����ĵ�һ��chunk
				pChunk = getChunkHead(chunks[0]);
				//�޸ĵ�һ��chunk������, ��֤�쳣core��ʱ��, chunk������������
				_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
				_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);

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
		if (ret != NFMemMultiHashMap::RT_OK)
		{
			//û���ڴ���Է���
			return ret;
		}

		_pMap->saveValue(&getHeadPtr()->_iBitset, NEXTCHUNK_BIT, true);
		_pMap->saveValue(&getHeadPtr()->_iNextChunk, chunks[0]);
		//chunkָ�����ĵ�һ��chunk
		tagChunkHead *pChunk = getChunkHead(chunks[0]);
		//�޸ĵ�һ��chunk������, ��֤�쳣core��ʱ��, chunk������������
		_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
		_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);

		//����ÿ��chunk
		return joinChunk(pChunk, chunks);
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::MainKey::joinChunk(tagChunkHead *pChunk, const std::vector<uint32_t>& chunks)
{
	for (size_t i = 0; i < chunks.size(); ++i)
	{
		if (i == chunks.size() - 1)
		{
			_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
			return NFMemMultiHashMap::RT_OK;
		}
		else
		{
			_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)true);
			_pMap->saveValue(&pChunk->_iNextChunk, chunks[i + 1]);
			pChunk = getChunkHead(chunks[i + 1]);
			_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
			_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);
		}
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::MainKey::allocateChunk(uint32_t fn, std::vector<uint32_t> &chunks, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	assert(fn > 0);

	while (true)
	{
		uint32_t iAllocSize = fn;
		// ����ռ�
		// ��������п��ܻ���̭���ݣ���һ�������ǵ�ǰ����keyͷ��ַ�����ܱ���̭
		uint32_t t = _pMap->_pDataAllocator->allocateChunk(_iHead, iAllocSize, vtData);
		if (t == 0)
		{
			//û���ڴ���Է�����, �Ȱѷ���Ĺ黹
			_pMap->_pDataAllocator->deallocateMemChunk(chunks);
			chunks.clear();
			return NFMemMultiHashMap::RT_NO_MEMORY;
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

	return NFMemMultiHashMap::RT_OK;
}

uint32_t NFMemMultiHashMap::MainKey::getDataLen()
{
	uint32_t n = 0;
	if (!HASNEXTCHUNK())
	{
		// ֻ��һ��chunk
		n += getHeadPtr()->_iDataLen;
		return n;
	}

	//��ǰ��Ĵ�С
	n += getHeadPtr()->_iSize - sizeof(tagMainKeyHead);
	tagChunkHead *pChunk = getChunkHead(getHeadPtr()->_iNextChunk);

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
			// ���һ��chunk
			n += pChunk->_iDataLen;
			break;
		}
	}

	return n;
}

int NFMemMultiHashMap::MainKey::get(void *pData, uint32_t &iDataLen)
{
	if (!HASNEXTCHUNK())
	{
		// û����һ��chunk, һ��chunk�Ϳ���װ��������
		iDataLen = min(getHeadPtr()->_iDataLen, iDataLen);
		memcpy(pData, getHeadPtr()->_cData, iDataLen);
		return NFMemMultiHashMap::RT_OK;
	}
	else
	{
		uint32_t iUseSize = getHeadPtr()->_iSize - sizeof(tagMainKeyHead);
		uint32_t iCopyLen = min(iUseSize, iDataLen);

		// ��copy��һ��chunk������
		memcpy(pData, getHeadPtr()->_cData, iCopyLen);
		if (iDataLen < iUseSize)
		{
			return NFMemMultiHashMap::RT_NOTALL_ERR;   // copy���ݲ���ȫ
		}

		// �Ѿ�copy����
		uint32_t iHasLen = iCopyLen;
		// ���ʣ�೤��
		uint32_t iLeftLen = iDataLen - iCopyLen;


		// copy�������е�chunk�е�����
		tagChunkHead *pChunk = getChunkHead(getHeadPtr()->_iNextChunk);
		while (iHasLen < iDataLen)
		{
			iUseSize = pChunk->_iSize - sizeof(tagChunkHead);
			if (!pChunk->_bNextChunk)
			{
				uint32_t iCopyLen = min(pChunk->_iDataLen, iLeftLen);
				memcpy((char*)pData + iHasLen, pChunk->_cData, iCopyLen);
				iDataLen = iHasLen + iCopyLen;

				if (iLeftLen < pChunk->_iDataLen)
				{
					return NFMemMultiHashMap::RT_NOTALL_ERR;       // copy����ȫ
				}

				return NFMemMultiHashMap::RT_OK;
			}
			else
			{
				uint32_t iCopyLen = min(iUseSize, iLeftLen);
				memcpy((char*)pData + iHasLen, pChunk->_cData, iCopyLen);
				if (iLeftLen <= iUseSize)
				{
					iDataLen = iHasLen + iCopyLen;
					return NFMemMultiHashMap::RT_NOTALL_ERR;   // copy����ȫ
				}

				iHasLen += iCopyLen;
				iLeftLen -= iCopyLen;

				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
		}
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::MainKey::get(std::string &mk)
{
	// ��ȡ���ݳ���
	uint32_t iLen = getDataLen();

	char *cData = new char[iLen];
	uint32_t iGetLen = iLen;
	int ret = get(cData, iGetLen);
	if (ret == NFMemMultiHashMap::RT_OK)
	{
		// �����string
		mk.assign(cData, iGetLen);
	}

	delete[] cData;

	return ret;
}

int NFMemMultiHashMap::MainKey::set(const void *pData, uint32_t iDataLen, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	// ���ȷ���ոչ��ĳ���, ���ܶ�һ��chunk, Ҳ������һ��chunk
	// allocate���жϵ�ǰchunk�ĳ����Ƿ�����iDataLen�����˾ͼ�chunk�����˾��ͷ�chunk
	int ret = allocate(iDataLen, vtData);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	// ��һ��chunk����Ч����
	uint32_t iUseSize = getHeadPtr()->_iSize - sizeof(tagMainKeyHead);
	if (!HASNEXTCHUNK())
	{
		// û����һ��chunk, һ��chunk�Ϳ���װ��������
		// ��copy����, �ٸ������ݳ���
		memcpy(getHeadPtr()->_cData, (char*)pData, iDataLen);
		getHeadPtr()->_iDataLen = iDataLen;
	}
	else
	{
		// copy����ǰ��chunk��
		memcpy(getHeadPtr()->_cData, (char*)pData, iUseSize);
		// ʣ��̶�
		uint32_t iLeftLen = iDataLen - iUseSize;
		uint32_t iCopyLen = iUseSize;

		tagChunkHead *pChunk = getChunkHead(getHeadPtr()->_iNextChunk);
		while (true)
		{
			// ����chunk�Ŀ��ô�С
			iUseSize = pChunk->_iSize - sizeof(tagChunkHead);

			if (!pChunk->_bNextChunk)
			{
				assert(iUseSize >= iLeftLen);
				// copy����ǰ��chunk��
				memcpy(pChunk->_cData, (char*)pData + iCopyLen, iLeftLen);
				// ���һ��chunk, �������ݳ���, ��copy�����ٸ�ֵ����
				pChunk->_iDataLen = iLeftLen;
				iCopyLen += iLeftLen;
				iLeftLen -= iLeftLen;
				break;
			}
			else
			{
				// copy����ǰ��chunk��
				memcpy(pChunk->_cData, (char*)pData + iCopyLen, iUseSize);
				iCopyLen += iUseSize;
				iLeftLen -= iUseSize;

				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
		}
		assert(iLeftLen == 0);
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::Block::getBlockData(NFMemMultiHashMap::BlockData &data)
{
	data._dirty = isDirty();
	data._synct = getSyncTime();
	data._iVersion = getVersion();

	std::string s;
	int ret = get(s);

	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	try
	{
		// block����������У���һ����Ϊ����key�����������
		TC_PackOut po(s.c_str(), s.length());
		po >> data._key;

		// �������ֻ��Key
		if (!isOnlyKey())
		{
			// �ڶ�����Ϊ����ֵ
			po >> data._value;
		}
		else
		{
			return NFMemMultiHashMap::RT_ONLY_KEY;
		}
	}
	catch (std::exception &ex)
	{
		ret = NFMemMultiHashMap::RT_DECODE_ERR;
	}

	return ret;
}

uint32_t NFMemMultiHashMap::Block::getLastBlockHead(bool bUKList)
{
	uint32_t iHead = _iHead;

	if (bUKList)
	{
		// ��ȡ��ǰ��������block���ϵ����һ��block
		while (getBlockHead(iHead)->_iUKBlockNext != 0)
		{
			iHead = getBlockHead(iHead)->_iUKBlockNext;
		}
	}
	else
	{
		// ��ȡ��ǰ��key block���ϵ����һ��block
		while (getBlockHead(iHead)->_iMKBlockNext != 0)
		{
			iHead = getBlockHead(iHead)->_iMKBlockNext;
		}
	}

	return iHead;
}

int NFMemMultiHashMap::Block::get(void *pData, uint32_t &iDataLen)
{
	if (!HASNEXTCHUNK())
	{
		// û����һ��chunk, һ��chunk�Ϳ���װ��������
		memcpy(pData, getBlockHead()->_cData, min(getBlockHead()->_iDataLen, iDataLen));
		iDataLen = getBlockHead()->_iDataLen;
		return NFMemMultiHashMap::RT_OK;
	}
	else
	{
		// ��һ��chunk����Ч�ռ䳤��
		uint32_t iUseSize = getBlockHead()->_iSize - sizeof(tagBlockHead);
		uint32_t iCopyLen = min(iUseSize, iDataLen);

		// copy��һ��chunk�е�����
		memcpy(pData, getBlockHead()->_cData, iCopyLen);
		if (iDataLen < iUseSize)
		{
			// �ⲿ�ṩ�Ļ��������Ȳ���
			return NFMemMultiHashMap::RT_NOTALL_ERR;   //copy���ݲ���ȫ
		}

		// �Ѿ�copy����
		uint32_t iHasLen = iCopyLen;
		// ���ʣ�೤��
		uint32_t iLeftLen = iDataLen - iCopyLen;

		tagChunkHead *pChunk = getChunkHead(getBlockHead()->_iNextChunk);
		while (iHasLen < iDataLen)
		{
			iUseSize = pChunk->_iSize - sizeof(tagChunkHead);
			if (!pChunk->_bNextChunk)
			{
				// ���һ��chunk
				uint32_t iCopyLen = min(pChunk->_iDataLen, iLeftLen);
				memcpy((char*)pData + iHasLen, pChunk->_cData, iCopyLen);
				iDataLen = iHasLen + iCopyLen;

				if (iLeftLen < pChunk->_iDataLen)
				{
					return NFMemMultiHashMap::RT_NOTALL_ERR;       // copy����ȫ
				}

				return NFMemMultiHashMap::RT_OK;
			}
			else
			{
				uint32_t iCopyLen = min(iUseSize, iLeftLen);
				// copy��ǰ��chunk
				memcpy((char*)pData + iHasLen, pChunk->_cData, iCopyLen);
				if (iLeftLen <= iUseSize)
				{
					iDataLen = iHasLen + iCopyLen;
					return NFMemMultiHashMap::RT_NOTALL_ERR;   // copy����ȫ
				}

				// copy��ǰchunk��ȫ
				iHasLen += iCopyLen;
				iLeftLen -= iCopyLen;

				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
		}
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::Block::get(std::string &s)
{
	uint32_t iLen = getDataLen();

	char *cData = new char[iLen];
	uint32_t iGetLen = iLen;
	int ret = get(cData, iGetLen);
	if (ret == NFMemMultiHashMap::RT_OK)
	{
		s.assign(cData, iGetLen);
	}

	delete[] cData;

	return ret;
}

int NFMemMultiHashMap::Block::set(const void *pData, uint32_t iDataLen, bool bOnlyKey,
	uint8_t iVersion, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	// versionΪ0��ʾ�ⲿ���������ݰ汾��onlykeyʱҲ����Ҫ��ע���ݰ汾
	if (iVersion != 0 && !isOnlyKey() && getBlockHead()->_iVersion != iVersion)
	{
		// ���ݰ汾��ƥ��
		return NFMemMultiHashMap::RT_DATA_VER_MISMATCH;
	}

	// ���ȷ���ոչ��ĳ���, ���ܶ�һ��chunk, Ҳ������һ��chunk
	// allocate���жϵ�ǰchunk�ĳ����Ƿ�����iDataLen�����˾ͼ�chunk�����˾��ͷ�chunk
	int ret = allocate(iDataLen, vtData);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	if (bOnlyKey)
	{
		// ԭʼ������������
		if (isDirty())
		{
			_pMap->delDirtyCount();
		}

		// ���ݱ��޸�, ����Ϊ������
		SETDIRTY(false);

		// ԭʼ���ݲ���OnlyKey����
		if (!isOnlyKey())
		{
			_pMap->incOnlyKeyCount();
		}
	}
	else
	{
		//ԭʼ���ݲ���������
		if (!isDirty())
		{
			_pMap->incDirtyCount();
		}

		//���ݱ��޸�, ����Ϊ������
		SETDIRTY(true);

		//ԭʼ������OnlyKey����
		if (isOnlyKey())
		{
			_pMap->delOnlyKeyCount();
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
	SETONLYKEY(bOnlyKey);

	// ��һ��chunk����Ч�ռ��С
	uint32_t iUseSize = getBlockHead()->_iSize - sizeof(tagBlockHead);
	if (!HASNEXTCHUNK())
	{
		// û����һ��chunk, һ��chunk�Ϳ���װ��������
		memcpy(getBlockHead()->_cData, (char*)pData, iDataLen);
		getBlockHead()->_iDataLen = iDataLen;
	}
	else
	{
		// copy����һ��chunk��
		memcpy(getBlockHead()->_cData, (char*)pData, iUseSize);
		// ʣ��̶�
		uint32_t iLeftLen = iDataLen - iUseSize;
		uint32_t iCopyLen = iUseSize;

		tagChunkHead *pChunk = getChunkHead(getBlockHead()->_iNextChunk);
		while (true)
		{
			// ����chunk�Ŀ��ô�С
			iUseSize = pChunk->_iSize - sizeof(tagChunkHead);

			if (!pChunk->_bNextChunk)
			{
				assert(iUseSize >= iLeftLen);
				// copy����ǰ��chunk��
				memcpy(pChunk->_cData, (char*)pData + iCopyLen, iLeftLen);
				// ���һ��chunk, �������ݳ���, ��copy�����ٸ�ֵ����
				pChunk->_iDataLen = iLeftLen;
				iCopyLen += iLeftLen;
				iLeftLen -= iLeftLen;
				break;
			}
			else
			{
				// copy����ǰ��chunk��
				memcpy(pChunk->_cData, (char*)pData + iCopyLen, iUseSize);
				iCopyLen += iUseSize;
				iLeftLen -= iUseSize;

				pChunk = getChunkHead(pChunk->_iNextChunk);
			}
		}
		assert(iLeftLen == 0);
	}

	return NFMemMultiHashMap::RT_OK;
}

void NFMemMultiHashMap::Block::setDirty(bool b)
{
	if (b)
	{
		if (!isDirty())
		{
			SETDIRTY(b);
			_pMap->incDirtyCount();
		}
	}
	else
	{
		if (isDirty())
		{
			SETDIRTY(b);
			_pMap->delDirtyCount();
		}
	}
}

bool NFMemMultiHashMap::Block::nextBlock()
{
	_iHead = getBlockHead()->_iUKBlockNext;
	_pHead = _pMap->getAbsolute(_iHead);

	return _iHead != 0;
}

bool NFMemMultiHashMap::Block::prevBlock()
{
	_iHead = getBlockHead()->_iUKBlockPrev;
	_pHead = _pMap->getAbsolute(_iHead);

	return _iHead != 0;
}

void NFMemMultiHashMap::Block::deallocate()
{
	// ���ͷ����еĺ���chunk
	if (HASNEXTCHUNK())
	{
		deallocate(getBlockHead()->_iNextChunk);
	}

	// ���ͷŵ�һ��chunk
	std::vector<uint32_t> v;
	v.push_back(_iHead);
	_pMap->_pDataAllocator->deallocateMemChunk(v);
}

void NFMemMultiHashMap::Block::makeNew(uint32_t iMainKeyAddr, uint32_t uIndex, uint32_t iAllocSize, bool bHead)
{
	getBlockHead()->_iSize = iAllocSize;
	getBlockHead()->_iIndex = uIndex;
	getBlockHead()->_iUKBlockNext = 0;
	getBlockHead()->_iUKBlockPrev = 0;
	getBlockHead()->_iMKBlockNext = 0;
	getBlockHead()->_iMKBlockPrev = 0;
	getBlockHead()->_iSetNext = 0;
	getBlockHead()->_iSetPrev = 0;
	getBlockHead()->_iMainKey = iMainKeyAddr;
	getBlockHead()->_iSyncTime = 0;
	getBlockHead()->_iDataLen = 0;
	getBlockHead()->_iVersion = 1;        // ��Ч�汾��Χ1-255
	SETONLYKEY(false);
	SETDIRTY(true);
	SETNEXTCHUNK(false);

	_pMap->incDirtyCount();
	_pMap->incElementCount();
	_pMap->incListCount(uIndex);

	// ������key�����block����
	MainKey mainKey(_pMap, iMainKeyAddr);
	_pMap->saveValue(&mainKey.getHeadPtr()->_iBlockCount, mainKey.getHeadPtr()->_iBlockCount + 1);
	_pMap->updateMaxMainKeyBlockCount(mainKey.getHeadPtr()->_iBlockCount);

	// ����block������
	if (_pMap->item(uIndex)->_iBlockAddr == 0)
	{
		// ��ǰhashͰû��Ԫ��
		_pMap->saveValue(&_pMap->item(uIndex)->_iBlockAddr, _iHead);
		_pMap->saveValue(&getBlockHead()->_iUKBlockNext, (uint32_t)0);
		_pMap->saveValue(&getBlockHead()->_iUKBlockPrev, (uint32_t)0);
	}
	else
	{
		// ��ǰhashͰ��Ԫ��, ����Ͱ��ͷ
		_pMap->saveValue(&getBlockHead(_pMap->item(uIndex)->_iBlockAddr)->_iUKBlockPrev, _iHead);
		_pMap->saveValue(&getBlockHead()->_iUKBlockNext, _pMap->item(uIndex)->_iBlockAddr);
		_pMap->saveValue(&_pMap->item(uIndex)->_iBlockAddr, _iHead);
		_pMap->saveValue(&getBlockHead()->_iUKBlockPrev, (uint32_t)0);
	}

	// ������key����
	if (mainKey.getHeadPtr()->_iAddr == 0)
	{
		// ��ǰ��key���ϻ�û��Ԫ��
		_pMap->saveValue(&mainKey.getHeadPtr()->_iAddr, _iHead);
		_pMap->saveValue(&getBlockHead()->_iMKBlockNext, (uint32_t)0);
		_pMap->saveValue(&getBlockHead()->_iMKBlockPrev, (uint32_t)0);
	}
	else
	{
		// ��ǰ��key�����Ѿ���Ԫ��
		if (bHead)
		{
			// ������ǰ��
			_pMap->saveValue(&getBlockHead(mainKey.getHeadPtr()->_iAddr)->_iMKBlockPrev, _iHead);
			_pMap->saveValue(&getBlockHead()->_iMKBlockNext, mainKey.getHeadPtr()->_iAddr);
			_pMap->saveValue(&mainKey.getHeadPtr()->_iAddr, _iHead);
			_pMap->saveValue(&getBlockHead()->_iMKBlockPrev, (uint32_t)0);
		}
		else
		{
			// �ҵ������
			Block blkHead(_pMap, mainKey.getHeadPtr()->_iAddr);
			uint32_t iLast = blkHead.getLastBlockHead(false);
			_pMap->saveValue(&getBlockHead(iLast)->_iMKBlockNext, _iHead);
			_pMap->saveValue(&getBlockHead()->_iMKBlockNext, (uint32_t)0);
			_pMap->saveValue(&getBlockHead()->_iMKBlockPrev, iLast);
		}
	}

	// ����Set�����ͷ��
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
}

void NFMemMultiHashMap::Block::erase()
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

	////////////////////�޸�Set���������//////////
	{
		bool bHead = (_pMap->_pHead->_iSetHead == _iHead);
		bool bTail = (_pMap->_pHead->_iSetTail == _iHead);

		if (!bHead)
		{
			if (bTail)
			{
				assert(getBlockHead()->_iSetNext == 0);
				// ��β��, β��ָ��ָ����һ��Ԫ��
				_pMap->saveValue(&_pMap->_pHead->_iSetTail, getBlockHead()->_iSetPrev);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetPrev)->_iSetNext, (uint32_t)0);
			}
			else
			{
				// ����ͷ��Ҳ����β��
				assert(getBlockHead()->_iSetNext != 0);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetPrev)->_iSetNext, getBlockHead()->_iSetNext);
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetNext)->_iSetPrev, getBlockHead()->_iSetPrev);
			}
		}
		else
		{
			if (bTail)
			{
				assert(getBlockHead()->_iSetNext == 0);
				assert(getBlockHead()->_iSetPrev == 0);
				// ͷ��Ҳ��β��, ָ�붼����Ϊ0
				_pMap->saveValue(&_pMap->_pHead->_iSetHead, (uint32_t)0);
				_pMap->saveValue(&_pMap->_pHead->_iSetTail, (uint32_t)0);
			}
			else
			{
				// ͷ������β��, ͷ��ָ��ָ����һ��Ԫ��
				assert(getBlockHead()->_iSetNext != 0);
				_pMap->saveValue(&_pMap->_pHead->_iSetHead, getBlockHead()->_iSetNext);
				// ��һ��Ԫ����ָ��Ϊ0
				_pMap->saveValue(&getBlockHead(getBlockHead()->_iSetNext)->_iSetPrev, (uint32_t)0);
			}
		}
	}

	////////////////////�޸���key���������//////////
	//
	{
		MainKey mainKey(_pMap, getBlockHead()->_iMainKey);
		if (getBlockHead()->_iMKBlockPrev != 0)
		{
			// ����һ��blockָ����һ��
			_pMap->saveValue(&getBlockHead(getBlockHead()->_iMKBlockPrev)->_iMKBlockNext, getBlockHead()->_iMKBlockNext);
		}
		else
		{
			// ��key���ϵĵ�һ��
			_pMap->saveValue(&mainKey.getHeadPtr()->_iAddr, getBlockHead()->_iMKBlockNext);
		}
		if (getBlockHead()->_iMKBlockNext != 0)
		{
			// ����һ��blockָ����һ��
			_pMap->saveValue(&getBlockHead(getBlockHead()->_iMKBlockNext)->_iMKBlockPrev, getBlockHead()->_iMKBlockPrev);
		}
	}

	///////////////////��block������ȥ��///////////
	//
	//��һ��blockָ����һ��block
	if (getBlockHead()->_iUKBlockPrev != 0)
	{
		_pMap->saveValue(&getBlockHead(getBlockHead()->_iUKBlockPrev)->_iUKBlockNext, getBlockHead()->_iUKBlockNext);
	}

	//��һ��blockָ����һ��
	if (getBlockHead()->_iUKBlockNext != 0)
	{
		_pMap->saveValue(&getBlockHead(getBlockHead()->_iUKBlockNext)->_iUKBlockPrev, getBlockHead()->_iUKBlockPrev);
	}

	//////////////////�����hashͷ��, ��Ҫ�޸�hash��������ָ��//////
	//
	_pMap->delListCount(getBlockHead()->_iIndex);
	if (getBlockHead()->_iUKBlockPrev == 0)
	{
		//�����hashͰ��ͷ��, ����Ҫ����
		NFMemMultiHashMap::tagHashItem *pItem = _pMap->item(getBlockHead()->_iIndex);
		assert(pItem->_iBlockAddr == _iHead);
		if (pItem->_iBlockAddr == _iHead)
		{
			_pMap->saveValue(&pItem->_iBlockAddr, getBlockHead()->_iUKBlockNext);
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

	// ������key�����ݸ���
	MainKey mainKey(_pMap, getBlockHead()->_iMainKey);
	_pMap->saveValue(&mainKey.getHeadPtr()->_iBlockCount, mainKey.getHeadPtr()->_iBlockCount - 1);
	_pMap->updateMaxMainKeyBlockCount(mainKey.getHeadPtr()->_iBlockCount);

	// �ֹ��ÿ�һЩ������ʹ�����ݻָ���������¼���Ա�crash��ָ�
	_pMap->saveValue(&getBlockHead()->_iSize, (uint32_t)0);
	_pMap->saveValue(&getBlockHead()->_iIndex, (uint32_t)0);
	_pMap->saveValue(&getBlockHead()->_iSyncTime, (time_t)0);
	_pMap->saveValue(&getBlockHead()->_iBitset, (uint8_t)0);
	_pMap->saveValue(&getBlockHead()->_iDataLen, (uint32_t)0);
	// �ر�أ�����һ��Ҫ�ֹ���blockָ���mainkeyΪ�գ�����crash��Ļָ����޷�������block��mainkey����ϵ
	_pMap->saveValue(&getBlockHead()->_iMainKey, (uint32_t)0);

	// �黹�ڴ�ǰ��ȷ�������޸ģ���Ϊ�ڴ汻�ͷź��ǲ��ָܻ���
	// ��ʹ������ڴ��ͷ�ʧ��Ҳ����������ݵ��ƻ���ֻ�����һ�����ݵĶ�ʧ
	_pMap->doUpdate();

	//�黹���ڴ���
	deallocate();
}

void NFMemMultiHashMap::Block::refreshSetList()
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
		MainKey mainKey(_pMap, getBlockHead()->_iMainKey);
		mainKey.refreshGetList();
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
	MainKey mainKey(_pMap, getBlockHead()->_iMainKey);
	mainKey.refreshGetList();
}


void NFMemMultiHashMap::Block::deallocate(uint32_t iChunk)
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
	_pMap->_pDataAllocator->deallocateMemChunk(v);
}

int NFMemMultiHashMap::Block::allocate(uint32_t iDataLen, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	uint32_t fn = 0;

	// ��һ��chunk����Ч��������
	fn = getBlockHead()->_iSize - sizeof(tagBlockHead);
	if (fn >= iDataLen)
	{
		// һ��chunk�Ϳ�����, ������chunk��Ҫ�ͷŵ�
		if (HASNEXTCHUNK())
		{
			uint32_t iNextChunk = getBlockHead()->_iNextChunk;
			// ���޸��Լ�������
			_pMap->saveValue(&getBlockHead()->_iBitset, NEXTCHUNK_BIT, false);
			_pMap->saveValue(&getBlockHead()->_iDataLen, (uint32_t)0);
			// �޸ĳɹ������ͷ�����, �Ӷ���֤, ����core��ʱ���������ڴ�鲻����
			deallocate(iNextChunk);
		}
		return NFMemMultiHashMap::RT_OK;
	}

	// ���㻹��Ҫ���ٳ���
	fn = iDataLen - fn;

	if (HASNEXTCHUNK())
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
					_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
					//һ���쳣core, �������������˿��õ�����, ���������ڴ�ṹ���ǿ��õ�
					deallocate(iNextChunk);
				}
				return NFMemMultiHashMap::RT_OK;
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
				if (ret != NFMemMultiHashMap::RT_OK)
				{
					//û���ڴ���Է���
					return ret;
				}

				_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)true);
				_pMap->saveValue(&pChunk->_iNextChunk, chunks[0]);
				//chunkָ�����ĵ�һ��chunk
				pChunk = getChunkHead(chunks[0]);
				//�޸ĵ�һ��chunk������, ��֤�쳣core��ʱ��, chunk������������
				_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
				_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);

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
		if (ret != NFMemMultiHashMap::RT_OK)
		{
			//û���ڴ���Է���
			return ret;
		}

		_pMap->saveValue(&getBlockHead()->_iBitset, NEXTCHUNK_BIT, true);
		_pMap->saveValue(&getBlockHead()->_iNextChunk, chunks[0]);
		//chunkָ�����ĵ�һ��chunk
		tagChunkHead *pChunk = getChunkHead(chunks[0]);
		//�޸ĵ�һ��chunk������, ��֤�쳣core��ʱ��, chunk������������
		_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
		_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);

		//����ÿ��chunk
		return joinChunk(pChunk, chunks);
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::Block::joinChunk(tagChunkHead *pChunk, const std::vector<uint32_t>& chunks)
{
	for (size_t i = 0; i < chunks.size(); ++i)
	{
		if (i == chunks.size() - 1)
		{
			_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
			return NFMemMultiHashMap::RT_OK;
		}
		else
		{
			_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)true);
			_pMap->saveValue(&pChunk->_iNextChunk, chunks[i + 1]);
			pChunk = getChunkHead(chunks[i + 1]);
			_pMap->saveValue(&pChunk->_bNextChunk, (uint8_t)false);
			_pMap->saveValue(&pChunk->_iDataLen, (uint32_t)0);
		}
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::Block::allocateChunk(uint32_t fn, std::vector<uint32_t> &chunks, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	assert(fn > 0);

	while (true)
	{
		uint32_t iAllocSize = fn;
		// ����ռ�
		// ���ڷ����block��������key�ǲ��ܱ���̭��
		uint32_t t = _pMap->_pDataAllocator->allocateChunk(getBlockHead()->_iMainKey, iAllocSize, vtData);
		if (t == 0)
		{
			//û���ڴ���Է�����, �Ȱѷ���Ĺ黹
			_pMap->_pDataAllocator->deallocateMemChunk(chunks);
			chunks.clear();
			return NFMemMultiHashMap::RT_NO_MEMORY;
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

	return NFMemMultiHashMap::RT_OK;
}

uint32_t NFMemMultiHashMap::Block::getDataLen()
{
	uint32_t n = 0;
	if (!HASNEXTCHUNK())
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

uint32_t NFMemMultiHashMap::BlockAllocator::allocateMemBlock(uint32_t iMainKeyAddr, uint32_t index, bool bHead,
	uint32_t &iAllocSize, std::vector<NFMemMultiHashMap::Value> &vtData)
{
begin:
	size_t iAddr = 0;
	size_t bigSize = iAllocSize;
	_pChunkAllocator->allocate2(bigSize, bigSize, iAddr);
	iAllocSize = (uint32_t)bigSize;
	if (iAddr == 0)
	{
		size_t ret = _pMap->eraseExcept(iMainKeyAddr, vtData);
		if (ret == 0)
		{
			return 0;     //û�пռ�����ͷ���
		}
		goto begin;
	}

	// ��ʼ��blockͷ����Ϣ
	Block block(_pMap, (uint32_t)iAddr);
	block.makeNew(iMainKeyAddr, index, iAllocSize, bHead);

	_pMap->incChunkCount();

	return (uint32_t)iAddr;
}

uint32_t NFMemMultiHashMap::BlockAllocator::allocateMainKeyHead(uint32_t index, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	size_t iAllocSize = sizeof(MainKey::tagMainKeyHead);
begin:
	size_t iAddr = 0;
	_pChunkAllocator->allocate2(iAllocSize, iAllocSize, iAddr);
	if (iAddr == 0)
	{
		size_t ret = _pMap->eraseExcept(0, vtData);
		if (ret == 0)
		{
			return 0;     //û�пռ�����ͷ���
		}
		goto begin;
	}

	// ������µ�MemBlock, ��ʼ��һ��
	MainKey mainKey(_pMap, (uint32_t)iAddr);
	mainKey.makeNew(index, (uint32_t)iAllocSize);

	_pMap->incChunkCount();

	return (uint32_t)iAddr;
}


uint32_t NFMemMultiHashMap::BlockAllocator::allocateChunk(uint32_t iAddr, uint32_t &iAllocSize, std::vector<NFMemMultiHashMap::Value> &vtData)
{
begin:
	size_t iChunkAddr = 0;
	size_t bigSize = iAllocSize;
	_pChunkAllocator->allocate2(bigSize, bigSize, iChunkAddr);
	iAllocSize = (uint32_t)bigSize;
	if (iChunkAddr == 0)
	{
		size_t ret = _pMap->eraseExcept(iAddr, vtData);
		if (ret == 0)
		{
			return 0;     //û�пռ�����ͷ���
		}
		goto begin;
	}

	_pMap->incChunkCount();

	return iChunkAddr;
}

void NFMemMultiHashMap::BlockAllocator::deallocateMemChunk(const std::vector<uint32_t> &v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		_pChunkAllocator->deallocate2(v[i]);
		_pMap->delChunkCount();
	}
}

void NFMemMultiHashMap::BlockAllocator::deallocateMemChunk(uint32_t iChunk)
{
	_pChunkAllocator->deallocate2(iChunk);
	_pMap->delChunkCount();
}

///////////////////////////////////////////////////////////////////

NFMemMultiHashMap::HashMapLockItem::HashMapLockItem(NFMemMultiHashMap *pMap, uint32_t iAddr)
	: _pMap(pMap)
	, _iAddr(iAddr)
{
}

NFMemMultiHashMap::HashMapLockItem::HashMapLockItem(const HashMapLockItem &mcmdi)
	: _pMap(mcmdi._pMap)
	, _iAddr(mcmdi._iAddr)
{
}

NFMemMultiHashMap::HashMapLockItem &NFMemMultiHashMap::HashMapLockItem::operator=(const NFMemMultiHashMap::HashMapLockItem &mcmdi)
{
	if (this != &mcmdi)
	{
		_pMap = mcmdi._pMap;
		_iAddr = mcmdi._iAddr;
	}
	return (*this);
}

bool NFMemMultiHashMap::HashMapLockItem::operator==(const NFMemMultiHashMap::HashMapLockItem &mcmdi)
{
	return _pMap == mcmdi._pMap && _iAddr == mcmdi._iAddr;
}

bool NFMemMultiHashMap::HashMapLockItem::operator!=(const NFMemMultiHashMap::HashMapLockItem &mcmdi)
{
	return _pMap != mcmdi._pMap || _iAddr != mcmdi._iAddr;
}

bool NFMemMultiHashMap::HashMapLockItem::isDirty()
{
	Block block(_pMap, _iAddr);
	return block.isDirty();
}

bool NFMemMultiHashMap::HashMapLockItem::isOnlyKey()
{
	Block block(_pMap, _iAddr);
	return block.isOnlyKey();
}

time_t NFMemMultiHashMap::HashMapLockItem::getSyncTime()
{
	Block block(_pMap, _iAddr);
	return block.getSyncTime();
}

int NFMemMultiHashMap::HashMapLockItem::get(NFMemMultiHashMap::Value &v)
{
	Block block(_pMap, _iAddr);
	MainKey mainKey(_pMap, block.getBlockHead()->_iMainKey);

	// �Ȼ�ȡ��key
	int ret = mainKey.get(v._mkey);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	// ��ȡ����
	ret = block.getBlockData(v._data);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::HashMapLockItem::get(std::string &mk, std::string &uk)
{
	Block block(_pMap, _iAddr);
	MainKey mainKey(_pMap, block.getBlockHead()->_iMainKey);

	// ��ȡ��key
	int ret = mainKey.get(mk);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	// ��ȡ������
	std::string s;
	ret = block.get(s);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	try
	{
		// ��ȡ����key�����������
		TC_PackOut po(s.c_str(), s.length());
		po >> uk;
	}
	catch (std::exception &ex)
	{
		return NFMemMultiHashMap::RT_EXCEPTION_ERR;
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::HashMapLockItem::set(const std::string &mk, const std::string &uk,
	const std::string &v, uint8_t iVersion, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	Block block(_pMap, _iAddr);

	TC_PackIn pi;
	pi << uk;        // ������ֻ���uk������mk����ʡ�ռ�
	pi << v;

	return block.set(pi.topacket().c_str(), pi.topacket().length(), false, iVersion, vtData);
}

int NFMemMultiHashMap::HashMapLockItem::set(const std::string &mk, const std::string &uk, std::vector<NFMemMultiHashMap::Value> &vtData)
{
	Block block(_pMap, _iAddr);

	TC_PackIn pi;
	pi << uk;        // ������ֻ���uk

	return block.set(pi.topacket().c_str(), pi.topacket().length(), true, 0, vtData);
}

bool NFMemMultiHashMap::HashMapLockItem::equal(const std::string &mk, const std::string &uk, NFMemMultiHashMap::Value &v, int &ret)
{
	ret = get(v);

	if ((ret == NFMemMultiHashMap::RT_OK || ret == NFMemMultiHashMap::RT_ONLY_KEY)
		&& mk == v._mkey && uk == v._data._key)
	{
		return true;
	}

	return false;
}

bool NFMemMultiHashMap::HashMapLockItem::equal(const std::string &mk, const std::string &uk, int &ret)
{
	std::string mk1, uk1;
	ret = get(mk1, uk1);

	if (ret == NFMemMultiHashMap::RT_OK && mk == mk1 && uk == uk1)
	{
		return true;
	}

	return false;
}

void NFMemMultiHashMap::HashMapLockItem::nextItem(int iType)
{
	if (_iAddr == 0)
	{
		// ��ͷ��
		return;
	}

	Block block(_pMap, _iAddr);

	if (iType == HashMapLockIterator::IT_BLOCK)
	{
		// ���������������µ�block������
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
		// ��set������
		_iAddr = block.getBlockHead()->_iSetNext;
	}
	else if (iType == HashMapLockIterator::IT_GET)
	{
		// ��get(ͬһ��key�µ�block)������������ȡ��һ��get���ϵ�����
		_iAddr = block.getBlockHead()->_iMKBlockNext;
		if (_iAddr == 0)
		{
			// ��ǰ��key�µ�block���Ѿ�û�������ˣ��ƶ�GET���ϵ���һ����key��
			MainKey mainKey(_pMap, block.getBlockHead()->_iMainKey);
			while (mainKey.getHeadPtr()->_iGetNext)
			{
				mainKey = MainKey(_pMap, mainKey.getHeadPtr()->_iGetNext);
				_iAddr = mainKey.getHeadPtr()->_iAddr;
				if (_iAddr != 0)
				{
					break;
				}
			}
		}
	}
	else if (iType == HashMapLockIterator::IT_MKEY)
	{
		// ��ͬһ��key��block������
		_iAddr = block.getBlockHead()->_iMKBlockNext;
	}
	else if (iType == HashMapLockIterator::IT_UKEY)
	{
		// ��ͬһ��������������block������
		_iAddr = block.getBlockHead()->_iUKBlockNext;
	}
}

void NFMemMultiHashMap::HashMapLockItem::prevItem(int iType)
{
	if (_iAddr == 0)
	{
		// ��ͷ��
		return;
	}

	Block block(_pMap, _iAddr);

	if (iType == HashMapLockIterator::IT_BLOCK)
	{
		// ���������������µ�block������
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
			_iAddr = tmp.getLastBlockHead(true);

			return;
		}

		_iAddr = 0;  //��β����
	}
	else if (iType == HashMapLockIterator::IT_SET)
	{
		// ��set������
		_iAddr = block.getBlockHead()->_iSetPrev;
	}
	else if (iType == HashMapLockIterator::IT_GET)
	{
		// ��get(ͬһ��key�µ�block)������������ȡ��һ��get���ϵ�����
		_iAddr = block.getBlockHead()->_iMKBlockPrev;
		if (_iAddr == 0)
		{
			// ��ǰ��key�µ�block���Ѿ�û�������ˣ��ƶ�get���ϵ���һ����key��
			MainKey mainKey(_pMap, block.getBlockHead()->_iMainKey);
			while (mainKey.getHeadPtr()->_iGetPrev)
			{
				mainKey = MainKey(_pMap, mainKey.getHeadPtr()->_iGetPrev);
				_iAddr = mainKey.getHeadPtr()->_iAddr;
				if (_iAddr != 0)
				{
					// �ƶ�����key���ϵ����һ��block
					Block tmp(_pMap, _iAddr);
					while (tmp.getBlockHead()->_iMKBlockNext)
					{
						_iAddr = tmp.getBlockHead()->_iMKBlockNext;
						tmp = Block(_pMap, _iAddr);
					}
					break;
				}
			}
		}
	}
	else if (iType == HashMapLockIterator::IT_MKEY)
	{
		// ��ͬһ��key��block������
		_iAddr = block.getBlockHead()->_iMKBlockPrev;
	}
	else if (iType == HashMapLockIterator::IT_UKEY)
	{
		// ��ͬһ��������������block������
		_iAddr = block.getBlockHead()->_iUKBlockPrev;
	}
}

///////////////////////////////////////////////////////////////////

NFMemMultiHashMap::HashMapLockIterator::HashMapLockIterator()
	: _pMap(NULL), _iItem(NULL, 0), _iType(IT_BLOCK), _iOrder(IT_NEXT)
{
}

NFMemMultiHashMap::HashMapLockIterator::HashMapLockIterator(NFMemMultiHashMap *pMap, uint32_t iAddr, int iType, int iOrder)
	: _pMap(pMap), _iItem(_pMap, iAddr), _iType(iType), _iOrder(iOrder)
{
}

NFMemMultiHashMap::HashMapLockIterator::HashMapLockIterator(const HashMapLockIterator &it)
	: _pMap(it._pMap), _iItem(it._iItem), _iType(it._iType), _iOrder(it._iOrder)
{
}

NFMemMultiHashMap::HashMapLockIterator& NFMemMultiHashMap::HashMapLockIterator::operator=(const HashMapLockIterator &it)
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

bool NFMemMultiHashMap::HashMapLockIterator::operator==(const HashMapLockIterator& mcmi)
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

bool NFMemMultiHashMap::HashMapLockIterator::operator!=(const HashMapLockIterator& mcmi)
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

NFMemMultiHashMap::HashMapLockIterator& NFMemMultiHashMap::HashMapLockIterator::operator++()
{
	if (_iOrder == IT_NEXT)
	{
		// ˳�����
		_iItem.nextItem(_iType);
	}
	else
	{
		// �������
		_iItem.prevItem(_iType);
	}
	return (*this);

}

NFMemMultiHashMap::HashMapLockIterator NFMemMultiHashMap::HashMapLockIterator::operator++(int)
{
	HashMapLockIterator it(*this);

	if (_iOrder == IT_NEXT)
	{
		// ˳�����
		_iItem.nextItem(_iType);
	}
	else
	{
		// �������
		_iItem.prevItem(_iType);
	}

	return it;

}

///////////////////////////////////////////////////////////////////

NFMemMultiHashMap::HashMapItem::HashMapItem(NFMemMultiHashMap *pMap, uint32_t iIndex)
	: _pMap(pMap)
	, _iIndex(iIndex)
{
}

NFMemMultiHashMap::HashMapItem::HashMapItem(const HashMapItem &mcmdi)
	: _pMap(mcmdi._pMap)
	, _iIndex(mcmdi._iIndex)
{
}

NFMemMultiHashMap::HashMapItem &NFMemMultiHashMap::HashMapItem::operator=(const NFMemMultiHashMap::HashMapItem &mcmdi)
{
	if (this != &mcmdi)
	{
		_pMap = mcmdi._pMap;
		_iIndex = mcmdi._iIndex;
	}
	return (*this);
}

bool NFMemMultiHashMap::HashMapItem::operator==(const NFMemMultiHashMap::HashMapItem &mcmdi)
{
	return _pMap == mcmdi._pMap && _iIndex == mcmdi._iIndex;
}

bool NFMemMultiHashMap::HashMapItem::operator!=(const NFMemMultiHashMap::HashMapItem &mcmdi)
{
	return _pMap != mcmdi._pMap || _iIndex != mcmdi._iIndex;
}

void NFMemMultiHashMap::HashMapItem::get(std::vector<NFMemMultiHashMap::Value> &vtData)
{
	uint32_t iAddr = _pMap->item(_iIndex)->_iBlockAddr;

	while (iAddr != 0)
	{
		Block block(_pMap, iAddr);
		MainKey mainKey(_pMap, block.getBlockHead()->_iMainKey);
		NFMemMultiHashMap::Value data;

		// ��ȡ��key
		int ret = mainKey.get(data._mkey);
		if (ret == NFMemMultiHashMap::RT_OK)
		{
			// ��ȡ��������������
			ret = block.getBlockData(data._data);
			if (ret == NFMemMultiHashMap::RT_OK)
			{
				vtData.push_back(data);
			}
		}

		iAddr = block.getBlockHead()->_iUKBlockNext;
	}
}

void NFMemMultiHashMap::HashMapItem::nextItem()
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

///////////////////////////////////////////////////////////////////

NFMemMultiHashMap::HashMapIterator::HashMapIterator()
	: _pMap(NULL), _iItem(NULL, (uint32_t)-1)
{
}

NFMemMultiHashMap::HashMapIterator::HashMapIterator(NFMemMultiHashMap *pMap, uint32_t iIndex)
	: _pMap(pMap), _iItem(_pMap, iIndex)
{
}

NFMemMultiHashMap::HashMapIterator::HashMapIterator(const HashMapIterator &it)
	: _pMap(it._pMap), _iItem(it._iItem)
{
}

NFMemMultiHashMap::HashMapIterator& NFMemMultiHashMap::HashMapIterator::operator=(const HashMapIterator &it)
{
	if (this != &it)
	{
		_pMap = it._pMap;
		_iItem = it._iItem;
	}

	return (*this);
}

bool NFMemMultiHashMap::HashMapIterator::operator==(const HashMapIterator& mcmi)
{
	if (_iItem.getIndex() != (uint32_t)-1 || mcmi._iItem.getIndex() != (uint32_t)-1)
	{
		return _pMap == mcmi._pMap && _iItem == mcmi._iItem;
	}

	return _pMap == mcmi._pMap;
}

bool NFMemMultiHashMap::HashMapIterator::operator!=(const HashMapIterator& mcmi)
{
	if (_iItem.getIndex() != (uint32_t)-1 || mcmi._iItem.getIndex() != (uint32_t)-1)
	{
		return _pMap != mcmi._pMap || _iItem != mcmi._iItem;
	}

	return _pMap != mcmi._pMap;
}

NFMemMultiHashMap::HashMapIterator& NFMemMultiHashMap::HashMapIterator::operator++()
{
	_iItem.nextItem();
	return (*this);
}

NFMemMultiHashMap::HashMapIterator NFMemMultiHashMap::HashMapIterator::operator++(int)
{
	HashMapIterator it(*this);
	_iItem.nextItem();
	return it;
}

//////////////////////////////////////////////////////////////////////////////////

void NFMemMultiHashMap::init(void *pAddr)
{
	_pHead = static_cast<tagMapHead*>(pAddr);
	_pstModifyHead = static_cast<tagModifyHead*>((void*)((char*)pAddr + sizeof(tagMapHead)));
}

void NFMemMultiHashMap::initDataBlockSize(size_t iMinDataSize, size_t iMaxDataSize, float fFactor)
{
	_iMinDataSize = iMinDataSize;
	_iMaxDataSize = iMaxDataSize;
	_fFactor = fFactor;
}

void NFMemMultiHashMap::create(void *pAddr, size_t iSize)
{
	if (sizeof(tagHashItem)
		+ sizeof(tagMainKeyHashItem)
		+ sizeof(tagMapHead)
		+ sizeof(tagModifyHead)
		+ sizeof(NFMemMultiChunkAllocator::tagChunkAllocatorHead)
		+ 10 > iSize)
	{
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::create] mem size not enougth.");
	}

	if (_iMinDataSize == 0 || _iMaxDataSize == 0 || _fFactor < 1.0)
	{
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::create] init data size error:" + lexical_cast<std::string>(_iMinDataSize) + "|" + lexical_cast<std::string>(_iMaxDataSize) + "|" + lexical_cast<std::string>(_fFactor));
	}

	init(pAddr);

	_pHead->_cMaxVersion = MAX_VERSION;
	_pHead->_cMinVersion = MIN_VERSION;
	_pHead->_bReadOnly = false;
	_pHead->_bAutoErase = true;
	_pHead->_cEraseMode = ERASEBYGET;
	_pHead->_iMemSize = iSize;
	_pHead->_iMinDataSize = _iMinDataSize;
	_pHead->_iMaxDataSize = _iMaxDataSize;
	_pHead->_fFactor = _fFactor;
	_pHead->_fHashRatio = _fHashRatio;
	_pHead->_fMainKeyRatio = _fMainKeyRatio;
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
	_pHead->_iMKOnlyKeyCount = 0;
	_pHead->_iOnlyKeyCount = 0;
	_pHead->_iMaxBlockCount = 0;
	memset(_pHead->_iReserve, 0, sizeof(_pHead->_iReserve));

	size_t iMaxHeadSize = sizeof(Block::tagBlockHead) > sizeof(MainKey::tagMainKeyHead) ?
		sizeof(Block::tagBlockHead) : sizeof(MainKey::tagMainKeyHead);

	// ����ƽ��block��С
	size_t iBlockSize = (_pHead->_iMinDataSize + _pHead->_iMaxDataSize) / 2 + iMaxHeadSize;

	// ���������key Hash����
	size_t iMHashCount = (iSize - sizeof(tagMapHead) - sizeof(tagModifyHead) - sizeof(TC_MemChunkAllocator::tagChunkAllocatorHead)) /
		((size_t)(iBlockSize*_fHashRatio*_fMainKeyRatio) +
			(size_t)(sizeof(tagHashItem)*_fMainKeyRatio) + sizeof(tagMainKeyHashItem));
	// ���������������Ϊhashֵ
	iMHashCount = getMinPrimeNumber(iMHashCount);
	// ����hash����
	size_t iUHashCount = getMinPrimeNumber(size_t(iMHashCount * _fMainKeyRatio));

	void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);

	// ��key hash������
	size_t iHashMemSize = NFMemVector<tagMainKeyHashItem>::calcMemSize(iMHashCount);
	if ((char*)pHashAddr - (char*)_pHead + iHashMemSize > iSize)
	{
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::create] mem size not enougth.");
	}
	_hashMainKey.create(pHashAddr, iHashMemSize);
	pHashAddr = (char*)pHashAddr + _hashMainKey.getMemSize();

	// ����hash������
	iHashMemSize = NFMemVector<tagHashItem>::calcMemSize(iUHashCount);
	if ((char*)pHashAddr - (char*)_pHead + iHashMemSize > iSize)
	{
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::create] mem size not enougth.");
	}
	_hash.create(pHashAddr, iHashMemSize);

	// chunk������
	void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();
	if ((char*)pDataAddr - (char*)_pHead + iMaxHeadSize + _pHead->_iMinDataSize > iSize)
	{
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::create] mem size not enougth.");
	}
	_pDataAllocator->create(pDataAddr, iSize - ((char*)pDataAddr - (char*)_pHead),
		iMaxHeadSize + _pHead->_iMinDataSize, iMaxHeadSize + _pHead->_iMaxDataSize, _pHead->_fFactor);
}

void NFMemMultiHashMap::connect(void *pAddr, size_t iSize)
{
	init(pAddr);

	if (_pHead->_cMaxVersion != MAX_VERSION || _pHead->_cMinVersion != MIN_VERSION)
	{
		// �汾��ƥ��
		std::ostringstream os;
		os << (int)_pHead->_cMaxVersion << "." << (int)_pHead->_cMinVersion << " != " << ((int)MAX_VERSION) << "." << ((int)MIN_VERSION);
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::connect] hash map version not equal:" + os.str() + " (data != code)");
	}

	if (_pHead->_iMemSize != iSize)
	{
		// �ڴ��С��ƥ��
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::connect] hash map size not equal:" + NFCommon::tostr(_pHead->_iMemSize) + "!=" + NFCommon::tostr(iSize));
	}

	// ������key hash��
	void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);
	_hashMainKey.connect(pHashAddr);

	// ��������hash��
	pHashAddr = (char*)pHashAddr + _hashMainKey.getMemSize();
	_hash.connect(pHashAddr);

	// ����chunk��
	void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();
	_pDataAllocator->connect(pDataAddr);

	_iMinDataSize = _pHead->_iMinDataSize;
	_iMaxDataSize = _pHead->_iMaxDataSize;
	_fFactor = _pHead->_fFactor;
	_fHashRatio = _pHead->_fHashRatio;
	_fMainKeyRatio = _pHead->_fMainKeyRatio;

	// �ָ����ܵĴ���
	doRecover();
}

int NFMemMultiHashMap::append(void *pAddr, size_t iSize)
{
	if (iSize <= _pHead->_iMemSize)
	{
		return -1;
	}

	init(pAddr);

	if (_pHead->_cMaxVersion != MAX_VERSION || _pHead->_cMinVersion != MIN_VERSION)
	{
		// �汾��ƥ��
		std::ostringstream os;
		os << (int)_pHead->_cMaxVersion << "." << (int)_pHead->_cMinVersion << " != " << ((int)MAX_VERSION) << "." << ((int)MIN_VERSION);
		throw NF_Multi_HashMap_Exception("[TC_Multi_HashMap::append] hash map version not equal:" + os.str() + " (data != code)");
	}

	_pHead->_iMemSize = iSize;

	// ������key hash��
	void *pHashAddr = (char*)_pHead + sizeof(tagMapHead) + sizeof(tagModifyHead);
	_hashMainKey.connect(pHashAddr);

	// ��������hash��
	pHashAddr = (char*)pHashAddr + _hashMainKey.getMemSize();
	_hash.connect(pHashAddr);

	// ��չchunk��
	void *pDataAddr = (char*)pHashAddr + _hash.getMemSize();
	_pDataAllocator->append(pDataAddr, iSize - ((size_t)pDataAddr - (size_t)pAddr));

	_iMinDataSize = _pHead->_iMinDataSize;
	_iMaxDataSize = _pHead->_iMaxDataSize;
	_fFactor = _pHead->_fFactor;
	_fHashRatio = _pHead->_fHashRatio;
	_fMainKeyRatio = _pHead->_fMainKeyRatio;

	return 0;
}

void NFMemMultiHashMap::clear()
{
	assert(_pHead);

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

	_hashMainKey.clear();
	_hash.clear();

	// �������
	doUpdate();

	_pDataAllocator->rebuild();
}

int NFMemMultiHashMap::dump2file(const std::string &sFile)
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

int NFMemMultiHashMap::load5file(const std::string &sFile)
{
	FILE *fp = fopen(sFile.c_str(), "rb");
	if (fp == NULL)
	{
		return RT_LOAD_FILE_ERR;
	}
	fseek(fp, 0L, SEEK_END);
	size_t fs = ftell(fp);
	if (fs != _pHead->_iMemSize)
	{
		fclose(fp);
		return RT_LOAD_FILE_ERR;
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
				return RT_LOAD_FILE_ERR;
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

	return RT_LOAD_FILE_ERR;
}

int NFMemMultiHashMap::checkMainKey(const std::string &mk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	int ret = RT_OK;
	uint32_t index = mhashIndex(mk);
	if (itemMainKey(index)->_iMainKeyAddr == 0)
	{
		return RT_NO_DATA;
	}

	MainKey mainKey(this, itemMainKey(index)->_iMainKeyAddr);
	do
	{
		std::string s;
		ret = mainKey.get(s);
		if (ret != RT_OK)
		{
			return ret;
		}
		if (s == mk)
		{
			incHitCount();
			// �ҵ���
			if (mainKey.getHeadPtr()->_iAddr == 0)
			{
				return RT_ONLY_KEY;
			}
			if (!mainKey.ISFULLDATA())
			{
				return RT_PART_DATA;
			}
			return RT_OK;
		}

	} while (mainKey.next());

	return RT_NO_DATA;
}

int NFMemMultiHashMap::setFullData(const std::string &mk, bool bFull)
{
	if (_pHead->_bReadOnly) return RT_READONLY;

	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	int ret = RT_OK;
	uint32_t index = mhashIndex(mk);
	if (itemMainKey(index)->_iMainKeyAddr == 0)
	{
		return RT_NO_DATA;
	}

	MainKey mainKey(this, itemMainKey(index)->_iMainKeyAddr);
	do
	{
		std::string s;
		ret = mainKey.get(s);
		if (ret != RT_OK)
		{
			return ret;
		}
		if (s == mk)
		{
			incHitCount();
			// �ҵ���
			mainKey.SETFULLDATA(bFull);
			return RT_OK;
		}

	} while (mainKey.next());

	return RT_NO_DATA;
}

int NFMemMultiHashMap::checkDirty(const std::string &mk, const std::string &uk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	int ret = NFMemMultiHashMap::RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, ret);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	//û������
	if (it == end())
	{
		return NFMemMultiHashMap::RT_NO_DATA;
	}

	incHitCount();

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemMultiHashMap::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	if (block.isDirty())
	{
		return NFMemMultiHashMap::RT_DIRTY_DATA;
	}

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::checkDirty(const std::string &mk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	int ret = RT_OK;
	uint32_t index = mhashIndex(mk);
	if (itemMainKey(index)->_iMainKeyAddr == 0)
	{
		return RT_NO_DATA;
	}

	MainKey mainKey(this, itemMainKey(index)->_iMainKeyAddr);
	do
	{
		std::string s;
		ret = mainKey.get(s);
		if (ret != RT_OK)
		{
			return ret;
		}
		if (s == mk)
		{
			incHitCount();
			// �ҵ���
			if (mainKey.getHeadPtr()->_iAddr == 0)
			{
				ret = RT_ONLY_KEY;
			}
			lock_iterator it(this, mainKey.getHeadPtr()->_iAddr, lock_iterator::IT_MKEY, lock_iterator::IT_NEXT);
			while (it != end())
			{
				if (it->isDirty())
				{
					if (_pHead->_iDirtyTail == 0)
					{
						_pHead->_iDirtyTail = mainKey.getHeadPtr()->_iAddr;
					}
					return RT_DIRTY_DATA;
				}
				it++;
			}
			return RT_OK;
		}

	} while (mainKey.next());

	return RT_NO_DATA;
}

int NFMemMultiHashMap::setDirty(const std::string &mk, const std::string &uk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	incGetCount();

	int ret = NFMemMultiHashMap::RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, ret);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	//û�����ݻ�ֻ��Key
	if (it == end())
	{
		return NFMemMultiHashMap::RT_NO_DATA;
	}

	incHitCount();

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemMultiHashMap::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	block.setDirty(true);
	block.refreshSetList();

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::setClean(const std::string &mk, const std::string &uk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	incGetCount();

	int ret = NFMemMultiHashMap::RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, ret);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	//û�����ݻ�ֻ��Key
	if (it == end())
	{
		return NFMemMultiHashMap::RT_NO_DATA;
	}

	incHitCount();

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemMultiHashMap::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	block.setDirty(false);
	block.refreshSetList();

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::setSyncTime(const std::string &mk, const std::string &uk, time_t iSyncTime)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	if (_pHead->_bReadOnly) return RT_READONLY;

	incGetCount();

	int ret = NFMemMultiHashMap::RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, ret);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	//û�����ݻ�ֻ��Key
	if (it == end())
	{
		return NFMemMultiHashMap::RT_NO_DATA;
	}

	incHitCount();

	//ֻ��Key
	if (it->isOnlyKey())
	{
		return NFMemMultiHashMap::RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	block.setSyncTime(iSyncTime);

	return NFMemMultiHashMap::RT_OK;
}

void NFMemMultiHashMap::incMainKeyBlockCount(const std::string &mk, bool bInc)
{
	uint32_t index = mhashIndex(mk);

	// �ҵ���������keyͷ
	if (itemMainKey(index)->_iMainKeyAddr != 0)
	{
		MainKey mainKey(this, itemMainKey(index)->_iMainKeyAddr);
		do
		{
			std::string sKey;
			mainKey.get(sKey);
			if (mk == sKey)
			{
				// �ҵ��ˣ����Ӽ���
				if (bInc)
				{
					saveValue(&mainKey.getHeadPtr()->_iBlockCount, mainKey.getHeadPtr()->_iBlockCount + 1);
					updateMaxMainKeyBlockCount(mainKey.getHeadPtr()->_iBlockCount + 1);
				}
				else
				{
					saveValue(&mainKey.getHeadPtr()->_iBlockCount, mainKey.getHeadPtr()->_iBlockCount - 1);
					updateMaxMainKeyBlockCount(mainKey.getHeadPtr()->_iBlockCount - 1);
				}
				break;
			}
		} while (mainKey.next());
	}
}

void NFMemMultiHashMap::updateMaxMainKeyBlockCount(size_t iCount)
{
	if (iCount > _pHead->_iMaxBlockCount)
	{
		saveValue(&_pHead->_iMaxBlockCount, iCount);
	}
}

int NFMemMultiHashMap::get(const std::string &mk, const std::string &uk, Value &v)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	int ret = RT_OK;

	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, v, ret);

	if (ret != RT_OK && ret != RT_ONLY_KEY)
	{
		return ret;
	}

	if (it == end())
	{
		// û������
		// ��ѯ��key��Ϣ
		uint32_t mIndex = mhashIndex(mk);
		uint32_t iAddr = find(mk, mIndex, ret);
		if (ret != RT_OK)
		{
			// �����п��ܷ���RT_ONLY_KEY
			return ret;
		}
		if (iAddr != 0)
		{
			MainKey mainKey(this, iAddr);
			if (mainKey.ISFULLDATA())
			{
				// ��key���ڣ�����cache�е����ݱ���������Դ(�����ݿ�)����һ��
				// ˵������Դ�ض�û�д˼�¼������only key���ɣ�����ʹ�õ����߲���
				// ������Դ��ȡ����
				return RT_ONLY_KEY;
			}
			// �����key���ڣ������ݲ�ȫ�����п��ܻ������ݿ����У�Ӧ�÷���RT_NO_DATA
		}
		return RT_NO_DATA;
	}

	incHitCount();

	// ֻ��Key
	if (it->isOnlyKey())
	{
		return RT_ONLY_KEY;
	}

	Block block(this, it->getAddr());
	// ���ֻ��, ��ˢ��get����
	if (!_pHead->_bReadOnly)
	{
		MainKey mainKey(this, block.getBlockHead()->_iMainKey);
		mainKey.refreshGetList();
	}
	return RT_OK;
}

int NFMemMultiHashMap::get(const std::string &mk, std::vector<Value> &vs)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	int ret = NFMemMultiHashMap::RT_OK;

	uint32_t index = mhashIndex(mk);
	uint32_t iMainKeyAddr = find(mk, index, ret);
	if (ret != RT_OK)
	{
		return ret;
	}
	if (iMainKeyAddr == 0)
	{
		// ��key������
		return RT_NO_DATA;
	}

	incHitCount();

	MainKey mainKey(this, iMainKeyAddr);
	if (mainKey.getHeadPtr()->_iAddr == 0)
	{
		return RT_ONLY_KEY;
	}
	uint32_t iAddr = mainKey.getHeadPtr()->_iAddr;
	while (iAddr != 0)
	{
		Block block(this, iAddr);
		Value v;
		v._mkey = mk;
		ret = block.getBlockData(v._data);
		if (ret != RT_OK && ret != RT_ONLY_KEY)
		{
			return ret;
		}
		if (ret == RT_OK)
		{
			// ��ȡ��only key������
			vs.push_back(v);
		}
		iAddr = block.getBlockHead()->_iMKBlockNext;
	}

	if (!mainKey.ISFULLDATA())
	{
		return RT_PART_DATA;
	}

	return RT_OK;
}

int NFMemMultiHashMap::get(uint32_t &mh, std::map<std::string, std::vector<Value> > &vs)
{
	int ret = NFMemMultiHashMap::RT_OK;

	uint32_t index = mh % _hashMainKey.size();
	uint32_t iMainKeyAddr = itemMainKey(index)->_iMainKeyAddr;
	if (iMainKeyAddr == 0)
	{
		return RT_OK;
	}

	MainKey mainKey(this, iMainKeyAddr);
	do
	{
		if (mainKey.getHeadPtr()->_iAddr != 0)
		{
			lock_iterator it(this, mainKey.getHeadPtr()->_iAddr, lock_iterator::IT_MKEY, lock_iterator::IT_NEXT);
			while (it != end())
			{
				Value v;
				ret = it->get(v);
				if (ret != RT_OK && ret != RT_ONLY_KEY)
				{
					return ret;
				}
				if (ret == RT_OK)
				{
					// ��ȡ��only key������
					vs[v._mkey].push_back(v);
				}
				it++;
			}
		}
	} while (mainKey.next());

	return RT_OK;
}

int NFMemMultiHashMap::set(const std::string &mk, const std::string &uk, const std::string &v,
	uint8_t iVersion, bool bDirty, DATATYPE eType, bool bHead, std::vector<Value> &vtData)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int ret = NFMemMultiHashMap::RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, ret);
	bool bNewBlock = false;

	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	incHitCount();

	if (it == end())
	{
		// �����в�����

		// �����Ƿ��Ѿ���������key��
		uint32_t mIndex = mhashIndex(mk);        // ��key����
		uint32_t iMainKeyAddr = find(mk, mIndex, ret);
		if (ret != RT_OK && ret != RT_ONLY_KEY)
		{
			return ret;
		}
		if (iMainKeyAddr == 0)
		{
			// ��keyͷ�в����ڣ��½�һ��
			iMainKeyAddr = _pDataAllocator->allocateMainKeyHead(mIndex, vtData);
			if (iMainKeyAddr == 0)
			{
				return NFMemMultiHashMap::RT_NO_MEMORY;
			}
			// ����key���õ���keyͷ��
			MainKey mainKey(this, iMainKeyAddr);
			ret = mainKey.set(mk.c_str(), mk.length(), vtData);
			if (ret != NFMemMultiHashMap::RT_OK)
			{
				return ret;
			}
		}

		TC_PackIn pi;
		pi << uk;
		pi << v;
		uint32_t iAllocSize = (uint32_t)pi.length();

		// �ȷ���Blcok�ռ�, �������̭������
		uint32_t iAddr = _pDataAllocator->allocateMemBlock(iMainKeyAddr, index, bHead, iAllocSize, vtData);
		if (iAddr == 0)
		{
			return NFMemMultiHashMap::RT_NO_MEMORY;
		}

		it = HashMapLockIterator(this, iAddr, HashMapLockIterator::IT_BLOCK, HashMapLockIterator::IT_NEXT);
		bNewBlock = true;
	}

	ret = it->set(mk, uk, v, iVersion, vtData);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		// set����ʧ�ܣ����ܵ�����ǿռ䲻����
		if (bNewBlock)
		{
			// �������set�����ݣ���ʱҪע��ɾ��ǰ����䲢�ҽӵ�blockͷ
			// �������block����Զ�޷����ʣ�Ҳ����ɾ������Ϊblock����һ���յ�block
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

	if (eType != AUTO_DATA)
	{
		MainKey mainKey(this, block.getBlockHead()->_iMainKey);
		mainKey.SETFULLDATA(eType == FULL_DATA);
	}

	block.refreshSetList();

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::set(const std::string &mk, const std::string &uk, DATATYPE eType, bool bHead, std::vector<Value> &vtData)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int ret = NFMemMultiHashMap::RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, ret);
	bool bNewBlock = false;

	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}

	incHitCount();

	if (it == end())
	{
		// �����Ƿ��Ѿ���������key��
		uint32_t mIndex = mhashIndex(mk);        // ��key����
		uint32_t iMainKeyAddr = find(mk, mIndex, ret);
		if (ret != RT_OK && ret != RT_ONLY_KEY)
		{
			return ret;
		}
		if (iMainKeyAddr == 0)
		{
			// ��keyͷ�в����ڣ��½�һ��
			iMainKeyAddr = _pDataAllocator->allocateMainKeyHead(mIndex, vtData);
			if (iMainKeyAddr == 0)
			{
				return NFMemMultiHashMap::RT_NO_MEMORY;
			}
			// ����key���õ���keyͷ��
			MainKey mainKey(this, iMainKeyAddr);
			ret = mainKey.set(mk.c_str(), mk.length(), vtData);
			if (ret != NFMemMultiHashMap::RT_OK)
			{
				return ret;
			}
		}

		TC_PackIn pi;
		pi << uk;
		uint32_t iAllocSize = (uint32_t)pi.length();

		//�ȷ���ռ�, �������̭������
		uint32_t iAddr = _pDataAllocator->allocateMemBlock(iMainKeyAddr, index, bHead, iAllocSize, vtData);
		if (iAddr == 0)
		{
			return NFMemMultiHashMap::RT_NO_MEMORY;
		}

		it = HashMapLockIterator(this, iAddr, HashMapLockIterator::IT_BLOCK, HashMapLockIterator::IT_NEXT);
		bNewBlock = true;
	}

	ret = it->set(mk, uk, vtData);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		// set����ʧ�ܣ����ܵ�����ǿռ䲻����
		if (bNewBlock)
		{
			// �������set�����ݣ���ʱҪע��ɾ��ǰ����䲢�ҽӵ�blockͷ
			// �������block����Զ�޷����ʣ�Ҳ����ɾ������Ϊblock����һ���յ�block
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

	if (eType != AUTO_DATA)
	{
		MainKey mainKey(this, block.getBlockHead()->_iMainKey);
		mainKey.SETFULLDATA(eType == FULL_DATA);
	}

	block.refreshSetList();

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::set(const std::string &mk, std::vector<Value> &vtData)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int ret = NFMemMultiHashMap::RT_OK;

	// �����Ƿ��Ѿ����ڸ���key
	uint32_t mIndex = mhashIndex(mk);        // ��key����
	uint32_t iMainKeyAddr = find(mk, mIndex, ret);
	if (ret != RT_OK && ret != RT_ONLY_KEY)
	{
		return ret;
	}

	incHitCount();

	if (iMainKeyAddr == 0)
	{
		// ��keyͷ�в����ڣ��½�һ��
		iMainKeyAddr = _pDataAllocator->allocateMainKeyHead(mIndex, vtData);
		if (iMainKeyAddr == 0)
		{
			return NFMemMultiHashMap::RT_NO_MEMORY;
		}
	}

	MainKey mainKey(this, iMainKeyAddr);

	while (mainKey.getHeadPtr()->_iAddr != 0)
	{
		// ���滹�����ݣ���Ҫ������Щ����
		Block block(this, mainKey.getHeadPtr()->_iAddr);
		Value value;
		value._mkey = mk;
		int ret = block.getBlockData(value._data);
		if (ret == NFMemMultiHashMap::RT_OK)
		{
			vtData.push_back(value);
		}
		block.erase();
	}

	// ����key���õ���keyͷ��
	ret = mainKey.set(mk.c_str(), mk.length(), vtData);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		return ret;
	}
	mainKey.refreshGetList();

	return NFMemMultiHashMap::RT_OK;
}

int NFMemMultiHashMap::set(const std::vector<Value> &vtSet, DATATYPE eType, bool bHead, bool bForce, std::vector<Value> &vtErased)
{
	// ���ﲻ��Ҫ�ӻָ�����
	//TC_Multi_HashMap::FailureRecover recover(this);

	int ret = RT_OK;

	for (size_t i = 0; i < vtSet.size(); i++)
	{
		if (bForce || checkMainKey(vtSet[i]._mkey) == RT_NO_DATA)
		{
			// ǿ�Ƹ��»���key��û������
			if (vtSet[i]._data._key.empty())
			{
				// û������key��������key��only key����
				ret = set(vtSet[i]._mkey, vtErased);
			}
			else if (vtSet[i]._data._value.empty())
			{
				// û��value����������������only key����
				ret = set(vtSet[i]._mkey, vtSet[i]._data._key, eType, bHead, vtErased);
			}
			else
			{
				ret = set(vtSet[i]._mkey, vtSet[i]._data._key, vtSet[i]._data._value,
					vtSet[i]._data._iVersion, vtSet[i]._data._dirty, eType, bHead, vtErased);
			}
			if (ret != RT_OK)
			{
				return ret;
			}
		}
	}

	return RT_OK;
}

int NFMemMultiHashMap::del(const std::string &mk, const std::string &uk, Value &data)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	if (_pHead->_bReadOnly) return RT_READONLY;

	int    ret = RT_OK;
	uint32_t index = hashIndex(mk, uk);
	lock_iterator it = find(mk, uk, index, data, ret);
	if (ret != NFMemMultiHashMap::RT_OK && ret != NFMemMultiHashMap::RT_ONLY_KEY)
	{
		return ret;
	}

	if (it == end())
	{
		return NFMemMultiHashMap::RT_NO_DATA;
	}

	incHitCount();

	Block block(this, it->getAddr());
	block.erase();

	return ret;
}

int NFMemMultiHashMap::del(const std::string &mk, std::vector<Value> &data)
{
	if (_pHead->_bReadOnly) return RT_READONLY;

	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	uint32_t iIndex = mhashIndex(mk);
	if (itemMainKey(iIndex)->_iMainKeyAddr != 0)
	{
		incHitCount();

		MainKey mainKey(this, itemMainKey(iIndex)->_iMainKeyAddr);
		do
		{
			std::string sKey;
			mainKey.get(sKey);
			if (mk == sKey)
			{
				// �ҵ���keyͷ��
				return mainKey.erase(data);
			}
		} while (mainKey.next());
	}

	return NFMemMultiHashMap::RT_NO_DATA;
}

int NFMemMultiHashMap::erase(int ratio, std::vector<Value> &vtData, bool bCheckDirty)
{
	if (_pHead->_bReadOnly) return RT_READONLY;

	NFMemMultiHashMap::FailureRecover recover(this);

	if (ratio <= 0)   ratio = 1;
	if (ratio >= 100) ratio = 100;

	uint32_t iAddr = _pHead->_iGetTail;
	//������ͷ��
	if (iAddr == 0)
	{
		return RT_OK;
	}

	//ɾ����ָ��������
	if ((_pHead->_iUsedChunk * 100. / allBlockChunkCount()) < ratio)
	{
		return RT_OK;
	}

	// ɾ���������key��
	MainKey mainKey(this, iAddr);
	if (bCheckDirty)
	{
		// ��������ݣ������ݲ���̭
		// ֻҪ��key����һ�������ݾͲ���̭
		lock_iterator it(this, mainKey.getHeadPtr()->_iAddr, lock_iterator::IT_MKEY, lock_iterator::IT_NEXT);
		while (it != end())
		{
			if (it->isDirty())
			{
				if (_pHead->_iDirtyTail == 0)
				{
					_pHead->_iDirtyTail = mainKey.getHeadPtr()->_iAddr;
				}
				return RT_OK;
			}
			it++;
		}
	}

	// ��̭��key�µ���������
	mainKey.erase(vtData);

	return RT_ERASE_OK;
}

void NFMemMultiHashMap::sync()
{
	NFMemMultiHashMap::FailureRecover recover(this);

	_pHead->_iSyncTail = _pHead->_iDirtyTail;
}

int NFMemMultiHashMap::sync(time_t iNowTime, Value &data)
{
	NFMemMultiHashMap::FailureRecover recover(this);

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

	// ȡ����key
	MainKey mainKey(this, block.getBlockHead()->_iMainKey);
	mainKey.get(data._mkey);

	int ret = block.getBlockData(data._data);
	if (ret != NFMemMultiHashMap::RT_OK)
	{
		//û�л�ȡ�����ļ�¼
		if (_pHead->_iDirtyTail == iAddr)
		{
			_pHead->_iDirtyTail = block.getBlockHead()->_iSetPrev;
		}
		return ret;
	}

	//�������ҳ���_pHead->_iSyncTimeû�л�д, ��Ҫ��д
	if (_pHead->_iSyncTime + data._data._synct < iNowTime && block.isDirty())
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

void NFMemMultiHashMap::backup(bool bForceFromBegin)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	if (bForceFromBegin || _pHead->_iBackupTail == 0)
	{
		//�ƶ�����ָ�뵽Get��β��, ׼����ʼ��������
		_pHead->_iBackupTail = _pHead->_iGetTail;
	}
}

int NFMemMultiHashMap::backup(std::vector<Value> &vtData)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	uint32_t iAddr = _pHead->_iBackupTail;

	//������ͷ����, ����RT_OK
	if (iAddr == 0)
	{
		return RT_OK;
	}

	// ȡ����key
	MainKey mainKey(this, iAddr);
	std::string mk;
	mainKey.get(mk);

	// һ�α���������key
	int ret = RT_OK;
	lock_iterator it(this, mainKey.getHeadPtr()->_iAddr, lock_iterator::IT_MKEY, lock_iterator::IT_NEXT);
	while (it != end())
	{
		Block block(this, it->_iAddr);
		Value value;
		value._mkey = mk;
		ret = block.getBlockData(value._data);
		if (ret != RT_OK)
		{
			break;
		}
		vtData.push_back(value);

		it++;
	}

	//Ǩ��һ��
	_pHead->_iBackupTail = mainKey.getHeadPtr()->_iGetPrev;

	if (ret == RT_OK)
	{
		return RT_NEED_BACKUP;
	}

	return ret;
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::begin()
{
	NFMemMultiHashMap::FailureRecover recover(this);

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

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::rbegin()
{
	NFMemMultiHashMap::FailureRecover recover(this);

	for (size_t i = _hash.size(); i > 0; i--)
	{
		tagHashItem &hashItem = _hash[i - 1];
		if (hashItem._iBlockAddr != 0)
		{
			Block block(this, hashItem._iBlockAddr);
			return lock_iterator(this, block.getLastBlockHead(true), lock_iterator::IT_BLOCK, lock_iterator::IT_PREV);
		}
	}

	return end();
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::beginSetTime()
{
	NFMemMultiHashMap::FailureRecover recover(this);
	return lock_iterator(this, _pHead->_iSetHead, lock_iterator::IT_SET, lock_iterator::IT_NEXT);
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::rbeginSetTime()
{
	NFMemMultiHashMap::FailureRecover recover(this);
	return lock_iterator(this, _pHead->_iSetTail, lock_iterator::IT_SET, lock_iterator::IT_PREV);
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::beginGetTime()
{
	NFMemMultiHashMap::FailureRecover recover(this);
	if (_pHead->_iGetHead != 0)
	{
		MainKey mainKey(this, _pHead->_iGetHead);
		return lock_iterator(this, mainKey.getHeadPtr()->_iAddr, lock_iterator::IT_GET, lock_iterator::IT_NEXT);
	}
	else
	{
		return lock_iterator(this, 0, lock_iterator::IT_GET, lock_iterator::IT_NEXT);
	}
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::rbeginGetTime()
{
	NFMemMultiHashMap::FailureRecover recover(this);
	if (_pHead->_iGetTail != 0)
	{
		MainKey mainKey(this, _pHead->_iGetTail);
		// �ҵ���key���ϵ����һ������
		uint32_t iAddr = mainKey.getHeadPtr()->_iAddr;
		if (iAddr)
		{
			Block block(this, iAddr);
			while (block.getBlockHead()->_iMKBlockNext)
			{
				iAddr = block.getBlockHead()->_iMKBlockNext;
				block = Block(this, iAddr);
			}
		}
		return lock_iterator(this, iAddr, lock_iterator::IT_GET, lock_iterator::IT_PREV);
	}
	else
	{
		return lock_iterator(this, 0, lock_iterator::IT_GET, lock_iterator::IT_PREV);
	}
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::beginDirty()
{
	NFMemMultiHashMap::FailureRecover recover(this);
	return lock_iterator(this, _pHead->_iDirtyTail, lock_iterator::IT_SET, lock_iterator::IT_PREV);
}

NFMemMultiHashMap::hash_iterator NFMemMultiHashMap::hashBegin()
{
	NFMemMultiHashMap::FailureRecover recover(this);
	return hash_iterator(this, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string NFMemMultiHashMap::desc()
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
		s << "[MainKeyHashCount = " << _hashMainKey.size() << "]" << std::endl;
		s << "[HashRatio        = " << _pHead->_fHashRatio << "]" << std::endl;
		s << "[MainKeyRatio     = " << _pHead->_fMainKeyRatio << "]" << std::endl;
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
		s << "[HitRatio         = " << (_pHead->_iGetCount == 0 ? 0 : _pHead->_iHitCount * 100 / _pHead->_iGetCount) << "]" << std::endl;
		s << "[OnlyKeyCount     = " << _pHead->_iOnlyKeyCount << "]" << std::endl;
		s << "[MKOnlyKeyCount   = " << _pHead->_iMKOnlyKeyCount << "]" << std::endl;
		s << "[MaxMKBlockCount  = " << _pHead->_iMaxBlockCount << "]" << std::endl;
		s << "[ModifyStatus     = " << (int)_pstModifyHead->_cModifyStatus << "]" << std::endl;
		s << "[ModifyIndex      = " << _pstModifyHead->_iNowIndex << "]" << std::endl;
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

	analyseHashM(iMaxHash, iMinHash, fAvgHash);
	{
		s << "[MaxMainkeyHash   = " << iMaxHash << "]" << std::endl;
		s << "[MinMainKeyHash   = " << iMinHash << "]" << std::endl;
		s << "[AvgMainKeyHash   = " << fAvgHash << "]" << std::endl;
	}

	std::vector<NFMemChunk::tagChunkHead> vChunkHead = _pDataAllocator->getBlockDetail();

	s << "*************************************************************************" << std::endl;
	s << "[DiffBlockCount   = " << vChunkHead.size() << "]" << std::endl;
	for (size_t i = 0; i < vChunkHead.size(); i++)
	{
		s << "[BlockSize        = " << vChunkHead[i]._iBlockSize << "] ";
		s << "[BlockCount       = " << vChunkHead[i]._iBlockCount << "] ";
		s << "[BlockAvailable   = " << vChunkHead[i]._blockAvailable << "]" << std::endl;
	}

	return s.str();
}

size_t NFMemMultiHashMap::eraseExcept(uint32_t iNowAddr, std::vector<Value> &vtData)
{
	//���ܱ���̭
	if (!_pHead->_bAutoErase) return 0;

	size_t n = _pHead->_iEraseCount;
	if (n == 0) n = 10;
	size_t d = n;

	while (d != 0)
	{
		uint32_t iAddr;

		//�жϰ������ַ�ʽ��̭
		if (_pHead->_cEraseMode == NFMemMultiHashMap::ERASEBYSET)
		{
			// ��set����̭
			iAddr = _pHead->_iSetTail;
			if (iAddr == 0)
			{
				break;
			}
			// set����block�����������keyͷ�ĵ�ַ
			Block block(this, iAddr);
			iAddr = block.getBlockHead()->_iMainKey;
		}
		else
		{
			// ��get����̭
			iAddr = _pHead->_iGetTail;
			if (iAddr == 0)
			{
				break;
			}
		}

		// �����ǰ�get����̭���ǰ�set����̭������Ҫ��̭��key����������
		MainKey mainKey(this, iAddr);
		if (iNowAddr == iAddr)
		{
			// ��ǰ��key���������ڷ����block������
			iAddr = mainKey.getHeadPtr()->_iGetPrev;
		}
		if (iAddr == 0)
		{
			break;
		}

		uint32_t iBlockCount = mainKey.getHeadPtr()->_iBlockCount;
		if (d >= iBlockCount)
		{
			// ��̭��key���µ���������
			int ret = mainKey.erase(vtData);
			if (ret == RT_OK)
			{
				d -= iBlockCount;
			}
		}
		else
		{
			if (d == n)
			{
				// ���õ�Ҫɾ��������̫С��û��������������key��ֱ��ɾ��
				int ret = mainKey.erase(vtData);
				if (ret == RT_OK)
				{
					n = d + iBlockCount;    // ʹ������ȷ��������
				}
			}
			break;
		}
	}

	return n - d;
}

uint32_t NFMemMultiHashMap::hashIndex(const std::string &mk, const std::string &uk)
{
	// mk����key��uk�ǳ���key��ĸ�key�����߼�������Ϊ��������
	return hashIndex(mk + uk);
}

uint32_t NFMemMultiHashMap::hashIndex(const std::string& k)
{
	return _hashf(k) % _hash.size();
}

uint32_t NFMemMultiHashMap::mhashIndex(const std::string &mk)
{
	if (_mhashf)
	{
		return _mhashf(mk) % _hashMainKey.size();
	}
	else
	{
		// ���û�е���ָ����key hash��������ʹ������������hash����
		return _hashf(mk) % _hashMainKey.size();
	}
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::find(const std::string &mk, const std::string &uk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	uint32_t index = hashIndex(mk, uk);
	int ret = NFMemMultiHashMap::RT_OK;

	if (item(index)->_iBlockAddr == 0)
	{
		return end();
	}

	Block mb(this, item(index)->_iBlockAddr);
	while (true)
	{
		HashMapLockItem mcmdi(this, mb.getHead());
		if (mcmdi.equal(mk, uk, ret))
		{
			// �ҵ��ˣ�����������ͬ��keyҲ��ͬ
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

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::find(const std::string &mk, const std::string &uk, uint32_t index, Value &v, int &ret)
{
	ret = NFMemMultiHashMap::RT_OK;

	if (item(index)->_iBlockAddr == 0)
	{
		return end();
	}

	Block mb(this, item(index)->_iBlockAddr);
	while (true)
	{
		HashMapLockItem mcmdi(this, mb.getHead());
		if (mcmdi.equal(mk, uk, v, ret))
		{
			// �ҵ��ˣ�����������ͬ��keyҲ��ͬ
			return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
		}

		if (!mb.nextBlock())
		{
			return end();
		}
	}

	return end();
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::find(const std::string &mk, const std::string &uk, uint32_t index, int &ret)
{
	ret = NFMemMultiHashMap::RT_OK;

	if (item(index)->_iBlockAddr == 0)
	{
		return end();
	}

	Block mb(this, item(index)->_iBlockAddr);
	while (true)
	{
		HashMapLockItem mcmdi(this, mb.getHead());
		if (mcmdi.equal(mk, uk, ret))
		{
			// �ҵ��ˣ�����������ͬ��keyҲ��ͬ
			return lock_iterator(this, mb.getHead(), lock_iterator::IT_BLOCK, lock_iterator::IT_NEXT);
		}

		if (!mb.nextBlock())
		{
			return end();
		}
	}

	return end();
}

uint32_t NFMemMultiHashMap::find(const std::string &mk, uint32_t index, int &ret)
{
	ret = NFMemMultiHashMap::RT_OK;

	if (itemMainKey(index)->_iMainKeyAddr == 0)
	{
		return 0;
	}

	MainKey mainKey(this, itemMainKey(index)->_iMainKeyAddr);
	do
	{
		std::string s;
		ret = mainKey.get(s);
		if (ret != RT_OK)
		{
			return 0;
		}
		if (s == mk)
		{
			// �ҵ���
			if (mainKey.getHeadPtr()->_iAddr == 0)
			{
				ret = RT_ONLY_KEY;
			}
			return mainKey.getHead();
		}

	} while (mainKey.next());

	return 0;
}

size_t NFMemMultiHashMap::count(const std::string &mk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	uint32_t iIndex = mhashIndex(mk);
	if (itemMainKey(iIndex)->_iMainKeyAddr != 0)
	{
		MainKey mainKey(this, itemMainKey(iIndex)->_iMainKeyAddr);
		do
		{
			std::string sKey;
			mainKey.get(sKey);
			if (mk == sKey)
			{
				// �ҵ���Ӧ����key��
				incHitCount();
				return mainKey.getHeadPtr()->_iBlockCount;
			}
		} while (mainKey.next());
	}
	return 0;
}

NFMemMultiHashMap::lock_iterator NFMemMultiHashMap::find(const std::string &mk)
{
	NFMemMultiHashMap::FailureRecover recover(this);

	incGetCount();

	uint32_t iIndex = mhashIndex(mk);
	if (itemMainKey(iIndex)->_iMainKeyAddr != 0)
	{
		MainKey mainKey(this, itemMainKey(iIndex)->_iMainKeyAddr);
		do
		{
			std::string sKey;
			mainKey.get(sKey);
			if (mk == sKey)
			{
				// �ҵ���Ӧ����key��
				incHitCount();
				if (mainKey.getHeadPtr()->_iAddr != 0)
				{
					// ��key����������
					return lock_iterator(this, mainKey.getHeadPtr()->_iAddr, lock_iterator::IT_MKEY, lock_iterator::IT_NEXT);
				}
				break;
			}
		} while (mainKey.next());
	}
	return end();
}

void NFMemMultiHashMap::analyseHash(uint32_t &iMaxHash, uint32_t &iMinHash, float &fAvgHash)
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

void NFMemMultiHashMap::analyseHashM(uint32_t &iMaxHash, uint32_t &iMinHash, float &fAvgHash)
{
	iMaxHash = 0;
	iMinHash = (uint32_t)-1;

	fAvgHash = 0;

	uint32_t n = 0;
	for (uint32_t i = 0; i < _hashMainKey.size(); i++)
	{
		iMaxHash = max(_hashMainKey[i]._iListCount, iMaxHash);
		iMinHash = min(_hashMainKey[i]._iListCount, iMinHash);
		//ƽ��ֵֻͳ�Ʒ�0��
		if (_hashMainKey[i]._iListCount != 0)
		{
			n++;
			fAvgHash += _hashMainKey[i]._iListCount;
		}
	}

	if (n != 0)
	{
		fAvgHash = fAvgHash / n;
	}
}

void NFMemMultiHashMap::doRecover()
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
			//#if __WORDSIZE == 64
			else if (_pstModifyHead->_stModifyData[i]._cBytes == sizeof(uint32_t))
			{
				*(uint32_t*)((char*)_pHead + _pstModifyHead->_stModifyData[i]._iModifyAddr) = (uint32_t)_pstModifyHead->_stModifyData[i]._iModifyValue;
			}
			//#endif
			else if (_pstModifyHead->_stModifyData[i]._cBytes == sizeof(uint8_t))
			{
				*(uint8_t*)((char*)_pHead + _pstModifyHead->_stModifyData[i]._iModifyAddr) = (bool)_pstModifyHead->_stModifyData[i]._iModifyValue;
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

void NFMemMultiHashMap::doUpdate()
{
	//==2,�Ѿ��޸ĳɹ�, ���
	if (_pstModifyHead->_cModifyStatus == 1)
	{
		_pstModifyHead->_iNowIndex = 0;
		_pstModifyHead->_cModifyStatus = 0;
	}
}

size_t NFMemMultiHashMap::getMinPrimeNumber(size_t n)
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

size_t NFMemMultiHashMap::checkBadBlock(uint32_t iHash, bool bRepair)
{
	size_t iCount = 0;
	if (iHash >= _hash.size())
	{
		return 0;
	}

	NFMemMultiHashMap::FailureRecover recover(this);

check:
	uint32_t iAddr = item(iHash)->_iBlockAddr;
	Block block(this, iAddr);
	while (block.getHead() != 0)
	{
		BlockData blkData;
		int ret = block.getBlockData(blkData);
		if (ret != RT_OK && ret != RT_ONLY_KEY)
		{
			iCount++;
			if (bRepair)
			{
				// ɾ�����block
				block.erase();
				goto check;
			}
		}
		if (!block.nextBlock())
		{
			break;
		}
	}

	return iCount;
}