// -------------------------------------------------------------------------
//    @FileName         :    AFDataTable.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/15
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "NFCData.h"
#include "NFDefine.h"

class NFDataTable
{
public:
	enum DATA_TABLE_FEATURE
	{
		TABLE_PUBLIC = 0, //send to others
		TABLE_PRIVATE = 1, //send to self
		TABLE_REAL_TIME = 2, //send real-time when changed
		TABLE_SAVE = 3, //if need save to database
	};

	enum DATA_TABLE_OP_TYPE
	{
		TABLE_ADD = 0, //add whole row data
		TABLE_DELETE, //delete whole row data
		TABLE_UPDATE, //update row & col cell data
		TABLE_COVERAGE, //coverage whole row data
		TABLE_SWAP, //swap two whole row data
	};

public:
	NFDataTable() noexcept;
	virtual ~NFDataTable();

	void SetName(const std::string& value);
	void SetName(const char* value);
	const std::string& GetName() const;

	size_t GetRowCount() const;

	void SetColCount(size_t value);
	size_t GetColCount() const;

	bool SetColType(size_t index, int type);
	int GetColType(int col) const;

	bool AddRow();
	bool AddRow(size_t row);
	bool DeleteRow(size_t row);
	bool AddRow(size_t row, const NFCData& data);

	void Clear();

	void SetFeature(int8_t new_feature);
	int8_t GetFeature() const;

	void SetPublic();
	bool IsPublic() const;
	void SetPrivate();
	bool IsPrivate() const;
	void SetRealTime();
	bool IsRealTime() const;
	void SetSave();
	bool IsSave() const;

	bool SetValue(size_t row, size_t col, const NFCData& value);
	bool SetBool(size_t row, size_t col, const bool value);
	bool SetInt(size_t row, size_t col, const int value);
	bool SetInt32(size_t row, size_t col, const int32_t value);
	bool SetUInt32(size_t row, size_t col, const uint32_t value);
	bool SetInt64(size_t row, size_t col, const int64_t value);
	bool SetUInt64(size_t row, size_t col, const uint64_t value);
	bool SetFloat(size_t row, size_t col, const float value);
	bool SetDouble(size_t row, size_t col, const double value);
	bool SetString(size_t row, size_t col, const char* value);
	bool SetString(size_t row, size_t col, const std::string& value);

	const NFCData::Array& GetArray(size_t row, size_t col) const;
	const NFCData::List& GetList(size_t row, size_t col) const;
	const NFCData::MapStringData& GetMapStringObject(size_t row, size_t col) const;
	const NFCData::MapIntData& GetMapIntObject(size_t row, size_t col) const;

	NFCData::Array* MutableArray(size_t row, size_t col);
	NFCData::List* MutableList(size_t row, size_t col);
	NFCData::MapStringData* MutableMapStringData(size_t row, size_t col);
	NFCData::MapIntData* MutableMapIntData(size_t row, size_t col);
	NFCData* MutableData(size_t row, size_t col);

	bool AddArrayItem(size_t row, size_t col, const NFCData& data);
	bool AddListItem(size_t row, size_t col, const NFCData& data);
	bool AddMapStringItem(size_t row, size_t col, const std::string& key, const NFCData& value);
	bool AddMapIntItem(size_t row, size_t col, uint64_t key, const NFCData& value);

	bool GetValue(size_t row, size_t col, NFCData& value) const;
	bool GetBool(size_t row, size_t col) const;
	int GetInt(size_t row, size_t col) const;
	int32_t GetInt32(size_t row, size_t col) const;
	uint32_t GetUInt32(size_t row, size_t col) const;
	int64_t GetInt64(size_t row, size_t col) const;
	uint64_t GetUInt64(size_t row, size_t col) const;
	float GetFloat(size_t row, size_t col) const;
	double GetDouble(size_t row, size_t col) const;
	const std::string& GetString(size_t row, size_t col) const;

	bool GetColTypeList(NFCData& col_type_list) const;

	int FindRow(size_t col, const NFCData& key, size_t begin_row = 0) const;
	int FindBool(size_t col, const bool key, size_t begin_row = 0) const;
	int FindInt(size_t col, const int key, size_t begin_row = 0) const;
	int FindInt64(size_t col, const int64_t key, size_t begin_row = 0) const;
	int FindFloat(size_t col, const float key, size_t begin_row = 0) const;
	int FindDouble(size_t col, const double key, size_t begin_row = 0) const;
	int FindString(size_t col, const std::string& key, size_t begin_row = 0) const;

	bool QueryRow(const int row, NFCData& varList) const;

	void ReleaseAll();

	virtual bool AddCallback(const DATA_TABLE_EVENT_FUNCTOR_PTR& cb);
protected:
	void OnEventHandler(const uint64_t entity_id, const DATA_TABLE_EVENT_DATA& xEventData, const NFCData& oldData, const NFCData& newData);
private:
	std::string mStrName; //DataTable name
	int8_t mFeature; //DataTable feature
	std::vector<int> mColTypes; //DataTable column type array
	std::vector<NFCData> mRowDatas; //DataTable data array
	std::vector<DATA_TABLE_EVENT_FUNCTOR_PTR> mTableCallbacks;
};

