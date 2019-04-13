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

	virtual bool AddTable(uint64_t objectId, const std::string& table_name, const std::vector<int>& col_type_list, const int8_t feature) = 0;
	virtual bool AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddTableCallback(uint32_t index, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual int AddTableRow(const std::string& table_name) = 0;
	virtual size_t GetTableRowCount(const std::string& table_name) = 0;
	virtual void Clear() = 0;
	virtual void ClearTable(const std::string& table_name) = 0;

	virtual NFDataTable* GetTable(const std::string& name) const = 0;
	virtual size_t GetCount() const = 0;
	virtual NFDataTable* GetTableByIndex(size_t index) const = 0;

	virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value) = 0;
	virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableInt32(const std::string& name, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableUInt32(const std::string& name, const int row, const int col, const uint32_t value) = 0;
	virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value) = 0;
	virtual bool SetTableUInt64(const std::string& name, const int row, const int col, const uint64_t value) = 0;
	virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value) = 0;
	virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value) = 0;
	virtual bool SetTableString(const std::string& name, const int row, const int col, const std::string& value) = 0;

	virtual bool GetTableBool(const std::string& name, const int row, const int col) const = 0;
	virtual int32_t GetTableInt(const std::string& name, const int row, const int col) const = 0;
	virtual int32_t GetTableInt32(const std::string& name, const int row, const int col) const = 0;
	virtual uint32_t GetTableUInt32(const std::string& name, const int row, const int col) const = 0;
	virtual int64_t GetTableInt64(const std::string& name, const int row, const int col) const = 0;
	virtual uint64_t GetTableUInt64(const std::string& name, const int row, const int col) const = 0;
	virtual float GetTableFloat(const std::string& name, const int row, const int col) const = 0;
	virtual double GetTableDouble(const std::string& name, const int row, const int col) const = 0;
	virtual const std::string& GetTableString(const std::string& name, const int row, const int col) const = 0;

	virtual const NFCData::Array& GetTableArray(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::List& GetTableList(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::MapStringData& GetTableMapStringData(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::MapIntData& GetTableMapIntData(const std::string&, size_t row, size_t col) const = 0;

	virtual NFCData::Array* MutableTableArray(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::List* MutableTableList(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::MapStringData* MutableTableMapStringData(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::MapIntData* MutableTableMapIntData(const std::string&, size_t row, size_t col) = 0;

	virtual bool AddTableArrayItem(const std::string&, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableListItem(const std::string&, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableMapStringItem(const std::string&, size_t row, size_t col, const std::string& key, const NFCData& value) = 0;
	virtual bool AddTableMapIntItem(const std::string&, size_t row, size_t col, uint64_t key, const NFCData& value) = 0;

	virtual bool SetTableBool(uint32_t index, const int row, const int col, const bool value) = 0;
	virtual bool SetTableInt(uint32_t index, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableInt32(uint32_t index, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableUInt32(uint32_t index, const int row, const int col, const uint32_t value) = 0;
	virtual bool SetTableInt64(uint32_t index, const int row, const int col, const int64_t value) = 0;
	virtual bool SetTableUInt64(uint32_t index, const int row, const int col, const uint64_t value) = 0;
	virtual bool SetTableFloat(uint32_t index, const int row, const int col, const float value) = 0;
	virtual bool SetTableDouble(uint32_t index, const int row, const int col, const double value) = 0;
	virtual bool SetTableString(uint32_t index, const int row, const int col, const std::string& value) = 0;

	virtual bool GetTableBool(uint32_t index, const int row, const int col) const = 0;
	virtual int32_t GetTableInt(uint32_t index, const int row, const int col) const = 0;
	virtual int32_t GetTableInt32(uint32_t index, const int row, const int col) const = 0;
	virtual uint32_t GetTableUInt32(uint32_t index, const int row, const int col) const = 0;
	virtual int64_t GetTableInt64(uint32_t index, const int row, const int col) const = 0;
	virtual uint64_t GetTableUInt64(uint32_t index, const int row, const int col) const = 0;
	virtual float GetTableFloat(uint32_t index, const int row, const int col) const = 0;
	virtual double GetTableDouble(uint32_t index, const int row, const int col) const = 0;
	virtual const std::string& GetTableString(uint32_t index, const int row, const int col) const = 0;

	virtual const NFCData::Array& GetTableArray(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::List& GetTableList(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::MapStringData& GetTableMapStringData(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::MapIntData& GetTableMapIntData(uint32_t index, size_t row, size_t col) const = 0;

	virtual NFCData::Array* MutableTableArray(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::List* MutableTableList(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::MapStringData* MutableTableMapStringData(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::MapIntData* MutableTableMapIntData(uint32_t index, size_t row, size_t col) = 0;

	virtual bool AddTableArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableListItem(uint32_t index, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value) = 0;
	virtual bool AddTableMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value) = 0;
};

