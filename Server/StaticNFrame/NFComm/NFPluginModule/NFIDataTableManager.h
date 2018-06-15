// -------------------------------------------------------------------------
//    @FileName         :    AFIDataTableManager.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/15
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once
#include <cstdint>
#include "NFDefine.h"
#include "NFCData.h"

class NFDataTable;

class NFIDataTableManager
{
public:
    virtual ~NFIDataTableManager() = default;

    virtual uint64_t GetObjectId() = 0;

    virtual bool Exist(const std::string& name) const = 0;
    virtual bool Exist(const std::string& name, size_t& index) const = 0;

    virtual bool AddTable(uint64_t objectId, const char* table_name, const NFCData& col_type_list, const int8_t feature) = 0;
    virtual bool AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual void Clear() = 0;
    virtual NFDataTable* GetTable(const std::string& name) const = 0;
    virtual size_t GetCount() const = 0;
    virtual NFDataTable* GetTableByIndex(size_t index) const = 0;

    virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value) = 0;
    virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value) = 0;
    virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value) = 0;
    virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value) = 0;
    virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value) = 0;
    virtual bool SetTableString(const std::string& name, const int row, const int col, const std::string& value) = 0;

    virtual bool GetTableBool(const std::string& name, const int row, const int col) const = 0;
    virtual int32_t GetTableInt(const std::string& name, const int row, const int col) const = 0;
    virtual int64_t GetTableInt64(const std::string& name, const int row, const int col) const = 0;
    virtual float GetTableFloat(const std::string& name, const int row, const int col) const = 0;
    virtual double GetTableDouble(const std::string& name, const int row, const int col) const = 0;
    virtual const std::string& GetTableString(const std::string& name, const int row, const int col) const = 0;

	virtual const NFCData::Array& GetArray(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::List& GetList(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::MapStringData& GetMapStringObject(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::MapIntData& GetMapIntObject(const std::string&, size_t row, size_t col) const = 0;

	virtual NFCData::Array* MutableArray(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::List* MutableList(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::MapStringData* MutableMapStringData(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::MapIntData* MutableMapIntData(const std::string&, size_t row, size_t col) = 0;

	virtual bool AddArrayItem(const std::string&, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddListItem(const std::string&, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddMapStringItem(const std::string&, size_t row, size_t col, const std::string& key, const NFCData& value) = 0;
	virtual bool AddMapIntItem(const std::string&, size_t row, size_t col, uint64_t key, const NFCData& value) = 0;

    virtual bool SetTableBool(uint32_t index, const int row, const int col, const bool value) = 0;
    virtual bool SetTableInt(uint32_t index, const int row, const int col, const int32_t value) = 0;
    virtual bool SetTableInt64(uint32_t index, const int row, const int col, const int64_t value) = 0;
    virtual bool SetTableFloat(uint32_t index, const int row, const int col, const float value) = 0;
    virtual bool SetTableDouble(uint32_t index, const int row, const int col, const double value) = 0;
    virtual bool SetTableString(uint32_t index, const int row, const int col, const std::string& value) = 0;

    virtual bool GetTableBool(uint32_t index, const int row, const int col) const = 0;
    virtual int32_t GetTableInt(uint32_t index, const int row, const int col) const = 0;
    virtual int64_t GetTableInt64(uint32_t index, const int row, const int col) const = 0;
    virtual float GetTableFloat(uint32_t index, const int row, const int col) const = 0;
    virtual double GetTableDouble(uint32_t index, const int row, const int col) const = 0;
    virtual const std::string& GetTableString(uint32_t index, const int row, const int col) const = 0;

	virtual const NFCData::Array& GetArray(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::List& GetList(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::MapStringData& GetMapStringObject(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::MapIntData& GetMapIntObject(uint32_t index, size_t row, size_t col) const = 0;

	virtual NFCData::Array* MutableArray(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::List* MutableList(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::MapStringData* MutableMapStringData(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::MapIntData* MutableMapIntData(uint32_t index, size_t row, size_t col) = 0;

	virtual bool AddArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddListItem(uint32_t index, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value) = 0;
	virtual bool AddMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value) = 0;
};
