// -------------------------------------------------------------------------
//    @FileName         :    NFCData.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#include "NFCData.h"
#include "NFCDataValue.h"

class NFCDataDouble;

bool NFCDataStatics::empty_boolean = false;
int64_t NFCDataStatics::empty_int = 0;
double NFCDataStatics::empty_double = 0;
std::string NFCDataStatics::empty_string;
NFCData::Array NFCDataStatics::empty_array;
NFCData::List NFCDataStatics::empty_list;
NFCData::MapStringData NFCDataStatics::empty_map_string;
NFCData::MapIntData NFCDataStatics::empty_map_int;

NFCData& NFCData::operator=(const NFCData& src)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == src.mType);
	DeepCopy(src);
	return *this;
}

void NFCData::Swap(NFCData& src)
{
	std::swap(mType, src.mType);
	std::swap(m_ptr, src.m_ptr);
}

int NFCData::GetType() const
{
	return mType;
}

void NFCData::SetDefaultValue(int type)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == type);
	switch (type)
	{
	case NF_DT_BOOLEAN:
		SetBool(NFCDataStatics::empty_boolean);
		break;
	case NF_DT_INT:
		SetInt64(NFCDataStatics::empty_int);
		break;
	case NF_DT_DOUBLE:
		SetDouble(NFCDataStatics::empty_double);
		break;
	case NF_DT_STRING:
		SetString(NFCDataStatics::empty_string);
		break;
	case NF_DT_ARRAY:
		SetArray(NFCDataStatics::empty_array);
		break;
	case NF_DT_LIST:
		SetList(NFCDataStatics::empty_list);
		break;
	case NF_DT_MAPSTRING:
		SetMapStringData(NFCDataStatics::empty_map_string);
		break;
	case NF_DT_MAPINT:
		SetMapIntData(NFCDataStatics::empty_map_int);
		break;
	default:
		NF_ASSERT(0);
		break;
	}
}

bool NFCData::IsNumber() const
{
	return mType == NF_DT_INT || mType == NF_DT_BOOLEAN || mType == NF_DT_DOUBLE;
}

bool NFCData::IsNullValue() const
{
	switch (GetType())
	{
	case NF_DT_BOOLEAN:
		return GetBool() == NFCDataStatics::empty_boolean;
	case NF_DT_INT:
		return GetInt64() == NFCDataStatics::empty_int;
	case NF_DT_DOUBLE:
		return GetDouble() == NFCDataStatics::empty_double;
	case NF_DT_STRING:
		return GetString().empty();
	case NF_DT_ARRAY:
		return GetArray().empty();
	case NF_DT_LIST:
		return GetList().empty();
	case NF_DT_MAPSTRING:
		return GetMapStringData().empty();
	case NF_DT_MAPINT:
		return GetMapIntData().empty();
	default:
		NF_ASSERT(0);
		break;
	}

	return false;
}

bool NFCData::GetBool() const
{
	NF_ASSERT_RET_VAL(mType == NF_DT_BOOLEAN, NFCDataStatics::empty_boolean);
	return m_ptr->GetBool();
}

int NFCData::GetInt() const
{
	NF_ASSERT_RET_VAL(IsNumber(), static_cast<int>(NFCDataStatics::empty_int));
	return m_ptr->GetInt();
}

int32_t NFCData::GetInt32() const
{
	NF_ASSERT_RET_VAL(IsNumber(), static_cast<int>(NFCDataStatics::empty_int));
	return m_ptr->GetInt32();
}

uint32_t NFCData::GetUInt32() const
{
	NF_ASSERT_RET_VAL(IsNumber(), static_cast<int>(NFCDataStatics::empty_int));
	return m_ptr->GetUInt32();
}

int64_t NFCData::GetInt64() const
{
	NF_ASSERT_RET_VAL(IsNumber(), NFCDataStatics::empty_int);
	return m_ptr->GetInt64();
}

uint64_t NFCData::GetUInt64() const
{
	NF_ASSERT_RET_VAL(IsNumber(), NFCDataStatics::empty_int);
	return m_ptr->GetUInt64();
}

float NFCData::GetFloat() const
{
	NF_ASSERT_RET_VAL(IsNumber(), NFCDataStatics::empty_double);
	return m_ptr->GetFloat();
}

double NFCData::GetDouble() const
{
	NF_ASSERT_RET_VAL(IsNumber(), NFCDataStatics::empty_double);
	return m_ptr->GetDouble();
}

const std::string& NFCData::GetString() const
{
	NF_ASSERT_RET_VAL(mType == NF_DT_STRING, NFCDataStatics::empty_string);
	return m_ptr->GetString();
}

void NFCData::SetUnknown()
{
	mType = NF_DT_UNKNOWN;
	m_ptr = nullptr;
}

void NFCData::SetBool(bool value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_BOOLEAN);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_BOOLEAN;
		m_ptr = std::make_shared<NFCDataBoolean>(value);
		return;
	}

	mType = NF_DT_BOOLEAN;
	if (m_ptr)
	{
		m_ptr->SetBool(value);
	}
}

void NFCData::SetInt(int value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_INT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = NF_DT_INT;
	if (m_ptr)
	{
		m_ptr->SetInt(value);
	}
}

void NFCData::SetInt32(int32_t value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_INT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = NF_DT_INT;
	if (m_ptr)
	{
		m_ptr->SetInt32(value);
	}
}

void NFCData::SetUInt32(uint32_t value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_INT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = NF_DT_INT;
	if (m_ptr)
	{
		m_ptr->SetUInt32(value);
	}
}

void NFCData::SetInt64(int64_t value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_INT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = NF_DT_INT;
	if (m_ptr)
	{
		m_ptr->SetInt64(value);
	}
}

void NFCData::SetUInt64(uint64_t value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_INT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = NF_DT_INT;
	if (m_ptr)
	{
		m_ptr->SetUInt64(value);
	}
}

void NFCData::SetFloat(float value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_DOUBLE);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_DOUBLE;
		m_ptr = std::make_shared<NFCDataDouble>(value);
		return;
	}

	mType = NF_DT_DOUBLE;
	if (m_ptr)
	{
		m_ptr->SetFloat(value);
	}
}

void NFCData::SetDouble(double value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_DOUBLE);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_DOUBLE;
		m_ptr = std::make_shared<NFCDataDouble>(value);
		return;
	}

	mType = NF_DT_DOUBLE;
	if (m_ptr)
	{
		m_ptr->SetDouble(value);
	}
}

void NFCData::SetString(const std::string& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_STRING);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_STRING;
		m_ptr = std::make_shared<NFCDataString>(value);
		return;
	}

	mType = NF_DT_STRING;
	if (m_ptr)
	{
		m_ptr->SetString(value);
	};
}

const NFCData::Array& NFCData::GetArray() const
{
	NF_ASSERT_RET_VAL(mType == NF_DT_ARRAY, NFCDataStatics::empty_array);

	return m_ptr->GetArray();
}

const NFCData::List& NFCData::GetList() const
{
	NF_ASSERT_RET_VAL(mType == NF_DT_LIST, NFCDataStatics::empty_list);

	return m_ptr->GetList();
}

const NFCData::MapStringData& NFCData::GetMapStringData() const
{
	NF_ASSERT_RET_VAL(mType == NF_DT_MAPSTRING, NFCDataStatics::empty_map_string);

	return m_ptr->GetMapStringData();
}

const NFCData::MapIntData& NFCData::GetMapIntData() const
{
	NF_ASSERT_RET_VAL(mType == NF_DT_MAPINT, NFCDataStatics::empty_map_int);

	return m_ptr->GetMapIntData();
}

NFCData::Array* NFCData::MutableArray()
{
	NF_ASSERT_RET_VAL(mType == NF_DT_ARRAY, nullptr);

	return m_ptr->MutableArray();
}

NFCData::List* NFCData::MutableList()
{
	NF_ASSERT_RET_VAL(mType == NF_DT_LIST, nullptr);

	return m_ptr->MutableList();
}

NFCData::MapStringData* NFCData::MutableMapStringData()
{
	NF_ASSERT_RET_VAL(mType == NF_DT_MAPSTRING, nullptr);

	return m_ptr->MutableMapStringData();
}

NFCData::MapIntData* NFCData::MutableMapIntData()
{
	NF_ASSERT_RET_VAL(mType == NF_DT_MAPINT, nullptr);

	return m_ptr->MutableMapIntData();
}

void NFCData::AddArrayItem(const NFCData& data)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_ARRAY);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_ARRAY;
		m_ptr = std::make_shared<NFCDataArray>(NFCData::Array());
		return;
	}

	mType = NF_DT_ARRAY;
	if (m_ptr)
	{
		m_ptr->AddArrayItem(data);
	}
}

void NFCData::AddListItem(const NFCData& data)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_LIST);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_LIST;
		m_ptr = std::make_shared<NFCDataList>(NFCData::List());
		return;
	}

	mType = NF_DT_LIST;
	if (m_ptr)
	{
		m_ptr->AddListItem(data);
	}
}

void NFCData::AddMapStringItem(const std::string& key, const NFCData& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_MAPSTRING);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_MAPSTRING;
		m_ptr = std::make_shared<NFCDataMapStringData>(NFCData::MapStringData());
		return;
	}

	mType = NF_DT_MAPSTRING;
	if (m_ptr)
	{
		m_ptr->AddMapStringItem(key, value);
	}
}

void NFCData::AddMapIntItem(uint64_t key, const NFCData& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_MAPINT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_MAPINT;
		m_ptr = std::make_shared<NFCDataMapIntData>(NFCData::MapIntData());
		return;
	}

	mType = NF_DT_MAPINT;
	if (m_ptr)
	{
		m_ptr->AddMapIntItem(key, value);
	}
}

void NFCData::SetMapStringData(const MapStringData& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_MAPSTRING);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_MAPSTRING;
		m_ptr = std::make_shared<NFCDataMapStringData>(value);
		return;
	}

	mType = NF_DT_MAPSTRING;
	if (m_ptr)
	{
		m_ptr->SetMapStringData(value);
	}
}

void NFCData::SetMapStringData(MapStringData&& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_MAPSTRING);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_MAPSTRING;
		m_ptr = std::make_shared<NFCDataMapStringData>(std::move(value));
		return;
	}

	mType = NF_DT_MAPSTRING;
	if (m_ptr)
	{
		m_ptr->SetMapStringData(std::move(value));
	}
}

void NFCData::SetMapIntData(const MapIntData& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_MAPINT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_MAPINT;
		m_ptr = std::make_shared<NFCDataMapIntData>(value);
		return;
	}

	mType = NF_DT_MAPINT;
	if (m_ptr)
	{
		m_ptr->SetMapIntData(value);
	}
}

void NFCData::SetMapIntData(MapIntData&& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_MAPINT);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_MAPINT;
		m_ptr = std::make_shared<NFCDataMapIntData>(std::move(value));
		return;
	}

	mType = NF_DT_MAPINT;
	if (m_ptr)
	{
		m_ptr->SetMapIntData(std::move(value));
	}
}

void NFCData::SetArray(const Array& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_ARRAY);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_ARRAY;
		m_ptr = std::make_shared<NFCDataArray>(value);
		return;
	}

	mType = NF_DT_ARRAY;
	if (m_ptr)
	{
		m_ptr->SetArray(value);
	}
}

void NFCData::SetArray(Array&& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_ARRAY);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_ARRAY;
		m_ptr = std::make_shared<NFCDataArray>(std::move(value));
		return;
	}

	mType = NF_DT_ARRAY;
	if (m_ptr)
	{
		m_ptr->SetArray(std::move(value));
	}
}

void NFCData::SetList(const List& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_LIST);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_LIST;
		m_ptr = std::make_shared<NFCDataList>(value);
		return;
	}

	mType = NF_DT_LIST;
	if (m_ptr)
	{
		m_ptr->SetList(value);
	}
}

void NFCData::SetList(List&& value)
{
	NF_ASSERT(mType == NF_DT_UNKNOWN || mType == NF_DT_LIST);

	if (mType == NF_DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = NF_DT_LIST;
		m_ptr = std::make_shared<NFCDataList>(std::move(value));
		return;
	}

	mType = NF_DT_LIST;
	if (m_ptr)
	{
		m_ptr->SetList(std::move(value));
	}
}

NFCData::NFCData(int type, double value)
{
	assert(type == NF_DT_DOUBLE);
	mType = NF_DT_DOUBLE;
	m_ptr = std::make_shared<NFCDataDouble>(value);
}

NFCData::NFCData(int type, int32_t value)
{
	assert(type == NF_DT_INT);
	mType = NF_DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(value);
}

NFCData::NFCData(int type, uint32_t value)
{
	assert(type == NF_DT_INT);
	mType = NF_DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(value);
}

NFCData::NFCData(int type, int64_t value)
{
	assert(type == NF_DT_INT);
	mType = NF_DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(value);
}

NFCData::NFCData(int type, uint64_t value)
{
	assert(type == NF_DT_INT);
	mType = NF_DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(static_cast<int64_t>(value));
}

NFCData::NFCData(int type, bool value)
{
	assert(type == NF_DT_BOOLEAN);
	mType = NF_DT_BOOLEAN;
	m_ptr = std::make_shared<NFCDataBoolean>(value);
}

NFCData::NFCData(int type, const char* value)
{
	assert(type == NF_DT_STRING);
	mType = NF_DT_STRING;
	if (value != nullptr)
		m_ptr = std::make_shared<NFCDataString>(value);
	else
		m_ptr = std::make_shared<NFCDataString>("");
}

NFCData::NFCData(int type, const std::string& value)
{
	assert(type == NF_DT_STRING);
	mType = NF_DT_STRING;
	m_ptr = std::make_shared<NFCDataString>(value);
}

NFCData::NFCData(int type, std::string&& value)
{
	assert(type == NF_DT_STRING);
	mType = NF_DT_STRING;
	m_ptr = std::make_shared<NFCDataString>(value);
}

NFCData::NFCData(int type, const Array& value)
{
	assert(type == NF_DT_ARRAY);
	mType = NF_DT_ARRAY;
	m_ptr = std::make_shared<NFCDataArray>(value);
}

NFCData::NFCData(int type, Array&& value)
{
	assert(type == NF_DT_ARRAY);
	mType = NF_DT_ARRAY;
	m_ptr = std::make_shared<NFCDataArray>(value);
}

NFCData::NFCData(int type, const List& value)
{
	assert(type == NF_DT_LIST);
	mType = NF_DT_LIST;
	m_ptr = std::make_shared<NFCDataList>(value);
}

NFCData::NFCData(int type, List&& value)
{
	assert(type == NF_DT_LIST);
	mType = NF_DT_LIST;
	m_ptr = std::make_shared<NFCDataList>(value);
}

NFCData::NFCData(int type, const MapStringData& value)
{
	assert(type == NF_DT_MAPSTRING);
	mType = NF_DT_MAPSTRING;
	m_ptr = std::make_shared<NFCDataMapStringData>(value);
}

NFCData::NFCData(int type, const MapStringData&& value)
{
	assert(type == NF_DT_MAPSTRING);
	mType = NF_DT_MAPSTRING;
	m_ptr = std::make_shared<NFCDataMapStringData>(value);
}

NFCData::NFCData(int type, const MapIntData& value)
{
	assert(type == NF_DT_MAPINT);
	mType = NF_DT_MAPINT;
	m_ptr = std::make_shared<NFCDataMapIntData>(value);
}

NFCData::NFCData(int type, MapIntData&& value)
{
	assert(type == NF_DT_MAPINT);
	mType = NF_DT_MAPINT;
	m_ptr = std::make_shared<NFCDataMapIntData>(value);
}

void NFCData::DeepCopy(const NFCData& src)
{
	this->mType = src.GetType();
	switch (this->mType)
	{
	case NF_DT_BOOLEAN:
		m_ptr = make_shared<NFCDataBoolean>(src.GetBool());
		break;
	case NF_DT_INT:
		m_ptr = make_shared<NFCDataInt>(src.GetInt64());
		break;
	case NF_DT_DOUBLE:
		m_ptr = make_shared<NFCDataDouble>(src.GetDouble());
		break;
	case NF_DT_STRING:
		m_ptr = make_shared<NFCDataString>(src.GetString());
		break;
	case NF_DT_ARRAY:
		m_ptr = make_shared<NFCDataArray>(NFCData::Array());
		{
			const NFCData::Array& array = src.GetArray();
			for (size_t i = 0; i < array.size(); ++i)
			{
				const NFCData& tmp = array[i];
				NFCData data;
				data.DeepCopy(tmp);
				m_ptr->AddArrayItem(data);
			}
		}
		break;
	case NF_DT_LIST:
		m_ptr = make_shared<NFCDataList>(NFCData::List());
		{
			const NFCData::List& list = src.GetList();
			for (auto it = list.begin(); it != list.end(); ++it)
			{
				const NFCData& tmp = *it;
				NFCData data;
				data.DeepCopy(tmp);
				m_ptr->AddListItem(data);
			}
		}
		break;
	case NF_DT_MAPSTRING:
		m_ptr = make_shared<NFCDataMapStringData>(NFCData::MapStringData());
		{
			const NFCData::MapStringData& mapStringData = src.GetMapStringData();
			for (auto it = mapStringData.begin(); it != mapStringData.end(); ++it)
			{
				const std::string tmpKey = it->first;
				const NFCData& tmp = it->second;
				NFCData data;
				data.DeepCopy(tmp);
				m_ptr->AddMapStringItem(tmpKey, data);
			}
		}
		break;
	case NF_DT_MAPINT:
		m_ptr = make_shared<NFCDataMapIntData>(NFCData::MapIntData());
		{
			const NFCData::MapIntData& mapIntData = src.GetMapIntData();
			for (auto it = mapIntData.begin(); it != mapIntData.end(); ++it)
			{
				uint64_t tmpKey = it->first;
				const NFCData& tmp = it->second;
				NFCData data;
				data.DeepCopy(tmp);
				m_ptr->AddMapIntItem(tmpKey, data);
			}
		}
		break;
	default:
		NF_ASSERT(0);
		break;
	}
}

NFCData::NFCData(const NFCData& src)
{
	mType = NF_DT_UNKNOWN;
	DeepCopy(src);
}

