// -------------------------------------------------------------------------
//    @FileName         :    NFCDataTableManager.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/15
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFIDataTableManager.h"

class NFCDataTableManager : public NFIDataTableManager
{
protected:
	using TableCallbacks = std::vector<DATA_TABLE_EVENT_FUNCTOR_PTR>;

public:
	NFCDataTableManager() = delete;

	explicit NFCDataTableManager(const uint64_t objectId);
	virtual ~NFCDataTableManager();

	virtual uint64_t GetObjectId() override;

	virtual bool Exist(const std::string& name) const override;
	virtual bool Exist(const std::string& name, size_t& index) const override;

	virtual bool AddTable(uint64_t objectId, const std::string& table_name, const std::vector<int>& col_type_list, const int8_t feature) override;
	virtual bool AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) override;
	virtual bool AddTableCallback(uint32_t index, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) override;

	virtual int AddTableRow(const std::string& table_name) override;
	virtual size_t GetTableRowCount(const std::string& table_name) override;
	virtual void Clear() override;
	virtual void ClearTable(const std::string& table_name) override;
	virtual NFDataTable* GetTable(const std::string& name) const override;
	virtual size_t GetCount() const override;
	virtual NFDataTable* GetTableByIndex(size_t index) const override;

	virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value) override;
	virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value) override;
	virtual bool SetTableInt32(const std::string& name, const int row, const int col, const int32_t value) override;
	virtual bool SetTableUInt32(const std::string& name, const int row, const int col, const uint32_t value) override;
	virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value) override;
	virtual bool SetTableUInt64(const std::string& name, const int row, const int col, const uint64_t value) override;
	virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value) override;
	virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value) override;
	virtual bool SetTableString(const std::string& name, const int row, const int col, const std::string& value) override;

	virtual bool GetTableBool(const std::string& name, const int row, const int col) const override;
	virtual int32_t GetTableInt(const std::string& name, const int row, const int col) const override;
	virtual int32_t GetTableInt32(const std::string& name, const int row, const int col) const override;
	virtual uint32_t GetTableUInt32(const std::string& name, const int row, const int col) const override;
	virtual int64_t GetTableInt64(const std::string& name, const int row, const int col) const override;
	virtual uint64_t GetTableUInt64(const std::string& name, const int row, const int col) const override;
	virtual float GetTableFloat(const std::string& name, const int row, const int col) const override;
	virtual double GetTableDouble(const std::string& name, const int row, const int col) const override;
	virtual const std::string& GetTableString(const std::string& name, const int row, const int col) const override;

	virtual const NFCData::Array& GetTableArray(const std::string& name, size_t row, size_t col) const override;
	virtual const NFCData::List& GetTableList(const std::string& name, size_t row, size_t col) const override;
	virtual const NFCData::MapStringData& GetTableMapStringData(const std::string& name, size_t row, size_t col) const override;
	virtual const NFCData::MapIntData& GetTableMapIntData(const std::string& name, size_t row, size_t col) const override;

	virtual NFCData::Array* MutableTableArray(const std::string& name, size_t row, size_t col) override;
	virtual NFCData::List* MutableTableList(const std::string& name, size_t row, size_t col) override;
	virtual NFCData::MapStringData* MutableTableMapStringData(const std::string& name, size_t row, size_t col) override;
	virtual NFCData::MapIntData* MutableTableMapIntData(const std::string& name, size_t row, size_t col) override;

	virtual bool AddTableArrayItem(const std::string& name, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddTableListItem(const std::string& name, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddTableMapStringItem(const std::string& name, size_t row, size_t col, const std::string& key, const NFCData& value) override;
	virtual bool AddTableMapIntItem(const std::string& name, size_t row, size_t col, uint64_t key, const NFCData& value) override;

	virtual bool SetTableBool(uint32_t index, const int row, const int col, const bool value) override;
	virtual bool SetTableInt(uint32_t index, const int row, const int col, const int32_t value) override;
	virtual bool SetTableInt32(uint32_t index, const int row, const int col, const int32_t value) override;
	virtual bool SetTableUInt32(uint32_t index, const int row, const int col, const uint32_t value) override;
	virtual bool SetTableInt64(uint32_t index, const int row, const int col, const int64_t value) override;
	virtual bool SetTableUInt64(uint32_t index, const int row, const int col, const uint64_t value) override;
	virtual bool SetTableFloat(uint32_t index, const int row, const int col, const float value) override;
	virtual bool SetTableDouble(uint32_t index, const int row, const int col, const double value) override;
	virtual bool SetTableString(uint32_t index, const int row, const int col, const std::string& value) override;

	virtual bool GetTableBool(uint32_t index, const int row, const int col) const override;
	virtual int32_t GetTableInt(uint32_t index, const int row, const int col) const override;
	virtual int32_t GetTableInt32(uint32_t index, const int row, const int col) const override;
	virtual uint32_t GetTableUInt32(uint32_t index, const int row, const int col) const override;
	virtual int64_t GetTableInt64(uint32_t index, const int row, const int col) const override;
	virtual uint64_t GetTableUInt64(uint32_t index, const int row, const int col) const override;
	virtual float GetTableFloat(uint32_t index, const int row, const int col) const override;
	virtual double GetTableDouble(uint32_t index, const int row, const int col) const override;
	virtual const std::string& GetTableString(uint32_t index, const int row, const int col) const override;

	virtual const NFCData::Array& GetTableArray(uint32_t index, size_t row, size_t col) const override;
	virtual const NFCData::List& GetTableList(uint32_t index, size_t row, size_t col) const override;
	virtual const NFCData::MapStringData& GetTableMapStringData(uint32_t index, size_t row, size_t col) const override;
	virtual const NFCData::MapIntData& GetTableMapIntData(uint32_t index, size_t row, size_t col) const override;

	virtual NFCData::Array* MutableTableArray(uint32_t index, size_t row, size_t col) override;
	virtual NFCData::List* MutableTableList(uint32_t index, size_t row, size_t col) override;
	virtual NFCData::MapStringData* MutableTableMapStringData(uint32_t index, size_t row, size_t col) override;
	virtual NFCData::MapIntData* MutableTableMapIntData(uint32_t index, size_t row, size_t col) override;

	virtual bool AddTableArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddTableListItem(uint32_t index, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddTableMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value) override;
	virtual bool AddTableMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value) override;
protected:
	bool FindIndex(const std::string& name, size_t& index) const;
	bool GetTableData(const std::string& name, const int row, const int col, NFCData& value) const;

	bool AddTableInternal(NFDataTable* pTable);
	void ReleaseAll();
private:
	uint64_t mObjectId;

	std::unordered_map<std::string, uint32_t> mIndices;
	std::vector<NFDataTable*> mTables;
};

