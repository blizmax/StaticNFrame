// -------------------------------------------------------------------------
//    @FileName         :    NFObject.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFObjectData.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <limits>


static const int max_depth = 200;

/* Helper for representing null - just a do-nothing struct, plus comparison
 * operators so the helpers in JsonValue work. We can't use nullptr_t because
 * it may not be orderable.
 */
struct NullStruct
{
	bool operator==(NullStruct) const
	{
		return true;
	}

	bool operator<(NullStruct) const
	{
		return false;
	}
};

/* * * * * * * * * * * * * * * * * * * *
 * Value wrappers
 */

template <NFObjectData::NFObjectType tag, typename T>
class NFValue : public NFObjectValue
{
protected:
	// Constructors
	explicit NFValue(const T& value) : m_value(value)
	{
	}

	explicit NFValue(T&& value) : m_value(move(value))
	{
	}

	// Get type tag
	NFObjectData::NFObjectType Type() const override
	{
		return tag;
	}

	// Comparisons
	bool Equals(const NFObjectValue* other) const override
	{
		return m_value == static_cast<const NFValue<tag, T> *>(other)->m_value;
	}

	bool Less(const NFObjectValue* other) const override
	{
		return m_value < static_cast<const NFValue<tag, T> *>(other)->m_value;
	}

	const T m_value;
};

class NFObjectDouble final : public NFValue<NFObjectData::NUMBER, double>
{
protected:
	double DoubleValue() const override
	{
		return m_value;
	}

	float FloatValue() const override
	{
		return static_cast<float>(m_value);
	}

	int32_t Int32Value() const override
	{
		return static_cast<int32_t>(m_value);
	}

	uint32_t Uint32Value() const override
	{
		return static_cast<uint32_t>(m_value);
	}

	int64_t Int64Value() const override
	{
		return static_cast<int64_t>(m_value);
	}

	uint64_t Uint64Value() const override
	{
		return static_cast<uint64_t>(m_value);
	}

	bool Equals(const NFObjectValue* other) const override
	{
		return m_value == other->DoubleValue();
	}

	bool Less(const NFObjectValue* other) const override
	{
		return m_value < other->DoubleValue();
	}

public:
	explicit NFObjectDouble(double value) : NFValue(value)
	{
	}
};

class NFObjectInt final : public NFValue<NFObjectData::NUMBER, int64_t>
{
protected:
	double DoubleValue() const override
	{
		return static_cast<double>(m_value);
	}

	float FloatValue() const override
	{
		return static_cast<float>(m_value);
	}

	int32_t Int32Value() const override
	{
		return static_cast<int32_t>(m_value);
	}

	uint32_t Uint32Value() const override
	{
		return static_cast<uint32_t>(m_value);
	}

	int64_t Int64Value() const override
	{
		return static_cast<int64_t>(m_value);
	}

	uint64_t Uint64Value() const override
	{
		return static_cast<uint64_t>(m_value);
	}

	bool Equals(const NFObjectValue* other) const override
	{
		return m_value == other->Int64Value();
	}

	bool Less(const NFObjectValue* other) const override
	{
		return m_value < other->Int64Value();
	}

public:
	explicit NFObjectInt(int32_t value) : NFValue(static_cast<int64_t>(value))
	{
	}
	explicit NFObjectInt(uint32_t value) : NFValue(static_cast<int64_t>(value))
	{
	}
	explicit NFObjectInt(int64_t value) : NFValue(value)
	{
	}
};

class NFObjectUInt64 final : public NFValue<NFObjectData::NUMBER, uint64_t>
{
protected:
	double DoubleValue() const override
	{
		return static_cast<double>(m_value);
	}

	float FloatValue() const override
	{
		return static_cast<float>(m_value);
	}

	int32_t Int32Value() const override
	{
		return static_cast<int32_t>(m_value);
	}

	uint32_t Uint32Value() const override
	{
		return static_cast<uint32_t>(m_value);
	}

	int64_t Int64Value() const override
	{
		return static_cast<int64_t>(m_value);
	}

	uint64_t Uint64Value() const override
	{
		return static_cast<uint64_t>(m_value);
	}

	bool Equals(const NFObjectValue* other) const override
	{
		return m_value == other->Uint64Value();
	}

	bool Less(const NFObjectValue* other) const override
	{
		return m_value < other->Uint64Value();
	}

public:
	explicit NFObjectUInt64(uint64_t value) : NFValue(value)
	{
	}
};

class NFObjectBoolean final : public NFValue<NFObjectData::BOOL, bool>
{
protected:
	bool BoolValue() const override
	{
		return m_value;
	}

public:
	explicit NFObjectBoolean(bool value) : NFValue(value)
	{
	}
};

class NFObjectString final : public NFValue<NFObjectData::STRING, string>
{
protected:
	const string& StringValue() const override
	{
		return m_value;
	}

public:
	explicit NFObjectString(const std::string& value) : NFValue(value)
	{
	}

	explicit NFObjectString(std::string&& value) : NFValue(move(value))
	{
	}
};

class NFObjectArray final : public NFValue<NFObjectData::ARRAY, NFObjectData::Array>
{
protected:
	const NFObjectData::Array& ArrayItems() const override
	{
		return m_value;
	}

	const NFObjectData& operator[](size_t i) const override;
public:
	explicit NFObjectArray(const NFObjectData::Array& value) : NFValue(value)
	{
	}

	explicit NFObjectArray(NFObjectData::Array&& value) : NFValue(move(value))
	{
	}
};

class NFObjectList final : public NFValue<NFObjectData::LIST, NFObjectData::List>
{
protected:
	const NFObjectData::List& ListItems() const override
	{
		return m_value;
	}

public:
	explicit NFObjectList(const NFObjectData::List& value) : NFValue(value)
	{
	}

	explicit NFObjectList(NFObjectData::List&& value) : NFValue(move(value))
	{
	}
};

class NFObjectMapObject final : public NFValue<NFObjectData::MAPObject, NFObjectData::MapObject>
{
protected:
	const NFObjectData::MapObject& MapObjectItems() const override
	{
		return m_value;
	}

	const NFObjectData& operator[](const string& key) const override;
public:
	explicit NFObjectMapObject(const NFObjectData::MapObject& value) : NFValue(value)
	{
	}

	explicit NFObjectMapObject(NFObjectData::MapObject&& value) : NFValue(move(value))
	{
	}
};

class NFObjectNull final : public NFValue<NFObjectData::NUL, NullStruct>
{
public:
	NFObjectNull() : NFValue({})
	{
	}
};

/* * * * * * * * * * * * * * * * * * * *
 * Static globals - static-init-safe
 */
struct NFStatics
{
	const std::shared_ptr<NFObjectValue> null = make_shared<NFObjectNull>();
	const std::shared_ptr<NFObjectValue> t = make_shared<NFObjectBoolean>(true);
	const std::shared_ptr<NFObjectValue> f = make_shared<NFObjectBoolean>(false);
	const std::string empty_string;
	const NFObjectData::Array empty_vector;
	const NFObjectData::List empty_list;
	const NFObjectData::MapObject empty_map;

	NFStatics()
	{
	}
};

static const NFStatics& Statics()
{
	static const NFStatics s{};
	return s;
}

static const NFObjectData& StaticNull()
{
	// This has to be separate, not in Statics, because Json() accesses statics().null.
	static const NFObjectData object_null;
	return object_null;
}

/* * * * * * * * * * * * * * * * * * * *
 * Constructors
 */

NFObjectData::NFObjectData() noexcept : m_ptr(Statics().null)
{
}

NFObjectData::NFObjectData(std::nullptr_t) noexcept : m_ptr(Statics().null)
{
}

NFObjectData::NFObjectData(double value) : m_ptr(make_shared<NFObjectDouble>(value))
{
}

NFObjectData::NFObjectData(int32_t value) : m_ptr(make_shared<NFObjectInt>(value))
{
}

NFObjectData::NFObjectData(uint32_t value) : m_ptr(make_shared<NFObjectInt>(value))
{
}

NFObjectData::NFObjectData(int64_t value) : m_ptr(make_shared<NFObjectInt>(value))
{
}

NFObjectData::NFObjectData(uint64_t value) : m_ptr(make_shared<NFObjectUInt64>(value))
{
}

NFObjectData::NFObjectData(bool value) : m_ptr(value ? Statics().t : Statics().f)
{
}

NFObjectData::NFObjectData(const string& value) : m_ptr(make_shared<NFObjectString>(value))
{
}

NFObjectData::NFObjectData(string&& value) : m_ptr(make_shared<NFObjectString>(move(value)))
{
}

NFObjectData::NFObjectData(const char* value) : m_ptr(make_shared<NFObjectString>(value))
{
}

NFObjectData::NFObjectData(const NFObjectData::Array& values) : m_ptr(make_shared<NFObjectArray>(values))
{
}

NFObjectData::NFObjectData(NFObjectData::Array&& values) : m_ptr(make_shared<NFObjectArray>(move(values)))
{
}

NFObjectData::NFObjectData(const NFObjectData::MapObject& values) : m_ptr(make_shared<NFObjectMapObject>(values))
{
}

NFObjectData::NFObjectData(NFObjectData::MapObject&& values) : m_ptr(make_shared<NFObjectMapObject>(move(values)))
{
}

NFObjectData::NFObjectData(const NFObjectData::List& values) : m_ptr(make_shared<NFObjectList>(values))
{
}

NFObjectData::NFObjectData(NFObjectData::List&& values) : m_ptr(make_shared<NFObjectList>(move(values)))
{
}

/* * * * * * * * * * * * * * * * * * * *
 * Accessors
 */

NFObjectData::NFObjectType NFObjectData::Type() const
{
	return m_ptr->Type();
}

double NFObjectData::DoubleValue() const
{
	return m_ptr->DoubleValue();
}

float NFObjectData::FloatValue() const
{
	return m_ptr->FloatValue();
}

int32_t NFObjectData::Int32Value() const
{
	return m_ptr->Int32Value();
}

uint32_t NFObjectData::Uint32Value() const
{
	return m_ptr->Uint32Value();
}

int64_t NFObjectData::Int64Value() const
{
	return m_ptr->Int64Value();
}

uint64_t NFObjectData::Uint64Value() const
{
	return m_ptr->Uint64Value();
}

bool NFObjectData::BoolValue() const
{
	return m_ptr->BoolValue();
}

const std::string& NFObjectData::StringValue() const
{
	return m_ptr->StringValue();
}

const NFObjectData::Array& NFObjectData::ArrayItems() const
{
	return m_ptr->ArrayItems();
}

const NFObjectData::List& NFObjectData::ListItems() const
{
	return m_ptr->ListItems();
}

const NFObjectData::MapObject& NFObjectData::MapObjectItems() const
{
	return m_ptr->MapObjectItems();
}

const NFObjectData& NFObjectData::operator[](size_t i) const
{
	return (*m_ptr)[i];
}

const NFObjectData& NFObjectData::operator[](const string& key) const
{
	return (*m_ptr)[key];
}

double NFObjectValue::DoubleValue() const
{
	return 0;
}

float NFObjectValue::FloatValue() const
{
	return 0;
}

int32_t NFObjectValue::Int32Value() const
{
	return 0;
}

uint32_t NFObjectValue::Uint32Value() const
{
	return 0;
}

int64_t NFObjectValue::Int64Value() const
{
	return 0;
}

uint64_t NFObjectValue::Uint64Value() const
{
	return 0;
}

bool NFObjectValue::BoolValue() const
{
	return false;
}

const std::string& NFObjectValue::StringValue() const
{
	return Statics().empty_string;
}

const NFObjectData::Array& NFObjectValue::ArrayItems() const
{
	return Statics().empty_vector;
}

const NFObjectData::List& NFObjectValue::ListItems() const
{
	return Statics().empty_list;
}

const NFObjectData::MapObject& NFObjectValue::MapObjectItems() const
{
	return Statics().empty_map;
}

const NFObjectData& NFObjectValue::operator[](size_t) const
{
	return StaticNull();
}

const NFObjectData& NFObjectValue::operator[](const string&) const
{
	return StaticNull();
}

const NFObjectData& NFObjectMapObject::operator[](const string& key) const
{
	auto iter = m_value.find(key);
	return (iter == m_value.end()) ? StaticNull() : iter->second;
}

const NFObjectData& NFObjectArray::operator[](size_t i) const
{
	if (i >= m_value.size()) return StaticNull();
	else return m_value[i];
}

/* * * * * * * * * * * * * * * * * * * *
* Comparison
*/

bool NFObjectData::operator==(const NFObjectData& other) const
{
	if (m_ptr == other.m_ptr)
		return true;
	if (m_ptr->Type() != other.m_ptr->Type())
		return false;

	return m_ptr->Equals(other.m_ptr.get());
}

bool NFObjectData::operator<(const NFObjectData& other) const
{
	if (m_ptr == other.m_ptr)
		return false;
	if (m_ptr->Type() != other.m_ptr->Type())
		return m_ptr->Type() < other.m_ptr->Type();

	return m_ptr->Less(other.m_ptr.get());
}

