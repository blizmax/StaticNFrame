// -------------------------------------------------------------------------
//    @FileName         :    NFCDataValue.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCData.h"

class NFCDataValue
{
public:
	virtual ~NFCDataValue();

	virtual NF_DATA_TYPE Type() const = 0;
	//Get data
	virtual bool GetBool() const;
	virtual int GetInt() const;
	virtual int32_t GetInt32() const;
	virtual uint32_t GetUInt32() const;
	virtual int64_t GetInt64() const;
	virtual uint64_t GetUInt64() const;
	virtual float GetFloat() const;
	virtual double GetDouble() const;
	virtual const std::string& GetString() const;

	//Set data
	virtual void SetUnknown();
	virtual void SetBool(bool value);
	virtual void SetInt(int value);
	virtual void SetInt32(int32_t value);
	virtual void SetUInt32(uint32_t value);
	virtual void SetInt64(int64_t value);
	virtual void SetUInt64(uint64_t value);
	virtual void SetFloat(float value);
	virtual void SetDouble(double value);
	virtual void SetString(const std::string& value);

	virtual const NFCData::Array& GetArray() const;
	virtual const NFCData::List& GetList() const;
	virtual const NFCData::MapStringData& GetMapStringData() const;
	virtual const NFCData::MapIntData& GetMapIntData() const;

	virtual NFCData::Array* MutableArray();
	virtual NFCData::List* MutableList();
	virtual NFCData::MapStringData* MutableMapStringData();
	virtual NFCData::MapIntData* MutableMapIntData();

	virtual void AddArrayItem(const NFCData& data);
	virtual void AddListItem(const NFCData& data);
	virtual void AddMapStringItem(const std::string& key, const NFCData& value);
	virtual void AddMapIntItem(uint64_t key, const NFCData& value);

	virtual void SetMapStringData(const NFCData::MapStringData& value);
	virtual void SetMapStringData(NFCData::MapStringData&& value);

	virtual void SetMapIntData(const NFCData::MapIntData& value);
	virtual void SetMapIntData(NFCData::MapIntData&& value);


	virtual void SetArray(const NFCData::Array& value);
	virtual void SetArray(NFCData::Array&& value);

	virtual void SetList(const NFCData::List& value);
	virtual void SetList(NFCData::List&& value);
};

/* * * * * * * * * * * * * * * * * * * *
* ʹ�ü̳�ʵ��Ч��
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

	virtual int GetInt() const override
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

	virtual int GetInt() const override
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

	virtual NFCData::Array* MutableArray() override
	{
		return &m_value;
	}

	virtual void SetArray(const NFCData::Array& value) override
	{
		m_value = value;
	}

	virtual void SetArray(NFCData::Array&& value) override
	{
		m_value = std::move(value);
	}

	virtual void AddArrayItem(const NFCData& data) override
	{
		m_value.push_back(data);
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

	virtual NFCData::List* MutableList() override
	{
		return &m_value;
	}

	virtual void SetList(const NFCData::List& value) override
	{
		m_value = value;
	}

	virtual void SetList(NFCData::List&& value) override
	{
		m_value = std::move(value);
	}

	virtual void AddListItem(const NFCData& data) override
	{
		m_value.push_back(data);
	}
};

class NFCDataMapStringData final : public NFBaseValue<NF_DATA_TYPE::DT_MAPSTRING, NFCData::MapStringData>
{
public:
	explicit NFCDataMapStringData(const NFCData::MapStringData& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataMapStringData(NFCData::MapStringData&& value) : NFBaseValue(move(value))
	{
	}

	virtual const NFCData::MapStringData& GetMapStringData() const override
	{
		return m_value;
	}

	virtual NFCData::MapStringData* MutableMapStringData() override
	{
		return &m_value;
	}

	virtual void SetMapStringData(const NFCData::MapStringData& value) override
	{
		m_value = value;
	}

	virtual void SetMapStringData(NFCData::MapStringData&& value) override
	{
		m_value = std::move(value);
	}

	virtual void AddMapStringItem(const std::string& key, const NFCData& value) override
	{
		m_value[key] = value;
	}
};

class NFCDataMapIntData final : public NFBaseValue<NF_DATA_TYPE::DT_MAPINT, NFCData::MapIntData>
{
public:
	explicit NFCDataMapIntData(const NFCData::MapIntData& value) : NFBaseValue(value)
	{
	}

	explicit NFCDataMapIntData(NFCData::MapIntData&& value) : NFBaseValue(std::move(value))
	{
	}

	virtual const NFCData::MapIntData& GetMapIntData() const override
	{
		return m_value;
	}

	virtual NFCData::MapIntData* MutableMapIntData() override
	{
		return &m_value;
	}

	virtual void SetMapIntData(const NFCData::MapIntData& value) override
	{
		m_value = value;
	}

	virtual void SetMapIntData(NFCData::MapIntData&& value) override
	{
		m_value = std::move(value);
	}

	virtual void AddMapIntItem(uint64_t key, const NFCData& value) override
	{
		m_value[key] = value;
	}
};

