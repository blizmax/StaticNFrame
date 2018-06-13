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
float NFCDataStatics::empty_float = 0;
double NFCDataStatics::empty_double = 0;
std::string NFCDataStatics::empty_string;
NFCData::Array NFCDataStatics::empty_array;
NFCData::List NFCDataStatics::empty_list;
NFCData::MapStringData NFCDataStatics::empty_map_string;
NFCData::MapIntData NFCDataStatics::empty_map_int;

NFCData& NFCData::operator=(const NFCData& src)
{
	mType = src.mType;
	m_ptr = src.m_ptr;
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
	switch (type)
	{
	case DT_BOOLEAN:
		SetBool(NFCDataStatics::empty_boolean);
		break;
	case DT_INT:
		SetInt64(NFCDataStatics::empty_int);
		break;
	case DT_FLOAT:
		SetFloat(NFCDataStatics::empty_float);
		break;
	case DT_DOUBLE:
		SetDouble(NFCDataStatics::empty_double);
		break;
	case DT_STRING:
		SetString(NFCDataStatics::empty_string);
		break;
	case DT_ARRAY:
		SetArray(NFCDataStatics::empty_array);
		break;
	case DT_LIST:
		SetList(NFCDataStatics::empty_list);
		break;
	case DT_MAPSTRING:
		SetMapStringData(NFCDataStatics::empty_map_string);
		break;
	case DT_MAPINT:
		SetMapIntData(NFCDataStatics::empty_map_int);
		break;
	default:
		NF_ASSERT(0);
		break;
	}
}

bool NFCData::IsNumber() const
{
	return mType == DT_INT ||
		mType == DT_FLOAT || mType == DT_BOOLEAN;
}

bool NFCData::IsNullValue() const
{
	switch (GetType())
	{
	case DT_BOOLEAN:
		return GetBool() == NFCDataStatics::empty_boolean;
	case DT_INT:
		return GetInt64() == NFCDataStatics::empty_int;
	case DT_FLOAT:
		return GetFloat() == NFCDataStatics::empty_float;
	case DT_DOUBLE:
		return GetDouble() == NFCDataStatics::empty_double;
	case DT_STRING:
		return GetString().empty();
	case DT_ARRAY:
		return GetArray().empty();
	case DT_LIST:
		return GetList().empty();
	case DT_MAPSTRING:
		return GetMapStringObject().empty();
	case DT_MAPINT:
		return GetMapIntObject().empty();
	default:
		NF_ASSERT(0);
		break;
	}

	return false;
}

bool NFCData::GetBool() const
{
	NF_ASSERT_RET_VAL(mType == DT_BOOLEAN, NFCDataStatics::empty_boolean);
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
	NF_ASSERT_RET_VAL(IsNumber(), NFCDataStatics::empty_float);
	return m_ptr->GetFloat();
}

double NFCData::GetDouble() const
{
	NF_ASSERT_RET_VAL(IsNumber(), NFCDataStatics::empty_double);
	return m_ptr->GetDouble();
}

const std::string& NFCData::GetString() const
{
	NF_ASSERT_RET_VAL(mType == DT_STRING, NFCDataStatics::empty_string);
	return m_ptr->GetString();
}

void NFCData::SetUnknown()
{
	mType = DT_UNKNOWN;
	m_ptr = nullptr;
}

void NFCData::SetBool(bool value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_BOOLEAN);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_BOOLEAN;
		m_ptr = std::make_shared<NFCDataBoolean>(value);
		return;
	}

	mType = DT_BOOLEAN;
	if (m_ptr)
	{
		m_ptr->SetBool(value);
	}
}

void NFCData::SetInt(int value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_INT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = DT_INT;
	if (m_ptr)
	{
		m_ptr->SetInt(value);
	}
}

void NFCData::SetInt32(int32_t value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_INT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = DT_INT;
	if (m_ptr)
	{
		m_ptr->SetInt32(value);
	}
}

void NFCData::SetUInt32(uint32_t value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_INT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = DT_INT;
	if (m_ptr)
	{
		m_ptr->SetUInt32(value);
	}
}

void NFCData::SetInt64(int64_t value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_INT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = DT_INT;
	if (m_ptr)
	{
		m_ptr->SetInt64(value);
	}
}

void NFCData::SetUInt64(uint64_t value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_INT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_INT;
		m_ptr = std::make_shared<NFCDataInt>(value);
		return;
	}

	mType = DT_INT;
	if (m_ptr)
	{
		m_ptr->SetUInt64(value);
	}
}

void NFCData::SetFloat(float value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_FLOAT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_FLOAT;
		m_ptr = std::make_shared<NFCDataDouble>(value);
		return;
	}

	mType = DT_FLOAT;
	if (m_ptr)
	{
		m_ptr->SetFloat(value);
	}
}

void NFCData::SetDouble(double value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_DOUBLE);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_DOUBLE;
		m_ptr = std::make_shared<NFCDataDouble>(value);
		return;
	}

	mType = DT_DOUBLE;
	if (m_ptr)
	{
		m_ptr->SetDouble(value);
	}
}

void NFCData::SetString(const std::string& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_STRING);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_STRING;
		m_ptr = std::make_shared<NFCDataString>(value);
		return;
	}

	mType = DT_STRING;
	if (m_ptr)
	{
		m_ptr->SetString(value);
	};
}

const NFCData::Array& NFCData::GetArray() const
{
	NF_ASSERT_RET_VAL(mType == DT_ARRAY, NFCDataStatics::empty_array);

	return m_ptr->GetArray();
}

const NFCData::List& NFCData::GetList() const
{
	NF_ASSERT_RET_VAL(mType == DT_LIST, NFCDataStatics::empty_list);

	return m_ptr->GetList();
}

const NFCData::MapStringData& NFCData::GetMapStringObject() const
{
	NF_ASSERT_RET_VAL(mType == DT_MAPSTRING, NFCDataStatics::empty_map_string);

	return m_ptr->GetMapStringData();
}

const NFCData::MapIntData& NFCData::GetMapIntObject() const
{
	NF_ASSERT_RET_VAL(mType == DT_MAPINT, NFCDataStatics::empty_map_int);

	return m_ptr->GetMapIntData();
}

NFCData::Array* NFCData::MutableArray()
{
	NF_ASSERT_RET_VAL(mType == DT_ARRAY, nullptr);

	return m_ptr->MutableArray();
}

NFCData::List* NFCData::MutableList()
{
	NF_ASSERT_RET_VAL(mType == DT_LIST, nullptr);

	return m_ptr->MutableList();
}

NFCData::MapStringData* NFCData::MutableMapStringData()
{
	NF_ASSERT_RET_VAL(mType == DT_MAPSTRING, nullptr);

	return m_ptr->MutableMapStringData();
}

NFCData::MapIntData* NFCData::MutableMapIntData()
{
	NF_ASSERT_RET_VAL(mType == DT_MAPINT, nullptr);

	return m_ptr->MutableMapIntData();
}

void NFCData::AddArrayItem(const NFCData& data)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_ARRAY);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_ARRAY;
		m_ptr = std::make_shared<NFCDataArray>(NFCData::Array());
		return;
	}

	mType = DT_ARRAY;
	if (m_ptr)
	{
		m_ptr->AddArrayItem(data);
	}
}

void NFCData::AddListItem(const NFCData& data)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_LIST);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_LIST;
		m_ptr = std::make_shared<NFCDataList>(NFCData::List());
		return;
	}

	mType = DT_LIST;
	if (m_ptr)
	{
		m_ptr->AddListItem(data);
	}
}

void NFCData::AddMapStringItem(const std::string& key, const NFCData& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_MAPSTRING);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_MAPSTRING;
		m_ptr = std::make_shared<NFCDataMapStringData>(NFCData::MapStringData());
		return;
	}

	mType = DT_MAPSTRING;
	if (m_ptr)
	{
		m_ptr->AddMapStringItem(key, value);
	}
}

void NFCData::AddMapIntItem(uint64_t key, const NFCData& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_MAPINT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_MAPINT;
		m_ptr = std::make_shared<NFCDataMapIntData>(NFCData::MapIntData());
		return;
	}

	mType = DT_MAPINT;
	if (m_ptr)
	{
		m_ptr->AddMapIntItem(key, value);
	}
}

void NFCData::SetMapStringData(const MapStringData& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_MAPSTRING);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_MAPSTRING;
		m_ptr = std::make_shared<NFCDataMapStringData>(value);
		return;
	}

	mType = DT_MAPSTRING;
	if (m_ptr)
	{
		m_ptr->SetMapStringData(value);
	}
}

void NFCData::SetMapStringData(MapStringData&& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_MAPSTRING);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_MAPSTRING;
		m_ptr = std::make_shared<NFCDataMapStringData>(std::move(value));
		return;
	}

	mType = DT_MAPSTRING;
	if (m_ptr)
	{
		m_ptr->SetMapStringData(std::move(value));
	}
}

void NFCData::SetMapIntData(const MapIntData& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_MAPINT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_MAPINT;
		m_ptr = std::make_shared<NFCDataMapIntData>(value);
		return;
	}

	mType = DT_MAPINT;
	if (m_ptr)
	{
		m_ptr->SetMapIntData(value);
	}
}

void NFCData::SetMapIntData(MapIntData&& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_MAPINT);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_MAPINT;
		m_ptr = std::make_shared<NFCDataMapIntData>(std::move(value));
		return;
	}

	mType = DT_MAPINT;
	if (m_ptr)
	{
		m_ptr->SetMapIntData(std::move(value));
	}
}

void NFCData::SetArray(const Array& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_ARRAY);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_ARRAY;
		m_ptr = std::make_shared<NFCDataArray>(value);
		return;
	}

	mType = DT_ARRAY;
	if (m_ptr)
	{
		m_ptr->SetArray(value);
	}
}

void NFCData::SetArray(Array&& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_ARRAY);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_ARRAY;
		m_ptr = std::make_shared<NFCDataArray>(std::move(value));
		return;
	}

	mType = DT_ARRAY;
	if (m_ptr)
	{
		m_ptr->SetArray(std::move(value));
	}
}

void NFCData::SetList(const List& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_LIST);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_LIST;
		m_ptr = std::make_shared<NFCDataList>(value);
		return;
	}

	mType = DT_LIST;
	if (m_ptr)
	{
		m_ptr->SetList(value);
	}
}

void NFCData::SetList(List&& value)
{
	NF_ASSERT(mType == DT_UNKNOWN || mType == DT_LIST);

	if (mType == DT_UNKNOWN)
	{
		assert(m_ptr == nullptr);
		mType = DT_LIST;
		m_ptr = std::make_shared<NFCDataList>(std::move(value));
		return;
	}

	mType = DT_LIST;
	if (m_ptr)
	{
		m_ptr->SetList(std::move(value));
	}
}

NFCData::NFCData(int type, double value)
{
	assert(type == DT_DOUBLE);
	mType = DT_DOUBLE;
	m_ptr = std::make_shared<NFCDataDouble>(value);
}

NFCData::NFCData(int type, int32_t value)
{
	assert(type == DT_INT);
	mType = DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(value);
}

NFCData::NFCData(int type, uint32_t value)
{
	assert(type == DT_INT);
	mType = DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(value);
}

NFCData::NFCData(int type, int64_t value)
{
	assert(type == DT_INT);
	mType = DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(value);
}

NFCData::NFCData(int type, uint64_t value)
{
	assert(type == DT_INT);
	mType = DT_INT;
	m_ptr = std::make_shared<NFCDataInt>(static_cast<int64_t>(value));
}

NFCData::NFCData(int type, bool value)
{
	assert(type == DT_BOOLEAN);
	mType = DT_BOOLEAN;
	m_ptr = std::make_shared<NFCDataBoolean>(value);
}

NFCData::NFCData(int type, const char* value)
{
	assert(type == DT_STRING);
	mType = DT_STRING;
	m_ptr = std::make_shared<NFCDataString>(value);
}

NFCData::NFCData(int type, const std::string& value)
{
	assert(type == DT_STRING);
	mType = DT_STRING;
	m_ptr = std::make_shared<NFCDataString>(value);
}

NFCData::NFCData(int type, std::string&& value)
{
	assert(type == DT_STRING);
	mType = DT_STRING;
	m_ptr = std::make_shared<NFCDataString>(value);
}

NFCData::NFCData(int type, const Array& value)
{
	assert(type == DT_ARRAY);
	mType = DT_ARRAY;
	m_ptr = std::make_shared<NFCDataArray>(value);
}

NFCData::NFCData(int type, Array&& value)
{
	assert(type == DT_ARRAY);
	mType = DT_ARRAY;
	m_ptr = std::make_shared<NFCDataArray>(value);
}

NFCData::NFCData(int type, const List& value)
{
	assert(type == DT_LIST);
	mType = DT_LIST;
	m_ptr = std::make_shared<NFCDataList>(value);
}

NFCData::NFCData(int type, List&& value)
{
	assert(type == DT_LIST);
	mType = DT_LIST;
	m_ptr = std::make_shared<NFCDataList>(value);
}

NFCData::NFCData(int type, const MapStringData& value)
{
	assert(type == DT_MAPSTRING);
	mType = DT_MAPSTRING;
	m_ptr = std::make_shared<NFCDataMapStringData>(value);
}

NFCData::NFCData(int type, const MapStringData&& value)
{
	assert(type == DT_MAPSTRING);
	mType = DT_MAPSTRING;
	m_ptr = std::make_shared<NFCDataMapStringData>(value);
}

NFCData::NFCData(int type, const MapIntData& value)
{
	assert(type == DT_MAPINT);
	mType = DT_MAPINT;
	m_ptr = std::make_shared<NFCDataMapIntData>(value);
}

NFCData::NFCData(int type, MapIntData&& value)
{
	assert(type == DT_MAPINT);
	mType = DT_MAPINT;
	m_ptr = std::make_shared<NFCDataMapIntData>(value);
}

void NFCData::DeepCopy(const NFCData& src)
{
	this->mType = src.GetType();
	switch (this->mType)
	{
	case DT_BOOLEAN:
		m_ptr = make_shared<NFCDataBoolean>(src.GetBool());
		break;
	case DT_INT:
		m_ptr = make_shared<NFCDataInt>(src.GetInt64());
		break;
	case DT_FLOAT:
		m_ptr = make_shared<NFCDataDouble>(src.GetDouble());
		break;
	case DT_DOUBLE:
		m_ptr = make_shared<NFCDataDouble>(src.GetDouble());
		break;
	case DT_STRING:
		m_ptr = make_shared<NFCDataString>(src.GetString());
		break;
	case DT_ARRAY:
		m_ptr = make_shared<NFCDataArray>(src.GetArray());
		break;
	case DT_LIST:
		m_ptr = make_shared<NFCDataList>(src.GetList());
		break;
	case DT_MAPSTRING:
		m_ptr = make_shared<NFCDataMapStringData>(src.GetMapStringObject());
		break;
	case DT_MAPINT:
		m_ptr = make_shared<NFCDataMapIntData>(src.GetMapIntObject());
		break;
	default:
		NF_ASSERT(0);
		break;
	}
}

NFCData::NFCData(const NFCData& src)
{
	mType = src.mType;
	m_ptr = src.m_ptr;
}

