// -------------------------------------------------------------------------
//    @FileName         :    NFCData.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

class NFCDataValue;

enum NF_DATA_TYPE
{
	DT_UNKNOWN, //unknown type
	DT_BOOLEAN, //bool
	DT_INT, //int64_t
	DT_DOUBLE, //doubl
	DT_STRING, //string(char*)
	DT_ARRAY, //std::vector<NFIData>
	DT_LIST, //std::list<NFIData>
	DT_MAPSTRING, //std::map<std::string,NFIData>
	DT_MAPINT, //std::map<int,NFIData>
	DT_MAX, //max
};

class _NFExport NFCData
{
public:
	typedef std::vector<NFCData> Array;
	typedef std::list<NFCData> List;
	typedef std::unordered_map<std::string, NFCData> MapStringData;
	typedef std::unordered_map<uint64_t, NFCData> MapIntData;

	NFCData()
	{
		mType = DT_UNKNOWN;
	}

	virtual ~NFCData()
	{
	}

	/**
	* @brief 构造函数.
	*/
	NFCData(int type, double value); // NUMBER
	NFCData(int type, int32_t value); // NUMBER
	NFCData(int type, uint32_t value); // NUMBER
	NFCData(int type, int64_t value); // NUMBER
	NFCData(int type, uint64_t value); // NUMBER
	NFCData(int type, bool value); // BOOL
	NFCData(int type, const char* value); // STRING
	NFCData(int type, const std::string& value); // STRING
	NFCData(int type, std::string&& value); // STRING
	NFCData(int type, const Array& jvalues); // ARRAY
	NFCData(int type, Array&& values); // ARRAY
	NFCData(int type, const List& kvalues); // LIST
	NFCData(int type, List&& values); // LIST
	NFCData(int type, const MapStringData& values); // MAPOBJECT
	NFCData(int type, const MapStringData&& values); // MAPOBJECT
	NFCData(int type, const MapIntData& values); // MAPOBJECT
	NFCData(int type, MapIntData&& values); // MAPOBJECT
	NFCData(const NFCData& src);
	NFCData& operator=(const NFCData& src);
	void Swap(NFCData& src);
	void DeepCopy(const NFCData& src);

	virtual int GetType() const;
	virtual void SetDefaultValue(int type);
	virtual bool IsNullValue() const;
	virtual bool IsNumber() const;

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

	virtual const Array& GetArray() const;
	virtual const List& GetList() const;
	virtual const MapStringData& GetMapStringData() const;
	virtual const MapIntData& GetMapIntData() const;

	virtual Array* MutableArray();
	virtual List* MutableList();
	virtual MapStringData* MutableMapStringData();
	virtual MapIntData* MutableMapIntData();

	virtual void AddArrayItem(const NFCData& data);
	virtual void AddListItem(const NFCData& data);
	virtual void AddMapStringItem(const std::string& key, const NFCData& value);
	virtual void AddMapIntItem(uint64_t key, const NFCData& value);

	virtual void SetMapStringData(const MapStringData& value);
	virtual void SetMapStringData(MapStringData&& value);

	virtual void SetMapIntData(const MapIntData& value);
	virtual void SetMapIntData(MapIntData&& value);


	virtual void SetArray(const Array& value);
	virtual void SetArray(Array&& value);

	virtual void SetList(const List& value);
	virtual void SetList(List&& value);
private:
	/**
	* 数据指针
	*/
	int mType;
	std::shared_ptr<NFCDataValue> m_ptr;
};

/* * * * * * * * * * * * * * * * * * * *
 * Static globals - static-init-safe
 */
struct NFCDataStatics
{
	static bool empty_boolean;
	static int64_t empty_int;
	static double empty_double;
	static std::string empty_string;
	static NFCData::Array empty_array;
	static NFCData::List empty_list;
	static NFCData::MapStringData empty_map_string;
	static NFCData::MapIntData empty_map_int;
};

