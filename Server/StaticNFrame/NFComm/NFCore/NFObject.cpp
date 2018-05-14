// -------------------------------------------------------------------------
//    @FileName         :    NFObject.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFObject.h"
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
struct NullStruct {
    bool operator==(NullStruct) const { return true; }
    bool operator<(NullStruct) const { return false; }
};


/* * * * * * * * * * * * * * * * * * * *
 * Value wrappers
 */

template <NFObject::NFObjectType tag, typename T>
class NFValue : public NFObjectValue {
protected:
    // Constructors
    explicit NFValue(const T &value) : m_value(value) {}
    explicit NFValue(T &&value)      : m_value(move(value)) {}

    // Get type tag
    NFObject::NFObjectType Type() const override {
        return tag;
    }

    // Comparisons
    bool Equals(const NFObjectValue * other) const override {
        return m_value == static_cast<const NFValue<tag, T> *>(other)->m_value;
    }

	bool Less(const NFObjectValue * other) const override {
		return m_value < static_cast<const NFValue<tag, T> *>(other)->m_value;
	}

    const T m_value;
};

class NFObjectDouble final : public NFValue<NFObject::NUMBER, double> 
{
protected:
    double   DoubleValue() const override { return m_value; }
    float    FloatValue() const override  { return (float)m_value; }
    int32_t  Int32Value() const override  { return static_cast<int32_t>(m_value); }
    uint32_t Uint32Value() const override { return static_cast<uint32_t>(m_value); }
    int64_t  Int64Value() const override  { return static_cast<int64_t>(m_value); }
	uint64_t Uint64Value() const override { return static_cast<uint64_t>(m_value); }
    bool Equals(const NFObjectValue * other) const override { return m_value == other->DoubleValue(); }
	bool Less(const NFObjectValue * other)   const override { return m_value <  other->DoubleValue(); }
public:
    explicit NFObjectDouble(double value) : NFValue(value) {}
};

class NFObjectInt final : public NFValue<NFObject::NUMBER, int64_t>  
{
protected:
    double   DoubleValue() const override { return (double)m_value; }
    float    FloatValue() const override  { return (float)m_value; }
    int32_t  Int32Value() const override  { return (int32_t)(m_value); }
    uint32_t Uint32Value() const override { return (uint32_t)(m_value); }
    int64_t  Int64Value() const override  { return (int64_t)(m_value); }
	uint64_t Uint64Value() const override { return (uint64_t)(m_value); }
    bool Equals(const NFObjectValue * other) const override { return m_value == other->Int64Value(); }
	bool Less(const NFObjectValue * other)   const override { return m_value <  other->Int64Value(); }
public:
    explicit NFObjectInt(int value) : NFValue(value) {}
};

class NFObjectBoolean final : public NFValue<NFObject::BOOL, bool> 
{
protected:
    bool BoolValue() const override { return m_value; }
public:
    explicit NFObjectBoolean(bool value) : NFValue(value) {}
};

class NFObjectString final : public NFValue<NFObject::STRING, string> 
{
protected:
    const string &StringValue() const override { return m_value; }
public:
    explicit NFObjectString(const std::string &value) : NFValue(value) {}
    explicit NFObjectString(std::string &&value)      : NFValue(move(value)) {}
};

class NFObjectArray final : public NFValue<NFObject::ARRAY, NFObject::Array> 
{
protected:
    const NFObject::Array &ArrayItems() const override { return m_value; }
    const NFObject & operator[](size_t i) const override;
public:
    explicit NFObjectArray(const NFObject::Array &value) : NFValue(value) {}
    explicit NFObjectArray(NFObject::Array &&value)      : NFValue(move(value)) {}
};

class NFObjectList final : public NFValue<NFObject::LIST, NFObject::List> 
{
protected:
    const NFObject::List &ListItems() const override { return m_value; }
public:
    explicit NFObjectList(const NFObject::List &value) : NFValue(value) {}
    explicit NFObjectList(NFObject::List &&value)      : NFValue(move(value)) {}
};

class NFObjectMapObject final : public NFValue<NFObject::MAPObject, NFObject::MapObject> 
{
protected:
    const NFObject::MapObject &MapObjectItems() const override { return m_value; }
    const NFObject & operator[](const string &key) const override;
public:
    explicit NFObjectMapObject(const  NFObject::MapObject &value) : NFValue(value) {}
    explicit NFObjectMapObject( NFObject::MapObject &&value)      : NFValue(move(value)) {}
};

class NFObjectNull final : public NFValue<NFObject::NUL, NullStruct> 
{
public:
    NFObjectNull() : NFValue({}) {}
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
    const NFObject::Array empty_vector;
    const NFObject::List empty_list;
    const NFObject::MapObject empty_map;
    NFStatics() {}
};

static const NFStatics & Statics() {
    static const NFStatics s {};
    return s;
}

static const NFObject& StaticNull() {
    // This has to be separate, not in Statics, because Json() accesses statics().null.
    static const NFObject object_null;
    return object_null;
}

/* * * * * * * * * * * * * * * * * * * *
 * Constructors
 */

NFObject::NFObject() noexcept                  : m_ptr(Statics().null) {}
NFObject::NFObject(std::nullptr_t) noexcept    : m_ptr(Statics().null) {}
NFObject::NFObject(double value)               : m_ptr(make_shared<NFObjectDouble>(value)) {}
NFObject::NFObject(int value)                  : m_ptr(make_shared<NFObjectInt>(value)) {}
NFObject::NFObject(bool value)                 : m_ptr(value ? Statics().t : Statics().f) {}
NFObject::NFObject(const string &value)        : m_ptr(make_shared<NFObjectString>(value)) {}
NFObject::NFObject(string &&value)             : m_ptr(make_shared<NFObjectString>(move(value))) {}
NFObject::NFObject(const char * value)         : m_ptr(make_shared<NFObjectString>(value)) {}
NFObject::NFObject(const NFObject::Array &values)  : m_ptr(make_shared<NFObjectArray>(values)) {}
NFObject::NFObject(NFObject::Array &&values)       : m_ptr(make_shared<NFObjectArray>(move(values))) {}
NFObject::NFObject(const NFObject::MapObject &values) : m_ptr(make_shared<NFObjectMapObject>(values)) {}
NFObject::NFObject(NFObject::MapObject &&values)      : m_ptr(make_shared<NFObjectMapObject>(move(values))) {}
NFObject::NFObject(const NFObject::List& values) : m_ptr(make_shared<NFObjectList>(values)) {} 
NFObject::NFObject(NFObject::List &&values)		  : m_ptr(make_shared<NFObjectList>(move(values))) {}

/* * * * * * * * * * * * * * * * * * * *
 * Accessors
 */

NFObject::NFObjectType NFObject::Type()             const { return m_ptr->Type();         }
double NFObject::DoubleValue()                       const { return m_ptr->DoubleValue(); }
float  NFObject::FloatValue()                       const { return m_ptr->FloatValue(); }
int32_t NFObject::Int32Value()                             const { return m_ptr->Int32Value();    }
uint32_t NFObject::Uint32Value()                             const { return m_ptr->Uint32Value();    }
int64_t NFObject::Int64Value()                             const { return m_ptr->Int64Value();    }
uint64_t NFObject::Uint64Value()                             const { return m_ptr->Uint64Value();    }
bool NFObject::BoolValue()                           const { return m_ptr->BoolValue();   }
const std::string & NFObject::StringValue()               const { return m_ptr->StringValue(); }
const NFObject::Array & NFObject::ArrayItems()          const { return m_ptr->ArrayItems();  }
const NFObject::List & NFObject::ListItems()          const { return m_ptr->ListItems();  }
const NFObject::MapObject & NFObject::MapObjectItems()    const { return m_ptr->MapObjectItems(); }
const NFObject & NFObject::operator[] (size_t i)          const { return (*m_ptr)[i];           }
const NFObject & NFObject::operator[] (const string &key) const { return (*m_ptr)[key];         }

double                    NFObjectValue::DoubleValue()              const { return 0; }
float                     NFObjectValue::FloatValue()              const { return 0; }
int32_t                       NFObjectValue::Int32Value()                 const { return 0; }
uint32_t                       NFObjectValue::Uint32Value()                 const { return 0; }
int64_t                      NFObjectValue::Int64Value()                 const { return 0; }
uint64_t                      NFObjectValue::Uint64Value()                 const { return 0; }
bool                      NFObjectValue::BoolValue()                const { return false; }
const std::string &            NFObjectValue::StringValue()              const { return Statics().empty_string; }
const NFObject::Array&      NFObjectValue::ArrayItems()               const { return Statics().empty_vector; }
const NFObject::List &      NFObjectValue::ListItems()               const { return Statics().empty_list; }
const NFObject::MapObject & NFObjectValue::MapObjectItems()              const { return Statics().empty_map; }
const NFObject &              NFObjectValue::operator[] (size_t)         const { return StaticNull(); }
const NFObject &              NFObjectValue::operator[] (const string &) const { return StaticNull(); }

const NFObject & NFObjectMapObject::operator[] (const string &key) const {
    auto iter = m_value.find(key);
    return (iter == m_value.end()) ? StaticNull() : iter->second;
}
const NFObject & NFObjectArray::operator[] (size_t i) const {
    if (i >= m_value.size()) return StaticNull();
    else return m_value[i];
}

/* * * * * * * * * * * * * * * * * * * *
* Comparison
*/

bool NFObject::operator== (const NFObject &other) const {
	if (m_ptr == other.m_ptr)
		return true;
	if (m_ptr->Type() != other.m_ptr->Type())
		return false;

	return m_ptr->Equals(other.m_ptr.get());
}

bool NFObject::operator< (const NFObject &other) const {
	if (m_ptr == other.m_ptr)
		return false;
	if (m_ptr->Type() != other.m_ptr->Type())
		return m_ptr->Type() < other.m_ptr->Type();

	return m_ptr->Less(other.m_ptr.get());
}