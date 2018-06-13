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

NFCObject::NFCObject(const uint64_t objectId, NFIPluginManager* pLuginManager)
	: mObjectId(objectId)
{
	pPluginManager = pLuginManager;

	m_pNodeManager = NF_NEW NFCDataNodeManager(objectId);
}

NFCObject::~NFCObject()
{
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

bool NFCObject::AddNode(const std::string& name, const NFCData& value, const int8_t feature)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->AddNode(name, value, feature);
}

bool NFCObject::SetNode(const std::string& name, const NFCData& value)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->SetNode(name, value);
}

size_t NFCObject::GetNodeCount() const
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeCount();
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

NFIDataNodeManager* NFCObject::GetNodeManager()
{
	return m_pNodeManager;
}

