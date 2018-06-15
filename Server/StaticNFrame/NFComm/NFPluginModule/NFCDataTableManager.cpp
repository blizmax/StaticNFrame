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
    mTableCallbacks.clear();
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

bool NFCDataTableManager::AddTable(uint64_t objectId, const std::string table_name, const NFCData& col_type_list, const int8_t feature)
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

bool NFCDataTableManager::GetTableData(const std::string& name, const int row, const int col, NFCData& value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    return pTable->GetValue(row, col, value);
}

void NFCDataTableManager::OnEventHandler(const uint64_t entity_id, const DATA_TABLE_EVENT_DATA& xEventData, const NFCData& oldData, const NFCData& newData)
{
    for(auto& iter : mTableCallbacks)
    {
        //TODO:check name from xEventData
        //xEventData.name
        (*iter)(entity_id, xEventData, oldData, newData);
    }
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
    //table_name
    mTableCallbacks.push_back(cb);
    return true;
}

void NFCDataTableManager::Clear()
{
    ReleaseAll();
}

NFDataTable* NFCDataTableManager::GetTable(const std::string& name)
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

NFDataTable* NFCDataTableManager::GetTableByIndex(size_t index)
{
    NF_ASSERT_RET_VAL(index < GetCount(), nullptr);
    return mTables[index];
}

bool NFCDataTableManager::SetTableBool(const std::string& name, const int row, const int col, const bool value)
{
    NFDataTable* pTable = GetTable(name);
	if (pTable == nullptr) return false;

	NFCData* pData = pTable->MutableData(row, col);
	if (pData == nullptr) return false;

    //callback
    do
    {
		NFCData oldData;
		if (!pTable->GetValue(row, col, oldData))
		{
			NF_ASSERT_RET_VAL(0, false);	
		}

        if(oldData.GetBool() == value)
        {
            return true;
        }

        if(!mTableCallbacks.empty())
        {
			NFCData newData;
			newData.SetBool(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(mObjectId, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetBool(row, col, value);
}

bool NFCDataTableManager::SetTableInt(const char* name, const int row, const int col, const int32_t value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    //callback
    do
    {
        AFCData oldData;
        if(!GetTableData(name, row, col, oldData))
        {
            ARK_ASSERT_RET_VAL(0, false);
        }

        if(oldData.GetInt() == value)
        {
            return false;
        }

        if(!mTableCallbacks.empty())
        {
            AFCData newData;
            newData.SetInt(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(self, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetInt(row, col, value);
}

bool NFCDataTableManager::SetTableInt64(const char* name, const int row, const int col, const int64_t value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    //callback
    do
    {
        AFCData oldData;
        if(!GetTableData(name, row, col, oldData))
        {
            ARK_ASSERT_RET_VAL(0, false);
        }

        if(oldData.GetInt64() == value)
        {
            return false;
        }

        if(!mTableCallbacks.empty())
        {
            AFCData newData;
            newData.SetInt64(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(self, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetInt64(row, col, value);
}

bool NFCDataTableManager::SetTableFloat(const char* name, const int row, const int col, const float value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    //callback
    do
    {
        AFCData oldData;
        if(!GetTableData(name, row, col, oldData))
        {
            ARK_ASSERT_RET_VAL(0, false);
        }

        if(AFMisc::IsFloatEqual(oldData.GetFloat(), value))
        {
            return false;
        }

        if(!mTableCallbacks.empty())
        {
            AFCData newData;
            newData.SetFloat(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(self, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetFloat(row, col, value);
}

bool NFCDataTableManager::SetTableDouble(const char* name, const int row, const int col, const double value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    //callback
    do
    {
        AFCData oldData;
        if(!GetTableData(name, row, col, oldData))
        {
            ARK_ASSERT_RET_VAL(0, false);
        }

        if(AFMisc::IsDoubleEqual(oldData.GetDouble(), value))
        {
            return false;
        }

        if(!mTableCallbacks.empty())
        {
            AFCData newData;
            newData.SetDouble(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(self, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetDouble(row, col, value);
}

bool NFCDataTableManager::SetTableString(const char* name, const int row, const int col, const char* value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    //callback
    do
    {
        AFCData oldData;
        if(!GetTableData(name, row, col, oldData))
        {
            ARK_ASSERT_RET_VAL(0, false);
        }

        if(ARK_STRICMP(oldData.GetString(), value) == 0)
        {
            return false;
        }

        if(!mTableCallbacks.empty())
        {
            AFCData newData;
            newData.SetString(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(self, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetString(row, col, value);
}

bool NFCDataTableManager::SetTableObject(const char* name, const int row, const int col, const AFGUID& value)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    //callback
    do
    {
        AFCData oldData;
        if(!GetTableData(name, row, col, oldData))
        {
            ARK_ASSERT_RET_VAL(0, false);
        }

        if(oldData.GetObject() == value)
        {
            return false;
        }

        if(!mTableCallbacks.empty())
        {
            AFCData newData;
            newData.SetObject(value);

            DATA_TABLE_EVENT_DATA xTableEventData;
            xTableEventData.nOpType = NFDataTable::TABLE_UPDATE;
            xTableEventData.nRow = row;
            xTableEventData.nCol = col;
            xTableEventData.strName = name;

            OnEventHandler(self, xTableEventData, oldData, newData);
        }
    }
    while(0);

    return pTable->SetObject(row, col, value);
}

bool NFCDataTableManager::GetTableBool(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return false;
    }

    return pTable->GetBool(row, col);
}

int32_t NFCDataTableManager::GetTableInt(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NULL_INT;
    }

    return pTable->GetInt(row, col);
}

int64_t NFCDataTableManager::GetTableInt64(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NULL_INT64;
    }

    return pTable->GetInt64(row, col);
}

float NFCDataTableManager::GetTableFloat(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NULL_FLOAT;
    }

    return pTable->GetFloat(row, col);
}

double NFCDataTableManager::GetTableDouble(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NULL_DOUBLE;
    }

    return pTable->GetDouble(row, col);
}

const char* NFCDataTableManager::GetTableString(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NULL_STR.c_str();
    }

    return pTable->GetString(row, col);
}

const AFGUID NFCDataTableManager::GetTableObject(const char* name, const int row, const int col)
{
    NFDataTable* pTable = GetTable(name);
    if(pTable == nullptr)
    {
        return NULL_GUID;
    }

    return pTable->GetObject(row, col);
}