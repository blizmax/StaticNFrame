// -------------------------------------------------------------------------
//    @FileName         :    NFDataTable.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/15
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFDataTable.h"
#include "NFBitValue.hpp"
#include <NFComm/NFMath/NFMath.h>

NFDataTable::NFDataTable() noexcept: mFeature(0)
{
}

NFDataTable::~NFDataTable()
{
	ReleaseAll();
}

void NFDataTable::ReleaseAll()
{
	mRowDatas.clear();
}

bool NFDataTable::AddCallback(const DATA_TABLE_EVENT_FUNCTOR_PTR& cb)
{
	mTableCallbacks.push_back(cb);
	return true;
}

void NFDataTable::SetName(const char* value)
{
	NF_ASSERT_RET_NONE(value != nullptr);
	mStrName = value;
}

void NFDataTable::SetName(const std::string& value)
{
	mStrName = value;
}

const std::string& NFDataTable::GetName() const
{
	return mStrName;
}

size_t NFDataTable::GetRowCount() const
{
	return mRowDatas.size();
}

void NFDataTable::SetColCount(size_t value)
{
	assert(value > 0);
	if (mColTypes.size() > 0)
	{
		ReleaseAll();
	}

	mColTypes.resize(value);
}

size_t NFDataTable::GetColCount() const
{
	return mColTypes.size();
}

bool NFDataTable::SetColType(size_t index, int type)
{
	NF_ASSERT(index < mColTypes.size());
	NF_ASSERT(type > DT_UNKNOWN);

	mColTypes[index] = type;
	return true;
}

int NFDataTable::GetColType(int col) const
{
	NF_ASSERT(col < mColTypes.size());

	return mColTypes[col];
}

bool NFDataTable::AddRow()
{
	//default insert row
	NFCData row_data(DT_ARRAY, NFCData::Array());
	for (size_t i = 0; i < mColTypes.size(); i++)
	{
		NFCData tmp;
		tmp.SetDefaultValue(mColTypes[i]);
		row_data.AddArrayItem(tmp);
	}
	mRowDatas.push_back(row_data);

	return true;
}

bool NFDataTable::AddRow(size_t row)
{
	NFCData row_data(DT_ARRAY, NFCData::Array());
	for (size_t i = 0; i < mColTypes.size(); i++)
	{
		NFCData tmp;
		tmp.SetDefaultValue(mColTypes[i]);
		row_data.AddArrayItem(tmp);
	}

	if (row >= GetRowCount())
	{
		mRowDatas.push_back(row_data);
	}
	else
	{
		mRowDatas[row] = row_data;
	}

	return true;
}

bool NFDataTable::DeleteRow(size_t row)
{
	assert(row < mRowDatas.size());

	mRowDatas.erase(mRowDatas.begin() + row);

	return true;
}

bool NFDataTable::AddRow(size_t row, const NFCData& data)
{
	NF_ASSERT(data.GetType() == DT_ARRAY);
	size_t col_num = GetColCount();
	if (data.GetArray().size() != col_num)
	{
		NF_ASSERT_MSG(0, "data size is not equal with col_num, please check your arg.");
		return false;
	}

	const NFCData::Array& array = data.GetArray();
	for (size_t i = 0; i < col_num; i++)
	{
		NF_ASSERT_MSG(mColTypes[i] == array[i].GetType(), "data size type is not equal with col_num type, please check your arg.");
	}

	if (row >= GetRowCount())
	{
		mRowDatas.push_back(data);
	}
	else
	{
		mRowDatas[row] = data;
	}

	return true;
}

void NFDataTable::Clear()
{
	ReleaseAll();
}

void NFDataTable::SetFeature(int8_t new_feature)
{
	this->mFeature = new_feature;
}

int8_t NFDataTable::GetFeature() const
{
	return mFeature;
}

void NFDataTable::SetPublic()
{
	NFBitValue<int8_t>::SetBitValue(mFeature, TABLE_PUBLIC);
}

bool NFDataTable::IsPublic() const
{
	return NFBitValue<int8_t>::HaveBitValue(mFeature, TABLE_PUBLIC);
}

void NFDataTable::SetPrivate()
{
	NFBitValue<int8_t>::SetBitValue(mFeature, TABLE_PRIVATE);
}

bool NFDataTable::IsPrivate() const
{
	return NFBitValue<int8_t>::HaveBitValue(mFeature, TABLE_PRIVATE);
}

void NFDataTable::SetRealTime()
{
	NFBitValue<int8_t>::SetBitValue(mFeature, TABLE_REAL_TIME);
}

bool NFDataTable::IsRealTime() const
{
	return NFBitValue<int8_t>::HaveBitValue(mFeature, TABLE_REAL_TIME);
}

void NFDataTable::SetSave()
{
	NFBitValue<int8_t>::SetBitValue(mFeature, TABLE_SAVE);
}

bool NFDataTable::IsSave() const
{
	return NFBitValue<int8_t>::HaveBitValue(mFeature, TABLE_SAVE);
}

bool NFDataTable::SetValue(size_t row, size_t col, const NFCData& value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col] = value;

	return true;
}

bool NFDataTable::SetBool(size_t row, size_t col, const bool value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetBool(value);
	return true;
}

bool NFDataTable::SetInt(size_t row, size_t col, const int value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetInt(value);
	return true;
}

bool NFDataTable::SetInt32(size_t row, size_t col, const int32_t value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetInt32(value);
	return true;
}

bool NFDataTable::SetUInt32(size_t row, size_t col, const uint32_t value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetUInt32(value);
	return true;
}

bool NFDataTable::SetInt64(size_t row, size_t col, const int64_t value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetInt64(value);
	return true;
}

bool NFDataTable::SetUInt64(size_t row, size_t col, const uint64_t value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetUInt64(value);
	return true;
}

bool NFDataTable::SetFloat(size_t row, size_t col, const float value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetFloat(value);
	return true;
}

bool NFDataTable::SetDouble(size_t row, size_t col, const double value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetDouble(value);
	return true;
}

bool NFDataTable::SetString(size_t row, size_t col, const char* value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()) || value == nullptr)
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetString(value);
	return true;
}

bool NFDataTable::SetString(size_t row, size_t col, const std::string& value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	NF_ASSERT_RET_VAL(pArray != nullptr, false);

	(*pArray)[col].SetString(value);
	return true;
}

const NFCData::Array& NFDataTable::GetArray(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_array;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetArray();
}

const NFCData::List& NFDataTable::GetList(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_list;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetList();
}

const NFCData::MapStringData& NFDataTable::GetMapStringObject(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_map_string;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetMapStringData();
}

const NFCData::MapIntData& NFDataTable::GetMapIntObject(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_map_int;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetMapIntData();
}

NFCData::Array* NFDataTable::MutableArray(size_t row, size_t col)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return nullptr;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	if (pArray == nullptr) return nullptr;

	return (*pArray)[col].MutableArray();
}

NFCData::List* NFDataTable::MutableList(size_t row, size_t col)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return nullptr;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	if (pArray == nullptr) return nullptr;

	return (*pArray)[col].MutableList();
}

NFCData::MapStringData* NFDataTable::MutableMapStringData(size_t row, size_t col)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return nullptr;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	if (pArray == nullptr) return nullptr;

	return (*pArray)[col].MutableMapStringData();
}

NFCData::MapIntData* NFDataTable::MutableMapIntData(size_t row, size_t col)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return nullptr;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	if (pArray == nullptr) return nullptr;

	return (*pArray)[col].MutableMapIntData();
}

NFCData* NFDataTable::MutableData(size_t row, size_t col)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return nullptr;
	}

	NFCData& data = mRowDatas[row];
	NFCData::Array* pArray = data.MutableArray();
	if (pArray == nullptr) return nullptr;

	return &(*pArray)[col];
}

bool NFDataTable::AddArrayItem(size_t row, size_t col, const NFCData& data)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData::Array* pArray = mRowDatas[row].MutableArray();
	if (pArray == nullptr) return false;

	(*pArray)[col].AddArrayItem(data);
	return true;
}

bool NFDataTable::AddListItem(size_t row, size_t col, const NFCData& data)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData::Array* pArray = mRowDatas[row].MutableArray();
	if (pArray == nullptr) return false;

	(*pArray)[col].AddListItem(data);
	return true;
}

bool NFDataTable::AddMapStringItem(size_t row, size_t col, const std::string& key, const NFCData& value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData::Array* pArray = mRowDatas[row].MutableArray();
	if (pArray == nullptr) return false;

	(*pArray)[col].AddMapStringItem(key, value);
	return true;
}

bool NFDataTable::AddMapIntItem(size_t row, size_t col, uint64_t key, const NFCData& value)
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	NFCData::Array* pArray = mRowDatas[row].MutableArray();
	if (pArray == nullptr) return false;

	(*pArray)[col].AddMapIntItem(key, value);
	return true;
}

bool NFDataTable::GetValue(size_t row, size_t col, NFCData& value) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return false;
	}

	const NFCData& data = mRowDatas[row];
	value = data.GetArray()[col];

	return true;
}

bool NFDataTable::GetBool(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_boolean;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetBool();
}

int NFDataTable::GetInt(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_int;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetInt();;
}

int32_t NFDataTable::GetInt32(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_int;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetInt32();
}

uint32_t NFDataTable::GetUInt32(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return static_cast<uint32_t>(NFCDataStatics::empty_int);
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetUInt32();
}

int64_t NFDataTable::GetInt64(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_int;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetInt64();
}

uint64_t NFDataTable::GetUInt64(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return static_cast<uint64_t>(NFCDataStatics::empty_int);
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetUInt64();
}

float NFDataTable::GetFloat(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_double;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetFloat();
}

double NFDataTable::GetDouble(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_double;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetDouble();
}

const std::string& NFDataTable::GetString(size_t row, size_t col) const
{
	if ((row >= GetRowCount()) || (col >= GetColCount()))
	{
		return NFCDataStatics::empty_string;
	}

	const NFCData& data = mRowDatas[row];
	return data.GetArray()[col].GetString();
}

bool NFDataTable::GetColTypeList(NFCData& col_type_list) const
{
	col_type_list.SetDefaultValue(DT_ARRAY);
	for (size_t i = 0; i < mColTypes.size(); ++i)
	{
		NFCData data;
		data.SetDefaultValue(mColTypes[i]);
		col_type_list.AddArrayItem(data);
	}

	return true;
}

int NFDataTable::FindRow(size_t col, const NFCData& key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	switch (key.GetType())
	{
	case DT_BOOLEAN:
		return FindBool(col, key.GetBool(), begin_row);
	case DT_INT:
		return FindInt64(col, key.GetInt64(), begin_row);
	case DT_DOUBLE:
		return FindDouble(col, key.GetDouble(), begin_row);
	case DT_STRING:
		return FindString(col, key.GetString(), begin_row);
	default:
		break;
	}

	return -1;
}

int NFDataTable::FindBool(size_t col, const bool key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	size_t row_num = GetRowCount();
	if (begin_row >= row_num)
	{
		return -1;
	}

	for (size_t i = begin_row; i < row_num; ++i)
	{
		const NFCData& row_data = mRowDatas[i];
		if (row_data.GetArray()[col].GetBool() == key)
		{
			return i;
		}
	}

	return -1;
}

int NFDataTable::FindInt(size_t col, const int key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	size_t row_num = GetRowCount();
	if (begin_row >= row_num)
	{
		return -1;
	}

	for (size_t i = begin_row; i < row_num; ++i)
	{
		const NFCData& row_data = mRowDatas[i];
		if (row_data.GetArray()[col].GetInt() == key)
		{
			return i;
		}
	}

	return -1;
}

int NFDataTable::FindInt64(size_t col, const int64_t key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	size_t row_num = GetRowCount();
	if (begin_row >= row_num)
	{
		return -1;
	}

	for (size_t i = begin_row; i < row_num; ++i)
	{
		const NFCData& row_data = mRowDatas[i];
		if (row_data.GetArray()[col].GetInt64() == key)
		{
			return i;
		}
	}

	return -1;
}

int NFDataTable::FindFloat(size_t col, const float key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	size_t row_num = GetRowCount();
	if (begin_row >= row_num)
	{
		return -1;
	}

	for (size_t i = begin_row; i < row_num; ++i)
	{
		const NFCData& row_data = mRowDatas[i];
		if (almostEqual(row_data.GetArray()[col].GetFloat(), key))
		{
			return i;
		}
	}

	return -1;
}

int NFDataTable::FindDouble(size_t col, const double key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	size_t row_num = GetRowCount();
	if (begin_row >= row_num)
	{
		return -1;
	}

	for (size_t i = begin_row; i < row_num; ++i)
	{
		const NFCData& row_data = mRowDatas[i];
		if (almostEqual(row_data.GetArray()[col].GetDouble(), key))
		{
			return i;
		}
	}

	return -1;
}

int NFDataTable::FindString(size_t col, const std::string& key, size_t begin_row /*= 0*/) const
{
	if (col >= GetColCount())
	{
		return -1;
	}

	size_t row_num = GetRowCount();
	if (begin_row >= row_num)
	{
		return -1;
	}

	for (size_t i = begin_row; i < row_num; ++i)
	{
		const NFCData& row_data = mRowDatas[i];
		if (row_data.GetArray()[col].GetString() == key)
		{
			return i;
		}
	}

	return -1;
}

bool NFDataTable::QueryRow(const int row, NFCData& varList) const
{
	NF_ASSERT_RET_VAL(row < mRowDatas.size(), false);

	varList.SetDefaultValue(DT_ARRAY);
	const NFCData& rowData = mRowDatas[row];
	for (size_t i = 0; i < mColTypes.size(); ++i)
	{
		const NFCData& subData = rowData.GetArray()[i];
		varList.AddArrayItem(subData);
	}

	return true;
}

void NFDataTable::OnEventHandler(const uint64_t entity_id, const DATA_TABLE_EVENT_DATA& xEventData, const NFCData& oldData, const NFCData& newData)
{
	for (auto& iter : mTableCallbacks)
	{
		//TODO:check name from xEventData
		//xEventData.name
		(*iter)(entity_id, xEventData, oldData, newData);
	}
}

