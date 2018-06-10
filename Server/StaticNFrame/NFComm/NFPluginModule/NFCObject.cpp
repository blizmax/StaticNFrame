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

bool NFCObject::GetNodeBool(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
    return m_pNodeManager->GetNodeBool(name);
}

int32_t NFCObject::GetNodeInt(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
    return m_pNodeManager->GetNodeInt(name);
}

int32_t NFCObject::GetNodeInt32(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt32(name);
}

uint32_t NFCObject::GetNodeUInt32(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeUInt32(name);
}

int64_t NFCObject::GetNodeInt64(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeInt64(name);
}

uint64_t NFCObject::GetNodeUInt64(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
	return m_pNodeManager->GetNodeUInt64(name);
}

float NFCObject::GetNodeFloat(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
    return m_pNodeManager->GetNodeFloat(name);
}

double NFCObject::GetNodeDouble(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
    return m_pNodeManager->GetNodeDouble(name);
}

const std::string& NFCObject::GetNodeString(const std::string& name)
{
	NF_ASSERT(m_pNodeManager != nullptr);
    return m_pNodeManager->GetNodeString(name);
}

NFIDataNodeManager* NFCObject::GetNodeManager()
{
    return m_pNodeManager;
}