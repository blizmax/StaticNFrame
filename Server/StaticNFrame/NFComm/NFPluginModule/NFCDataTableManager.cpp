// -------------------------------------------------------------------------
//    @FileName         :    NFCDataTableManager.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/15
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFCDataTableManager.h"
#include "NFDataTable.h"
#include <NFComm/NFMath/NFMath.h>

NFCDataTableManager::NFCDataTableManager(const uint64_t objectId)
    : mObjectId(objectId)
{

}

NFCDataTableManager::~NFCDataTableManager()
{
    ReleaseAll();
}

uint64_t NFCDataTableManager::GetObjectId()
{
	return mObjectId;
}

void NFCDataTableManager::ReleaseAll()
{
    for(size_t i = 0; i < mTables.size(); ++i)
    {
        NFSafeDelete(mTables[i]);
    }

	mTables.clear();
}

bool NFCDataTableManager::Exist(const std::string& name) const
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return false;
	}

	if (index < mTables.size())
	{
		if (mTables[index] != nullptr)
		{
			return true;
		}
	}

	return false;
}

bool NFCDataTableManager::Exist(const std::string& name, size_t& index) const
{
	return FindIndex(name, index);
}

bool NFCDataTableManager::AddTable(uint64_t objectId, const std::string& table_name, const NFCData& col_type_list, const int8_t feature)
{
    NF_ASSERT_MSG(table_name.size() > 0, "Table name is invalid");
    NF_ASSERT_MSG(col_type_list.GetType() == DT_ARRAY, "Table name is invalid");

    NFDataTable* pTable = NF_NEW NFDataTable();
    pTable->SetName(table_name);
    pTable->SetColCount(col_type_list.GetArray().size());
    for(size_t i = 0; i < col_type_list.GetArray().size(); ++i)
    {
        pTable->SetColType(i, col_type_list.GetArray()[i].GetType());
    }

    pTable->SetFeature(feature);

    return AddTableInternal(pTable);
}

bool NFCDataTableManager::GetTableData(const std::string& name, const int row, const int col, NFCData& value) const
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    return pTable->GetValue(row, col, value);
}

bool NFCDataTableManager::FindIndex(const std::string& name, size_t& index) const
{
	auto it = mIndices.find(name);
	if (it != mIndices.end())
	{
		index = it->second;
		return true;
	}

	return false;
}

bool NFCDataTableManager::AddTableInternal(NFDataTable* pTable)
{
    assert(pTable != nullptr);
	if (Exist(pTable->GetName()))
	{
		return false;
	}

	mIndices.emplace(pTable->GetName(), mTables.size());
	mTables.push_back(pTable);

	return true;
}

bool NFCDataTableManager::AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb)
{
	size_t index;
	if (!FindIndex(table_name, index))
	{
		return false;
	}

	NF_ASSERT(index < mTables.size());
	return mTables[index]->AddCallback(cb);
}

bool NFCDataTableManager::AddTableCallback(uint32_t index, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb)
{
	NF_ASSERT(index < mTables.size());
	return mTables[index]->AddCallback(cb);
}

void NFCDataTableManager::Clear()
{
    ReleaseAll();
}

NFDataTable* NFCDataTableManager::GetTable(const std::string& name) const
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return nullptr;
	}

	NF_ASSERT(index < mTables.size());
	return mTables[index];
}

size_t NFCDataTableManager::GetCount() const
{
    return mTables.size();
}

NFDataTable* NFCDataTableManager::GetTableByIndex(size_t index) const
{
    NF_ASSERT_RET_VAL(index < GetCount(), nullptr);
    return mTables[index];
}

bool NFCDataTableManager::SetTableBool(const std::string& name, const int row, const int col, const bool value)
{
    NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetBool(row, col, value);
}

bool NFCDataTableManager::SetTableInt(const std::string& name, const int row, const int col, const int32_t value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetInt(row, col, value);
}

bool NFCDataTableManager::SetTableInt32(const std::string& name, const int row, const int col, const int32_t value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetInt32(row, col, value);
}

bool NFCDataTableManager::SetTableUInt32(const std::string& name, const int row, const int col, const uint32_t value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetUInt32(row, col, value);
}

bool NFCDataTableManager::SetTableInt64(const std::string& name, const int row, const int col, const int64_t value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetInt64(row, col, value);
}

bool NFCDataTableManager::SetTableUInt64(const std::string& name, const int row, const int col, const uint64_t value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetUInt64(row, col, value);
}

bool NFCDataTableManager::SetTableFloat(const std::string& name, const int row, const int col, const float value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetFloat(row, col, value);
}

bool NFCDataTableManager::SetTableDouble(const std::string& name, const int row, const int col, const double value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetDouble(row, col, value);
}

bool NFCDataTableManager::SetTableString(const std::string& name, const int row, const int col, const std::string& value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	return pTable->SetString(row, col, value);
}

bool NFCDataTableManager::GetTableBool(const std::string& name, const int row, const int col) const
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NFCDataStatics::empty_boolean;
    }

    return pTable->GetBool(row, col);
}

int32_t NFCDataTableManager::GetTableInt(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return static_cast<int32_t>(NFCDataStatics::empty_int);
	}

	return pTable->GetInt(row, col);
}

int32_t NFCDataTableManager::GetTableInt32(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetInt32(row, col);
}

uint32_t NFCDataTableManager::GetTableUInt32(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetUInt32(row, col);
}

int64_t NFCDataTableManager::GetTableInt64(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetInt64(row, col);
}

uint64_t NFCDataTableManager::GetTableUInt64(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetUInt64(row, col);
}

float NFCDataTableManager::GetTableFloat(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return static_cast<float>(NFCDataStatics::empty_double);
	}

	return pTable->GetDouble(row, col);
}

double NFCDataTableManager::GetTableDouble(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_double;
	}

	return pTable->GetDouble(row, col);
}

const std::string& NFCDataTableManager::GetTableString(const std::string& name, const int row, const int col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_string;
	}

	return pTable->GetString(row, col);
}

const NFCData::Array& NFCDataTableManager::GetTableArray(const std::string& name, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_array;
	}

	return pTable->GetArray(row, col);
}

const NFCData::List& NFCDataTableManager::GetTableList(const std::string& name, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_list;
	}

	return pTable->GetList(row, col);
}

const NFCData::MapStringData& NFCDataTableManager::GetTableMapStringData(const std::string& name, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_map_string;
	}

	return pTable->GetMapStringObject(row, col);
}

const NFCData::MapIntData& NFCDataTableManager::GetTableMapIntData(const std::string& name, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_map_int;
	}

	return pTable->GetMapIntObject(row, col);
}

NFCData::Array* NFCDataTableManager::MutableTableArray(const std::string& name, size_t row, size_t col)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableArray(row, col);
}

NFCData::List* NFCDataTableManager::MutableTableList(const std::string& name, size_t row, size_t col)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableList(row, col);
}

NFCData::MapStringData* NFCDataTableManager::MutableTableMapStringData(const std::string& name, size_t row, size_t col)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableMapStringData(row, col);
}

NFCData::MapIntData* NFCDataTableManager::MutableTableMapIntData(const std::string& name, size_t row, size_t col)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableMapIntData(row, col);
}

bool NFCDataTableManager::AddTableArrayItem(const std::string& name, size_t row, size_t col, const NFCData& data)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddArrayItem(row, col, data);
}

bool NFCDataTableManager::AddTableListItem(const std::string& name, size_t row, size_t col, const NFCData& data)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddListItem(row, col, data);
}

bool NFCDataTableManager::AddTableMapStringItem(const std::string& name, size_t row, size_t col, const std::string& key, const NFCData& value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddMapStringItem(row, col, key, value);
}

bool NFCDataTableManager::AddTableMapIntItem(const std::string& name, size_t row, size_t col, uint64_t key, const NFCData& value)
{
	NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddMapIntItem(row, col, key, value);
}

bool NFCDataTableManager::SetTableBool(uint32_t index, const int row, const int col, const bool value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetBool(row, col, value);
}

bool NFCDataTableManager::SetTableInt(uint32_t index, const int row, const int col, const int32_t value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetInt(row, col, value);
}

bool NFCDataTableManager::SetTableInt32(uint32_t index, const int row, const int col, const int32_t value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetInt32(row, col, value);
}

bool NFCDataTableManager::SetTableUInt32(uint32_t index, const int row, const int col, const uint32_t value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetUInt32(row, col, value);
}

bool NFCDataTableManager::SetTableInt64(uint32_t index, const int row, const int col, const int64_t value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetInt64(row, col, value);
}

bool NFCDataTableManager::SetTableUInt64(uint32_t index, const int row, const int col, const uint64_t value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetUInt64(row, col, value);
}

bool NFCDataTableManager::SetTableFloat(uint32_t index, const int row, const int col, const float value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetFloat(row, col, value);
}

bool NFCDataTableManager::SetTableDouble(uint32_t index, const int row, const int col, const double value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetDouble(row, col, value);
}

bool NFCDataTableManager::SetTableString(uint32_t index, const int row, const int col, const std::string& value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr) return false;

	return pTable->SetString(row, col, value);
}

bool NFCDataTableManager::GetTableBool(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_boolean;
	}

	return pTable->GetBool(row, col);
}

int32_t NFCDataTableManager::GetTableInt(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetInt(row, col);
}

int32_t NFCDataTableManager::GetTableInt32(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetInt32(row, col);
}

uint32_t NFCDataTableManager::GetTableUInt32(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetUInt32(row, col);
}

int64_t NFCDataTableManager::GetTableInt64(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetInt64(row, col);
}

uint64_t NFCDataTableManager::GetTableUInt64(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_int;
	}

	return pTable->GetUInt64(row, col);
}

float NFCDataTableManager::GetTableFloat(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_double;
	}

	return pTable->GetFloat(row, col);
}

double NFCDataTableManager::GetTableDouble(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_double;
	}

	return pTable->GetDouble(row, col);
}

const std::string& NFCDataTableManager::GetTableString(uint32_t index, const int row, const int col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_string;
	}

	return pTable->GetString(row, col);
}

const NFCData::Array& NFCDataTableManager::GetTableArray(uint32_t index, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_array;
	}

	return pTable->GetArray(row, col);
}

const NFCData::List& NFCDataTableManager::GetTableList(uint32_t index, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_list;
	}

	return pTable->GetList(row, col);
}

const NFCData::MapStringData& NFCDataTableManager::GetTableMapStringData(uint32_t index, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_map_string;
	}

	return pTable->GetMapStringObject(row, col);
}

const NFCData::MapIntData& NFCDataTableManager::GetTableMapIntData(uint32_t index, size_t row, size_t col) const
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return NFCDataStatics::empty_map_int;
	}

	return pTable->GetMapIntObject(row, col);
}

NFCData::Array* NFCDataTableManager::MutableTableArray(uint32_t index, size_t row, size_t col)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableArray(row, col);
}

NFCData::List* NFCDataTableManager::MutableTableList(uint32_t index, size_t row, size_t col)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableList(row, col);
}

NFCData::MapStringData* NFCDataTableManager::MutableTableMapStringData(uint32_t index, size_t row, size_t col)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableMapStringData(row, col);
}

NFCData::MapIntData* NFCDataTableManager::MutableTableMapIntData(uint32_t index, size_t row, size_t col)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return nullptr;
	}

	return pTable->MutableMapIntData(row, col);
}

bool NFCDataTableManager::AddTableArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddArrayItem(row, col, data);
}

bool NFCDataTableManager::AddTableListItem(uint32_t index, size_t row, size_t col, const NFCData& data)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddListItem(row, col, data);
}

bool NFCDataTableManager::AddTableMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddMapStringItem(row, col, key, value);
}

bool NFCDataTableManager::AddTableMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value)
{
	NFDataTable* pTable = GetTableByIndex(index);
	if (pTable == nullptr)
	{
		return false;
	}

	return pTable->AddMapIntItem(row, col, key, value);
}
