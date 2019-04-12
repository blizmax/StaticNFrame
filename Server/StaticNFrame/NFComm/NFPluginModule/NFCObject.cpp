// -------------------------------------------------------------------------
//    @FileName         :    NFCObject.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/10
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#include "NFCObject.h"
#include "NFCDataNodeManager.h"
#include "NFCDataTableManager.h"
#include "NFLogMgr.h"

NFCObject::NFCObject(const uint64_t objectId, NFIPluginManager* pLuginManager)
	: mObjectId(objectId)
{
	m_pPluginManager = pLuginManager;

	m_pNodeManager = NF_NEW NFCDataNodeManager(objectId);
	m_pTableManager = NF_NEW NFCDataTableManager(objectId);
}

NFCObject::~NFCObject()
{
	NF_SAFE_DELETE(m_pNodeManager);
	NF_SAFE_DELETE(m_pTableManager);
}

bool NFCObject::Init()
{
	return true;
}

bool NFCObject::Shut()
{
	return true;
}

bool NFCObject::Execute()
{
	return true;
}

uint64_t NFCObject::GetObjectId()
{
	return mObjectId;
}

bool NFCObject::AddNodeCallBack(const std::string& name, const DATA_NODE_EVENT_FUNCTOR_PTR& cb)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->RegisterCallback(name, cb);
}

bool NFCObject::AddNodeCallBack(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->RegisterCallback(index, cb);
}

bool NFCObject::AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableCallback(table_name, cb);
}

bool NFCObject::AddTableCallback(uint32_t index, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableCallback(index, cb);
}

bool NFCObject::CheckNodeExist(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return (m_pNodeManager->GetNode(name) != nullptr);
}

bool NFCObject::CheckNodeExist(uint32_t index)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return (m_pNodeManager->GetNodeByIndex(index) != nullptr);
}

const NFCData::Array& NFCObject::GetArray(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetArray(name);
}

const NFCData::List& NFCObject::GetList(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetList(name);
}

const NFCData::MapStringData& NFCObject::GetMapStringData(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetMapStringObject(name);
}

const NFCData::MapIntData& NFCObject::GetMapIntData(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetMapIntObject(name);
}

NFCData::Array* NFCObject::MutableArray(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableArray(name);
}

NFCData::List* NFCObject::MutableList(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableList(name);
}

NFCData::MapStringData* NFCObject::MutableMapStringData(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableMapStringData(name);
}

NFCData::MapIntData* NFCObject::MutableMapIntData(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableMapIntData(name);
}

bool NFCObject::AddArrayItem(const std::string& name, const NFCData& data)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddArrayItem(name, data);
}

bool NFCObject::AddListItem(const std::string& name, const NFCData& data)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddListItem(name, data);
}

bool NFCObject::AddMapStringItem(const std::string& name, const std::string& key, const NFCData& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddMapStringItem(name, key, value);
}

bool NFCObject::AddMapIntItem(const std::string& name, uint64_t key, const NFCData& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddMapIntItem(name, key, value);
}

const NFCData::Array& NFCObject::GetArray(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetArray(index);
}

const NFCData::List& NFCObject::GetList(int32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetList(index);
}

const NFCData::MapStringData& NFCObject::GetMapStringData(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetMapStringObject(index);
}

const NFCData::MapIntData& NFCObject::GetMapIntData(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetMapIntObject(index);
}

NFCData::Array* NFCObject::MutableArray(uint32_t index)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableArray(index);
}

NFCData::List* NFCObject::MutableList(uint32_t index)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableList(index);
}

NFCData::MapStringData* NFCObject::MutableMapStringData(uint32_t index)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableMapStringData(index);
}

NFCData::MapIntData* NFCObject::MutableMapIntData(uint32_t index)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->MutableMapIntData(index);
}

bool NFCObject::AddArrayItem(uint32_t index, const NFCData& data)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddArrayItem(index, data);
}

bool NFCObject::AddListItem(uint32_t index, const NFCData& data)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddListItem(index, data);
}

bool NFCObject::AddMapStringItem(uint32_t index, const std::string& key, const NFCData& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddMapStringItem(index, key, value);
}

bool NFCObject::AddMapIntItem(uint32_t index, uint64_t key, const NFCData& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddMapIntItem(index, key, value);
}

bool NFCObject::SetNodeBool(const std::string& name, const bool value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeBool(name, value);
}

bool NFCObject::SetNodeInt(const std::string& name, const int32_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeInt(name, value);
}

bool NFCObject::SetNodeInt32(const std::string& name, const int32_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeInt32(name, value);
}

bool NFCObject::SetNodeUInt32(const std::string& name, const uint32_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeUInt32(name, value);
}

bool NFCObject::SetNodeInt64(const std::string& name, const int64_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeInt64(name, value);
}

bool NFCObject::SetNodeUInt64(const std::string& name, const uint64_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeUInt64(name, value);
}

bool NFCObject::SetNodeFloat(const std::string& name, const float value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeFloat(name, value);
}

bool NFCObject::SetNodeDouble(const std::string& name, const double value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeDouble(name, value);
}

bool NFCObject::SetNodeString(const std::string& name, const std::string& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeString(name, value);
}

bool NFCObject::SetNodeString(const std::string& name, const char* value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	if (value != nullptr)
		return m_pNodeManager->SetNodeString(name, value);
	else
		return m_pNodeManager->SetNodeString(name, std::string());
}

bool NFCObject::SetNodeBool(uint32_t index, const bool value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeBool(index, value);
}

bool NFCObject::SetNodeInt(uint32_t index, const int32_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeInt(index, value);
}

bool NFCObject::SetNodeInt32(uint32_t index, const int32_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeInt32(index, value);
}

bool NFCObject::SetNodeUInt32(uint32_t index, const uint32_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeUInt32(index, value);
}

bool NFCObject::SetNodeInt64(uint32_t index, const int64_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeInt64(index, value);
}

bool NFCObject::SetNodeUInt64(uint32_t index, const uint64_t value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeUInt64(index, value);
}

bool NFCObject::SetNodeFloat(uint32_t index, const float value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeFloat(index, value);
}

bool NFCObject::SetNodeDouble(uint32_t index, const double value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeDouble(index, value);
}

bool NFCObject::SetNodeString(uint32_t index, const std::string& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNodeString(index, value);
}

bool NFCObject::SetNodeString(uint32_t index, const char* value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	if (value != nullptr)
		return m_pNodeManager->SetNodeString(index, value);
	else
		return m_pNodeManager->SetNodeString(index, std::string());
}

bool NFCObject::GetNodeBool(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeBool(name);
}

int32_t NFCObject::GetNodeInt(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt(name);
}

int32_t NFCObject::GetNodeInt32(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt32(name);
}

uint32_t NFCObject::GetNodeUInt32(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeUInt32(name);
}

int64_t NFCObject::GetNodeInt64(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt64(name);
}

uint64_t NFCObject::GetNodeUInt64(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeUInt64(name);
}

float NFCObject::GetNodeFloat(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeFloat(name);
}

double NFCObject::GetNodeDouble(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeDouble(name);
}

const std::string& NFCObject::GetNodeString(const std::string& name) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeString(name);
}

bool NFCObject::GetNodeBool(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeBool(index);
}

int32_t NFCObject::GetNodeInt(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt(index);
}

int32_t NFCObject::GetNodeInt32(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt32(index);
}

uint32_t NFCObject::GetNodeUInt32(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeUInt32(index);
}

int64_t NFCObject::GetNodeInt64(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt64(index);
}

uint64_t NFCObject::GetNodeUInt64(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeUInt64(index);
}

float NFCObject::GetNodeFloat(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeFloat(index);
}

double NFCObject::GetNodeDouble(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeDouble(index);
}

const std::string& NFCObject::GetNodeString(uint32_t index) const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeString(index);
}

bool NFCObject::AddTableRow(const std::string& table_name)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableRow(table_name);
}

size_t NFCObject::GetTableRowCount(const std::string& table_name)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableRowCount(table_name);
}

int NFCObject::GetTableCurRow(const std::string& table_name)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableCurRow(table_name);
}

bool NFCObject::SetTableBool(const std::string& name, const int row, const int col, const bool value)
{
	NF_ASSERT(m_pTableManager!= nullptr);
	return m_pTableManager->SetTableBool(name, row, col, value);
}

bool NFCObject::SetTableInt(const std::string& name, const int row, const int col, const int32_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableInt(name, row, col, value);
}

bool NFCObject::SetTableInt32(const std::string& name, const int row, const int col, const int32_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableInt32(name, row, col, value);
}

bool NFCObject::SetTableUInt32(const std::string& name, const int row, const int col, const uint32_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableUInt32(name, row, col, value);
}

bool NFCObject::SetTableInt64(const std::string& name, const int row, const int col, const int64_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableInt64(name, row, col, value);
}

bool NFCObject::SetTableUInt64(const std::string& name, const int row, const int col, const uint64_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableUInt64(name, row, col, value);
}

bool NFCObject::SetTableFloat(const std::string& name, const int row, const int col, const float value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableFloat(name, row, col, value);
}

bool NFCObject::SetTableDouble(const std::string& name, const int row, const int col, const double value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableDouble(name, row, col, value);
}

bool NFCObject::SetTableString(const std::string& name, const int row, const int col, const std::string& value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableString(name, row, col, value);
}

bool NFCObject::GetTableBool(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableBool(name, row, col);
}

int32_t NFCObject::GetTableInt(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableInt(name, row, col);
}

int32_t NFCObject::GetTableInt32(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableInt32(name, row, col);
}

uint32_t NFCObject::GetTableUInt32(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableUInt32(name, row, col);
}

int64_t NFCObject::GetTableInt64(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableInt64(name, row, col);
}

uint64_t NFCObject::GetTableUInt64(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableUInt64(name, row, col);
}

float NFCObject::GetTableFloat(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableFloat(name, row, col);
}

double NFCObject::GetTableDouble(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableDouble(name, row, col);
}

const std::string& NFCObject::GetTableString(const std::string& name, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableString(name, row, col);
}

const NFCData::Array& NFCObject::GetTableArray(const std::string& name, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableArray(name, row, col);
}

const NFCData::List& NFCObject::GetTableList(const std::string& name, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableList(name, row, col);
}

const NFCData::MapStringData& NFCObject::GetTableMapStringData(const std::string& name, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableMapStringData(name, row, col);
}

const NFCData::MapIntData& NFCObject::GetTableMapIntData(const std::string& name, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableMapIntData(name, row, col);
}

NFCData::Array* NFCObject::MutableTableArray(const std::string& name, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableArray(name, row, col);
}

NFCData::List* NFCObject::MutableTableList(const std::string& name, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableList(name, row, col);
}

NFCData::MapStringData* NFCObject::MutableTableMapStringData(const std::string& name, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableMapStringData(name, row, col);
}

NFCData::MapIntData* NFCObject::MutableTableMapIntData(const std::string& name, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableMapIntData(name, row, col);
}

bool NFCObject::AddTableArrayItem(const std::string& name, size_t row, size_t col, const NFCData& data)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableArrayItem(name, row, col, data);
}

bool NFCObject::AddTableListItem(const std::string& name, size_t row, size_t col, const NFCData& data)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableListItem(name, row, col, data);
}

bool NFCObject::AddTableMapStringItem(const std::string& name, size_t row, size_t col, const std::string& key, const NFCData& value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableMapStringItem(name, row, col, key, value);
}

bool NFCObject::AddTableMapIntItem(const std::string& name, size_t row, size_t col, uint64_t key, const NFCData& value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableMapIntItem(name, row, col, key, value);
}

bool NFCObject::SetTableBool(uint32_t index, const int row, const int col, const bool value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableBool(index, row, col, value);
}

bool NFCObject::SetTableInt(uint32_t index, const int row, const int col, const int32_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableInt(index, row, col, value);
}

bool NFCObject::SetTableInt32(uint32_t index, const int row, const int col, const int32_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableInt32(index, row, col, value);
}

bool NFCObject::SetTableUInt32(uint32_t index, const int row, const int col, const uint32_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableUInt32(index, row, col, value);
}

bool NFCObject::SetTableInt64(uint32_t index, const int row, const int col, const int64_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableInt64(index, row, col, value);
}

bool NFCObject::SetTableUInt64(uint32_t index, const int row, const int col, const uint64_t value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableUInt64(index, row, col, value);
}

bool NFCObject::SetTableFloat(uint32_t index, const int row, const int col, const float value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableFloat(index, row, col, value);
}

bool NFCObject::SetTableDouble(uint32_t index, const int row, const int col, const double value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableDouble(index, row, col, value);
}

bool NFCObject::SetTableString(uint32_t index, const int row, const int col, const std::string& value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->SetTableString(index, row, col, value);
}

bool NFCObject::GetTableBool(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableBool(index, row, col);
}

int32_t NFCObject::GetTableInt(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableInt(index, row, col);
}

int32_t NFCObject::GetTableInt32(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableInt32(index, row, col);
}

uint32_t NFCObject::GetTableUInt32(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableUInt32(index, row, col);
}

int64_t NFCObject::GetTableInt64(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableInt64(index, row, col);
}

uint64_t NFCObject::GetTableUInt64(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableUInt64(index, row, col);
}

float NFCObject::GetTableFloat(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableFloat(index, row, col);
}

double NFCObject::GetTableDouble(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableDouble(index, row, col);
}

const std::string& NFCObject::GetTableString(uint32_t index, const int row, const int col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableString(index, row, col);
}

const NFCData::Array& NFCObject::GetTableArray(uint32_t index, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableArray(index, row, col);
}

const NFCData::List& NFCObject::GetTableList(uint32_t index, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableList(index, row, col);
}

const NFCData::MapStringData& NFCObject::GetTableMapStringData(uint32_t index, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableMapStringData(index, row, col);
}

const NFCData::MapIntData& NFCObject::GetTableMapIntData(uint32_t index, size_t row, size_t col) const
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->GetTableMapIntData(index, row, col);
}

NFCData::Array* NFCObject::MutableTableArray(uint32_t index, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableArray(index, row, col);
}

NFCData::List* NFCObject::MutableTableList(uint32_t index, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableList(index, row, col);
}

NFCData::MapStringData* NFCObject::MutableTableMapStringData(uint32_t index, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableMapStringData(index, row, col);
}

NFCData::MapIntData* NFCObject::MutableTableMapIntData(uint32_t index, size_t row, size_t col)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->MutableTableMapIntData(index, row, col);
}

bool NFCObject::AddTableArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableArrayItem(index, row, col, data);
}

bool NFCObject::AddTableListItem(uint32_t index, size_t row, size_t col, const NFCData& data)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableListItem(index, row, col, data);
}

bool NFCObject::AddTableMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableMapStringItem(index, row, col, key, value);
}

bool NFCObject::AddTableMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value)
{
	NF_ASSERT(m_pTableManager != nullptr);
	return m_pTableManager->AddTableMapIntItem(index, row, col, key, value);
}

NFIDataNodeManager* NFCObject::GetNodeManager()
{
	return m_pNodeManager;
}

NFIDataTableManager* NFCObject::GetTableManager()
{
	return m_pTableManager;
}

