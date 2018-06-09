// -------------------------------------------------------------------------
//    @FileName         :    NFObject.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>

#include "NFPlatform.h"

enum NFObjectParse
{
	STANDARD,
	COMMENTS,
};

class NFObjectValue;

/**
 *@brief  NFrame的数据类
 */
class _NFExport NFObjectData final
{
public:
	/**
	*@brief 数据类型
	*/
	enum NFObjectType
	{
		NUL,
		NUMBER,
		BOOL,
		STRING,
		ARRAY,
		LIST,
		MAPObject,
	};

	// Array and object typedefs
	typedef std::vector<NFObjectData> Array;
	typedef std::list<NFObjectData> List;
	typedef std::map<std::string, NFObjectData> MapObject;

	/**
	* @brief 构造函数.
	*/
	NFObjectData() noexcept; // NUL
	NFObjectData(std::nullptr_t) noexcept; // NUL
	NFObjectData(double value); // NUMBER
	NFObjectData(int32_t value); // NUMBER
	NFObjectData(uint32_t value); // NUMBER
	NFObjectData(int64_t value); // NUMBER
	NFObjectData(uint64_t value); // NUMBER
	NFObjectData(bool value); // BOOL
	NFObjectData(const std::string& value); // STRING
	NFObjectData(std::string&& value); // STRING
	NFObjectData(const char* value); // STRING
	NFObjectData(const Array& jvalues); // ARRAY
	NFObjectData(Array&& values); // ARRAY
	NFObjectData(const List& kvalues); // LIST
	NFObjectData(List&& values); // LIST
	NFObjectData(const MapObject& values); // MAPOBJECT
	NFObjectData(MapObject&& values); // MAPOBJECT

	// This prevents NFObject(some_pointer) from accidentally producing a bool. Use
	// NFObject(bool(some_pointer)) if that behavior is desired.
	NFObjectData(void*) = delete;

	// Accessors
	NFObjectType Type() const;

	bool IsNull() const
	{
		return Type() == NUL;
	}

	bool IsNumber() const
	{
		return Type() == NUMBER;
	}

	bool IsBool() const
	{
		return Type() == BOOL;
	}

	bool IsString() const
	{
		return Type() == STRING;
	}

	bool IsArray() const
	{
		return Type() == ARRAY;
	}

	bool IsList() const
	{
		return Type() == LIST;
	}

	bool IsMapObject() const
	{
		return Type() == MAPObject;
	}

	// Return the enclosed value if this is a number, 0 otherwise. Note that json11 does not
	// distinguish between integer and non-integer numbers - number_value() and int_value()
	// can both be applied to a NUMBER-typed object.
	double DoubleValue() const;
	float FloatValue() const;
	int32_t Int32Value() const;
	uint32_t Uint32Value() const;
	int64_t Int64Value() const;
	uint64_t Uint64Value() const;

	// Return the enclosed value if this is a boolean, false otherwise.
	bool BoolValue() const;
	// Return the enclosed string if this is a string, "" otherwise.
	const std::string& StringValue() const;
	// Return the enclosed std::vector if this is an array, or an empty vector otherwise.
	const Array& ArrayItems() const;
	const List& ListItems() const;
	// Return the enclosed std::map if this is an object, or an empty map otherwise.
	const MapObject& MapObjectItems() const;

	// Return a reference to arr[i] if this is an array, Json() otherwise.
	const NFObjectData& operator[](size_t i) const;
	// Return a reference to obj[key] if this is an object, Json() otherwise.
	const NFObjectData& operator[](const std::string& key) const;

	bool operator==(const NFObjectData& rhs) const;
	bool operator<(const NFObjectData& rhs) const;

	bool operator!=(const NFObjectData& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator<=(const NFObjectData& rhs) const
	{
		return !(rhs < *this);
	}

	bool operator>(const NFObjectData& rhs) const
	{
		return (rhs < *this);
	}

	bool operator>=(const NFObjectData& rhs) const
	{
		return !(*this < rhs);
	}

private:
	/**
	* 数据指针
	*/
	std::shared_ptr<NFObjectValue> m_ptr;
};

class NFObjectValue
{
protected:
	friend class NFObjectData;
	friend class NFObjectDouble;
	friend class NFObjectInt;
	friend class NFObjectUInt64;
	friend class NFObjectList;
	virtual NFObjectData::NFObjectType Type() const = 0;
	virtual bool Equals(const NFObjectValue* other) const = 0;
	virtual bool Less(const NFObjectValue* other) const = 0;
	virtual double DoubleValue() const;
	virtual float FloatValue() const;
	virtual int32_t Int32Value() const;
	virtual uint32_t Uint32Value() const;
	virtual int64_t Int64Value() const;
	virtual uint64_t Uint64Value() const;
	virtual bool BoolValue() const;
	virtual const std::string& StringValue() const;
	virtual const NFObjectData::Array& ArrayItems() const;
	virtual const NFObjectData& operator[](size_t i) const;
	virtual const NFObjectData::List& ListItems() const;
	virtual const NFObjectData::MapObject& MapObjectItems() const;
	virtual const NFObjectData& operator[](const std::string& key) const;

	virtual ~NFObjectValue()
	{
	}
};

