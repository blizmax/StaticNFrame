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

    virtual void Clear() final;
    virtual NFDataTable* GetTable(const std::string& name) override;
    virtual size_t GetCount() const;
    virtual NFDataTable* GetTableByIndex(size_t index);

    virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value);
    virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value);
    virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value);
    virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value);
    virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value);
    virtual bool SetTableString(const std::string& name, const int row, const int col, const char* value);

    virtual bool GetTableBool(const std::string& name, const int row, const int col);
    virtual int32_t GetTableInt(const std::string& name, const int row, const int col);
    virtual int64_t GetTableInt64(const std::string& name, const int row, const int col);
    virtual float GetTableFloat(const std::string& name, const int row, const int col);
    virtual double GetTableDouble(const std::string& name, const int row, const int col);
    virtual const std::string& GetTableString(const std::string& name, const int row, const int col);

	virtual const NFCData::Array& GetArray(const std::string&, size_t row, size_t col) const;
	virtual const NFCData::List& GetList(const std::string&, size_t row, size_t col) const;
	virtual const NFCData::MapStringData& GetMapStringObject(const std::string&, size_t row, size_t col) const;
	virtual const NFCData::MapIntData& GetMapIntObject(const std::string&, size_t row, size_t col) const;

	virtual NFCData::Array* MutableArray(const std::string&, size_t row, size_t col);
	virtual NFCData::List* MutableList(const std::string&, size_t row, size_t col);
	virtual NFCData::MapStringData* MutableMapStringData(const std::string&, size_t row, size_t col);
	virtual NFCData::MapIntData* MutableMapIntData(const std::string&, size_t row, size_t col);

	virtual bool AddArrayItem(const std::string&, size_t row, size_t col, const NFCData& data);
	virtual bool AddListItem(const std::string&, size_t row, size_t col, const NFCData& data);
	virtual bool AddMapStringItem(const std::string&, size_t row, size_t col, const std::string& key, const NFCData& value);
	virtual bool AddMapIntItem(const std::string&, size_t row, size_t col, uint64_t key, const NFCData& value);

    virtual bool SetTableBool(uint32_t index, const int row, const int col, const bool value);
    virtual bool SetTableInt(uint32_t index, const int row, const int col, const int32_t value);
    virtual bool SetTableInt64(uint32_t index, const int row, const int col, const int64_t value);
    virtual bool SetTableFloat(uint32_t index, const int row, const int col, const float value);
    virtual bool SetTableDouble(uint32_t index, const int row, const int col, const double value);
    virtual bool SetTableString(uint32_t index, const int row, const int col, const std::string& value);

    virtual bool GetTableBool(uint32_t index, const int row, const int col);
    virtual int32_t GetTableInt(uint32_t index, const int row, const int col);
    virtual int64_t GetTableInt64(uint32_t index, const int row, const int col);
    virtual float GetTableFloat(uint32_t index, const int row, const int col);
    virtual double GetTableDouble(uint32_t index, const int row, const int col);
    virtual const std::string& GetTableString(uint32_t index, const int row, const int col);

	virtual const NFCData::Array& GetArray(uint32_t index, size_t row, size_t col) const;
	virtual const NFCData::List& GetList(uint32_t index, size_t row, size_t col) const;
	virtual const NFCData::MapStringData& GetMapStringObject(uint32_t index, size_t row, size_t col) const;
	virtual const NFCData::MapIntData& GetMapIntObject(uint32_t index, size_t row, size_t col) const;

	virtual NFCData::Array* MutableArray(uint32_t index, size_t row, size_t col);
	virtual NFCData::List* MutableList(uint32_t index, size_t row, size_t col);
	virtual NFCData::MapStringData* MutableMapStringData(uint32_t index, size_t row, size_t col);
	virtual NFCData::MapIntData* MutableMapIntData(uint32_t index, size_t row, size_t col);

	virtual bool AddArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data);
	virtual bool AddListItem(uint32_t index, size_t row, size_t col, const NFCData& data);
	virtual bool AddMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value);
	virtual bool AddMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value);

protected:
	bool FindIndex(const std::string& name, size_t& index) const;
    bool GetTableData(const std::string& name, const int row, const int col, NFCData& value);

    void OnEventHandler(const uint64_t entity_id, const DATA_TABLE_EVENT_DATA& xEventData, const NFCData& oldData, const NFCData& newData);

    bool AddTableInternal(NFDataTable* pTable);
    void ReleaseAll();

private:
    uint64_t mObjectId;

    std::unordered_map<std::string, uint32_t> mIndices;
	std::vector<NFDataTable*> mTables;
    std::vector<DATA_TABLE_EVENT_FUNCTOR_PTR> mTableCallbacks;
};