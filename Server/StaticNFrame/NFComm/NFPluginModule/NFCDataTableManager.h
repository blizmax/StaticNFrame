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

    virtual bool AddTable(uint64_t objectId, const std::string table_name, const NFCData& col_type_list, const int8_t feature);
    virtual bool AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) override;

    virtual void Clear() override final;
    virtual NFDataTable* GetTable(const std::string& name) const override;
    virtual size_t GetCount() const override;
    virtual NFDataTable* GetTableByIndex(size_t index) const override;

    virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value) override;
    virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value) override;
    virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value) override;
    virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value) override;
    virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value) override;
    virtual bool SetTableString(const std::string& name, const int row, const int col, const std::string& value) override;

    virtual bool GetTableBool(const std::string& name, const int row, const int col) const override;
    virtual int32_t GetTableInt(const std::string& name, const int row, const int col) const override;
    virtual int64_t GetTableInt64(const std::string& name, const int row, const int col) const override;
    virtual float GetTableFloat(const std::string& name, const int row, const int col) const override;
    virtual double GetTableDouble(const std::string& name, const int row, const int col) const override;
    virtual const std::string& GetTableString(const std::string& name, const int row, const int col) const override;

	virtual const NFCData::Array& GetArray(const std::string& name, size_t row, size_t col) const override;
	virtual const NFCData::List& GetList(const std::string& name, size_t row, size_t col) const override;
	virtual const NFCData::MapStringData& GetMapStringObject(const std::string& name, size_t row, size_t col) const override;
	virtual const NFCData::MapIntData& GetMapIntObject(const std::string& name, size_t row, size_t col) const override;

	virtual NFCData::Array* MutableArray(const std::string& name, size_t row, size_t col) override;
	virtual NFCData::List* MutableList(const std::string& name, size_t row, size_t col) override;
	virtual NFCData::MapStringData* MutableMapStringData(const std::string& name, size_t row, size_t col) override;
	virtual NFCData::MapIntData* MutableMapIntData(const std::string& name, size_t row, size_t col) override;

	virtual bool AddArrayItem(const std::string& name, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddListItem(const std::string& name, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddMapStringItem(const std::string& name, size_t row, size_t col, const std::string& key, const NFCData& value) override;
	virtual bool AddMapIntItem(const std::string& name, size_t row, size_t col, uint64_t key, const NFCData& value) override;

    virtual bool SetTableBool(uint32_t index, const int row, const int col, const bool value) override;
    virtual bool SetTableInt(uint32_t index, const int row, const int col, const int32_t value) override;
    virtual bool SetTableInt64(uint32_t index, const int row, const int col, const int64_t value) override;
    virtual bool SetTableFloat(uint32_t index, const int row, const int col, const float value) override;
    virtual bool SetTableDouble(uint32_t index, const int row, const int col, const double value) override;
    virtual bool SetTableString(uint32_t index, const int row, const int col, const std::string& value) override;

    virtual bool GetTableBool(uint32_t index, const int row, const int col) const override;
    virtual int32_t GetTableInt(uint32_t index, const int row, const int col) const override;
    virtual int64_t GetTableInt64(uint32_t index, const int row, const int col) const override;
    virtual float GetTableFloat(uint32_t index, const int row, const int col) const override;
    virtual double GetTableDouble(uint32_t index, const int row, const int col) const override;
    virtual const std::string& GetTableString(uint32_t index, const int row, const int col) const override;

	virtual const NFCData::Array& GetArray(uint32_t index, size_t row, size_t col) const override;
	virtual const NFCData::List& GetList(uint32_t index, size_t row, size_t col) const override;
	virtual const NFCData::MapStringData& GetMapStringObject(uint32_t index, size_t row, size_t col) const override;
	virtual const NFCData::MapIntData& GetMapIntObject(uint32_t index, size_t row, size_t col) const override;

	virtual NFCData::Array* MutableArray(uint32_t index, size_t row, size_t col) override;
	virtual NFCData::List* MutableList(uint32_t index, size_t row, size_t col) override;
	virtual NFCData::MapStringData* MutableMapStringData(uint32_t index, size_t row, size_t col) override;
	virtual NFCData::MapIntData* MutableMapIntData(uint32_t index, size_t row, size_t col) override;

	virtual bool AddArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddListItem(uint32_t index, size_t row, size_t col, const NFCData& data) override;
	virtual bool AddMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value) override;
	virtual bool AddMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value) override;
protected:
	bool FindIndex(const std::string& name, size_t& index) const;
    bool GetTableData(const std::string& name, const int row, const int col, NFCData& value) const;

    void OnEventHandler(const uint64_t entity_id, const DATA_TABLE_EVENT_DATA& xEventData, const NFCData& oldData, const NFCData& newData);

    bool AddTableInternal(NFDataTable* pTable);
    void ReleaseAll();
private:
    uint64_t mObjectId;

    std::unordered_map<std::string, uint32_t> mIndices;
	std::vector<NFDataTable*> mTables;
    std::vector<DATA_TABLE_EVENT_FUNCTOR_PTR> mTableCallbacks;
};