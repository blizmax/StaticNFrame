// -------------------------------------------------------------------------
//    @FileName         :    NFCDataValue.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#include "NFCDataValue.h"
#include "NFCData.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <string.h>
#include <memory>

static const int max_depth = 200;

NFCDataValue::~NFCDataValue()
{
}

bool NFCDataValue::GetBool() const
{
	return false;
}

int NFCDataValue::GetInt() const
{
	return 0;
}

int32_t NFCDataValue::GetInt32() const
{
	return 0;
}

uint32_t NFCDataValue::GetUInt32() const
{
	return 0;
}

int64_t NFCDataValue::GetInt64() const
{
	return 0;
}

uint64_t NFCDataValue::GetUInt64() const
{
	return 0;
}

float NFCDataValue::GetFloat() const
{
	return 0;
}

double NFCDataValue::GetDouble() const
{
	return 0;
}

const std::string& NFCDataValue::GetString() const
{
	return NFCDataStatics::empty_string;
}

void NFCDataValue::SetUnknown()
{
}

void NFCDataValue::SetBool(bool value)
{
}

void NFCDataValue::SetInt(int value)
{
}

void NFCDataValue::SetInt32(int32_t value)
{
}

void NFCDataValue::SetUInt32(uint32_t value)
{
}

void NFCDataValue::SetInt64(int64_t value)
{
}

void NFCDataValue::SetUInt64(uint64_t value)
{
}

void NFCDataValue::SetFloat(float value)
{
}

void NFCDataValue::SetDouble(double value)
{
}

void NFCDataValue::SetString(const std::string& value)
{
}

void NFCDataValue::SetMapStringObject(const NFCData::MapStringObject& value)
{
}

void NFCDataValue::SetMapStringObject(NFCData::MapStringObject&& value)
{
}

void NFCDataValue::SetMapIntObject(const NFCData::MapIntObject& value)
{
}

void NFCDataValue::SetMapIntObject(NFCData::MapIntObject&& value)
{
}

void NFCDataValue::SetArray(const NFCData::Array& value)
{
}

void NFCDataValue::SetArray(NFCData::Array&& value)
{
}

void NFCDataValue::SetList(const NFCData::List& value)
{
}

void NFCDataValue::SetList(NFCData::List&& value)
{
}

const NFCData::Array& NFCDataValue::GetArray() const
{
	return NFCDataStatics::empty_vector;
}

const NFCData::List& NFCDataValue::GetList() const
{
	return NFCDataStatics::empty_list;
}

const NFCData::MapStringObject& NFCDataValue::GetMapStringObject() const
{
	return NFCDataStatics::empty_map_string;
}

const NFCData::MapIntObject& NFCDataValue::GetMapIntObject() const
{
	return NFCDataStatics::empty_map_int;
}

NFCData::Array& NFCDataValue::GetArray()
{
	return NFCDataStatics::empty_vector;
}

NFCData::List& NFCDataValue::GetList()
{
	return NFCDataStatics::empty_list;
}

NFCData::MapStringObject& NFCDataValue::GetMapStringObject()
{
	return NFCDataStatics::empty_map_string;
}

NFCData::MapIntObject& NFCDataValue::GetMapIntObject()
{
	return NFCDataStatics::empty_map_int;
}

/* * * * * * * * * * * * * * * * * * * *
 * 使用继承实现效果
 */

template <NF_DATA_TYPE tag, typename T>
class NFBaseValue : public NFCDataValue
{
public:
	// Constructors
	explicit NFBaseValue(const T& value) : m_value(value)
	{
	}

	explicit NFBaseValue(T&& value) : m_value(move(value))
	{
	}

	// Get type tag
	virtual NF_DATA_TYPE Type() const override
	{
		return tag;
	}

	T m_value;
};

class NFCDataDouble : public NFBaseValue<NF_DATA_TYPE::DT_DOUBLE, double>
{
public:
	explicit NFCDataDouble(double value) : NFBaseValue(value)
	{
	}

	explicit NFCDataDouble(float value) : NFBaseValue(static_cast<double>(value))
	{
	}

	virtual int  GetInt() const override
	{
		return static_cast<int>(m_value);
	}

	virtual int32_t GetInt32() const override
	{
		return static_cast<int32_t>(m_value);
	}

	virtual uint32_t GetUInt32() const override
	{
		return static_cast<uint32_t>(m_value);
	}

	virtual int64_t GetInt64() const override
	{
		return static_cast<int64_t>(m_value);
	}

	virtual uint64_t GetUInt64() const override
	{
		return static_cast<uint64_t>(m_value);
	}

	virtual float GetFloat() const override
	{
		return static_cast<float>(m_value);
	}

	virtual double GetDouble() const override
	{
		return static_cast<double>(m_value);
	}

	//Set data
	virtual void SetInt(int value) override
	{
		m_value = static_cast<double>(value);
	}

	virtual void SetInt32(int32_t value) override
	{
		m_value = static_cast<double>(value);
	}

	virtual void SetUInt32(uint32_t value) override
	{
		m_value = static_cast<double>(value);
	}

	virtual void SetInt64(int64_t value) override
	{
		m_value = static_cast<double>(value);
	}

	virtual void SetUInt64(uint64_t value) override
	{
		m_value = static_cast<double>(value);
	}

	virtual void SetFloat(float value) override
	{
		m_value = static_cast<double>(value);
	}

	virtual void SetDouble(double value) override
	{
		m_value = static_cast<double>(value);
	}
};

class NFCDataInt final : public NFBaseValue<NF_DATA_TYPE::DT_INT, int64_t>
{
public:
	explicit NFCDataInt(int64_t value) : NFBaseValue(value)
	{
	}

	virtual int  GetInt() const override
	{
		return static_cast<int>(m_value);
	}

	virtual int32_t GetInt32() const override
	{
		return static_cast<int32_t>(m_value);
	}

	virtual uint32_t GetUInt32() const override
	{
		return static_cast<uint32_t>(m_value);
	}

	virtual int64_t GetInt64() const override
	{
		return static_cast<int64_t>(m_value);
	}

	virtual uint64_t GetUInt64() const override
	{
		return static_cast<uint64_t>(m_value);
	}

	virtual float GetFloat() const override
	{
		return static_cast<float>(m_value);
	}

	virtual double GetDouble() const override
	{
		return static_cast<double>(m_value);
	}

	//Set data
	virtual void SetInt(int value) override
	{
		m_value = static_cast<int64_t>(value);
	}

	virtual void SetInt32(int32_t value) override
	{
		m_value = static_cast<int64_t>(value);
	}

	virtual void SetUInt32(uint32_t value) override
	{
		m_value = static_cast<int64_t>(value);
	}

	virtual void SetInt64(int64_t value) override
	{
		m_value = static_cast<int64_t>(value);
	}

	virtual void SetUInt64(uint64_t value) override
	{
		m_value = static_cast<int64_t>(value);
	}

	virtual void SetFloat(float value) override
	{
		m_value = static_cast<int64_t>(value);
	}

	virtual void SetDouble(double value) override
	{
		m_value = static_cast<int64_t>(value);
	}
};

class NFCDataUInt64 : public NFBaseValue<NF_DATA_TYPE::DT_UINT64, uint64_t>
{
public:
	explicit NFCDataUInt64(uint64_t value) : NFBaseValue(value)
	{
	}

	virtual int  GetInt() const override
	{
		return static_cast<int>(m_value);
	}

	virtual int32_t GetInt32() const override
	{
		return static_cast<int32_t>(m_value);
	}

	virtual uint32_t GetUInt32() const override
	{
		return static_cast<uint32_t>(m_value);
	}

	virtual int64_t GetInt64() const override
	{
		return static_cast<int64_t>(m_value);
	}

	virtual uint64_t GetUInt64() const override
	{
		return static_cast<uint64_t>(m_value);
	}

	virtual float GetFloat() const override
	{
		return static_cast<float>(m_value);
	}

	virtual double GetDouble() const override
	{
		return static_cast<double>(m_value);
	}

	//Set data
	virtual void SetInt(int value) override
	{
		m_value = static_cast<uint64_t>(value);
	}

	virtual void SetInt32(int32_t value) override
	{
		m_value = static_cast<uint64_t>(value);
	}

	virtual void SetUInt32(uint32_t value) override
	{
		m_value = static_cast<uint64_t>(value);
	}

	virtual void SetInt64(int64_t value) override
	{
		m_value = static_cast<uint64_t>(value);
	}

	virtual void SetUInt64(uint64_t value) override
	{
		m_value = static_cast<uint64_t>(value);
	}

	virtual void SetFloat(float value) override
	{
		m_value = static_cast<uint64_t>(value);
	}

	virtual void SetDouble(double value) override
	{
		m_value = static_cast<uint64_t>(value);
	}
};

class NFCDataBoolean final : public NFBaseValue<NF_DATA_TYPE::DT_BOOLEAN, bool>
{
public:
	explicit NFCDataBoolean(bool value) : NFBaseValue(value)
	{
	}

	virtual bool GetBool() const override
	{
		return m_value;
	}

	virtual void SetBool(bool value) override
	{
		m_value = value;
	}
};

class NFCDataString final : public NFBaseValue<NF_DATA_TYPE::DT_STRING, string>
{
public:
	explicit NFCDataString(const std::string& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataString(std::string&& value) : NFBaseValue(move(value))
	{
	}

	virtual const std::string& GetString() const override
	{
		return m_value;
	}

	virtual void SetString(const std::string& value) override
	{
		m_value = value;
	}
};

class NFCDataArray final : public NFBaseValue<NF_DATA_TYPE::DT_ARRAY, NFCData::Array>
{
public:
	explicit NFCDataArray(const NFCData::Array& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataArray(NFCData::Array&& value) : NFBaseValue(std::move(value))
	{
	}

	virtual const NFCData::Array& GetArray() const override
	{
		return m_value;
	}

	virtual NFCData::Array& GetArray() override
	{
		return m_value;
	}

	virtual void SetArray(const NFCData::Array& value) override
	{
		m_value = value;
	}

	virtual void SetArray(NFCData::Array&& value) override
	{
		m_value = std::move(value);
	}
};

class NFCDataList final : public NFBaseValue<NF_DATA_TYPE::DT_LIST, NFCData::List>
{
public:
	explicit NFCDataList(const NFCData::List& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataList(NFCData::List&& value) : NFBaseValue(std::move(value))
	{
	}

	virtual const NFCData::List& GetList() const override
	{
		return m_value;
	}

	virtual NFCData::List& GetList() override
	{
		return m_value;
	}

	virtual void SetList(const NFCData::List& value) override
	{
		m_value = value;
	}

	virtual void SetList(NFCData::List&& value) override
	{
		m_value = std::move(value);
	}
};

class NFCDataMapStringObject final : public NFBaseValue<NF_DATA_TYPE::DT_MAPSTRING, NFCData::MapStringObject>
{
public:
	explicit NFCDataMapStringObject(const NFCData::MapStringObject& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataMapStringObject(NFCData::MapStringObject&& value) : NFBaseValue(move(value))
	{
	}

	virtual const NFCData::MapStringObject& GetMapStringObject() const override
	{
		return m_value;
	}

	virtual NFCData::MapStringObject& GetMapStringObject() override
	{
		return m_value;
	}

	virtual void SetMapStringObject(const NFCData::MapStringObject& value) override
	{
		m_value = value;
	}

	virtual void SetMapStringObject(NFCData::MapStringObject&& value) override
	{
		m_value = std::move(value);
	}
};

class NFCDataMapIntObject final : public NFBaseValue<NF_DATA_TYPE::DT_MAPINT, NFCData::MapIntObject>
{
public:
	explicit NFCDataMapIntObject(const NFCData::MapIntObject& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataMapIntObject(NFCData::MapIntObject&& value) : NFBaseValue(move(value))
	{
	}

	virtual const NFCData::MapIntObject& GetMapIntObject() const override
	{
		return m_value;
	}

	virtual NFCData::MapIntObject& GetMapIntObject() override
	{
		return m_value;
	}

	virtual void SetMapIntObject(const NFCData::MapIntObject& value) override
	{
		m_value = value;
	}

	virtual void SetMapIntObject(NFCData::MapIntObject&& value) override
	{
		m_value = std::move(value);
	}
};

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
	assert(type == DT_UINT64);
	mType = DT_UINT64;
	m_ptr = std::make_shared<NFCDataUInt64>(value);
}

NFCData::NFCData(int type, bool value)
{
	assert(type == DT_BOOLEAN);
	mType = DT_BOOLEAN;
	m_ptr = std::make_shared<NFCDataBoolean>(value);
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

NFCData::NFCData(int type, const MapStringObject& value)
{
	assert(type == DT_MAPSTRING);
	mType = DT_MAPSTRING;
	m_ptr = std::make_shared<NFCDataMapStringObject>(value);
}

NFCData::NFCData(int type, const MapStringObject&& value)
{
	assert(type == DT_MAPSTRING);
	mType = DT_MAPSTRING;
	m_ptr = std::make_shared<NFCDataMapStringObject>(value);
}

NFCData::NFCData(int type, const MapIntObject& value)
{
	assert(type == DT_MAPINT);
	mType = DT_MAPINT;
	m_ptr = std::make_shared<NFCDataMapIntObject>(value);
}

NFCData::NFCData(int type, MapIntObject&& value)
{
	assert(type == DT_MAPINT);
	mType = DT_MAPINT;
	m_ptr = std::make_shared<NFCDataMapIntObject>(value);
}

NFCData::NFCData(const NFCData& src)
{
	this->mType = src.GetType();
	switch(this->mType)
	{
	case DT_BOOLEAN:
		m_ptr = std::make_shared<NFCDataBoolean>(src.GetBool());
		break;
	case DT_INT:
		m_ptr = std::make_shared<NFCDataInt>(src.GetInt64());
		break;
	case DT_UINT64:
		m_ptr = std::make_shared<NFCDataUInt64>(src.GetUInt64());
		break;
	case DT_FLOAT:
		m_ptr = std::make_shared<NFCDataDouble>(src.GetDouble());
		break;
	case DT_DOUBLE:
		m_ptr = std::make_shared<NFCDataDouble>(src.GetDouble());
		break;
	case DT_STRING:
		m_ptr = std::make_shared<NFCDataString>(src.GetString());
		break;
	case DT_MAPSTRING:
		m_ptr = std::make_shared<NFCDataMapStringObject>(src.GetMapStringObject());
		break;
	case DT_MAPINT:
		m_ptr = std::make_shared<NFCDataMapIntObject>(src.GetMapIntObject());
		break;
	default:
		NF_ASSERT(0);
		break;
	}
}