// -------------------------------------------------------------------------
//    @FileName         :    AFCDataNodeManager.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/10
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFCDataNodeManager.h"
#include "NFCData.h"
#include "NFDataNode.h"
#include <NFComm/NFMath/NFMath.h>

NFCDataNodeManager::NFCDataNodeManager(const uint64_t objectId) : mObjectId(objectId)
{

}

NFCDataNodeManager::~NFCDataNodeManager()
{
	NFCDataNodeManager::Clear();
}

void NFCDataNodeManager::Clear()
{
	mNodes.clear();
    mIndices.clear();
}

uint64_t NFCDataNodeManager::GetObjectId() const
{
    return mObjectId;
}

bool NFCDataNodeManager::RegisterCallback(const std::string& name, const DATA_NODE_EVENT_FUNCTOR_PTR& cb)
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return false;
	}

	mCallBackList[index].push_back(cb);
	return true;
}

bool NFCDataNodeManager::RegisterCallback(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb)
{
	NF_ASSERT(index < mNodes.size());

	mCallBackList[index].push_back(cb);
	return true;
}

size_t NFCDataNodeManager::GetNodeCount()
{
    return mNodes.size();
}

NFDataNode* NFCDataNodeManager::GetNodeByIndex(size_t index)
{
    NF_ASSERT_RET_VAL(index >= 0 && index <= mNodes.size(), nullptr);

    return mNodes[index];
}

NFDataNode* NFCDataNodeManager::GetNode(const std::string& name)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return nullptr;
    }

    return mNodes[index];
}

bool NFCDataNodeManager::FindIndex(const std::string& name, size_t& index) const
{
	auto it = mIndices.find(name);
	if (it != mIndices.end())
	{
		index = it->second;
		return true;
	}

    return false;
}

bool NFCDataNodeManager::OnNodeCallback(const uint32_t index, const NFCData& oldData, const NFCData& newData)
{
	NF_ASSERT_RET_VAL(index < mNodes.size(), false);

	auto it = mCallBackList.find(index);
	if (it != mCallBackList.end())
	{
		std::vector<DATA_NODE_EVENT_FUNCTOR_PTR>& callBackList = it->second;
		for (size_t i = 0; i < callBackList.size(); ++i)
		{
			if (callBackList[i])
			{
				(*callBackList[i])(mObjectId, mNodes[i]->GetName(), oldData, newData);
			}
		}
	}

	return true;
}

bool NFCDataNodeManager::AddNode(const std::string& name, const NFCData& value, const int8_t feature)
{
    NFDataNode* pNode = new NFDataNode();
    pNode->mName = name;
    pNode->mValue = value;
    pNode->mFeature = feature;
    mIndices.emplace(name, mNodes.size());
    mNodes.push_back(pNode);
    return true;
}

bool NFCDataNodeManager::SetNode(const std::string& name, const NFCData& value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    switch (value.GetType())
    {
    case DT_BOOLEAN:
        return SetNodeBool(name, value.GetBool());
    case DT_INT:
        return SetNodeInt64(name, value.GetInt64());
    case DT_UINT64:
        return SetNodeInt64(name, value.GetInt64());
    case DT_FLOAT:
        return SetNodeFloat(name, value.GetFloat());
    case DT_DOUBLE:
        return SetNodeDouble(name, value.GetDouble());
    case DT_STRING:
        return SetNodeString(name, value.GetString());
    default:
        NF_ASSERT(0);
        break;
    }

    return false;
}

bool NFCDataNodeManager::SetNodeBool(const std::string& name, const bool value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    //old value
    NFCData oldData;
    bool oldValue = mNodes[index]->mValue.GetBool();
    oldData.SetBool(oldValue);

    mNodes[index]->mValue.SetBool(value);

    if(oldValue != value)
    {
        //DataNode callbacks
        OnNodeCallback(index, oldData, mNodes[index]->mValue);
    }

    return true;
}

bool NFCDataNodeManager::SetNodeInt(const std::string& name, const int32_t value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    //old value
	NFCData oldData;
    int32_t oldValue = mNodes[index]->mValue.GetInt();
    oldData.SetInt(oldValue);

    mNodes[index]->mValue.SetInt(value);

    if(oldValue != value)
    {
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);;
    }

    return true;
}

bool NFCDataNodeManager::SetNodeInt32(const std::string& name, const int32_t value)
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return false;
	}

	//old value
	NFCData oldData;
	int32_t oldValue = mNodes[index]->mValue.GetInt32();
	oldData.SetInt32(oldValue);

	mNodes[index]->mValue.SetInt32(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeUInt32(const std::string& name, const uint32_t value)
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return false;
	}

	//old value
	NFCData oldData;
	uint32_t oldValue = mNodes[index]->mValue.GetUInt32();
	oldData.SetUInt32(oldValue);

	mNodes[index]->mValue.SetUInt32(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeInt64(const std::string& name, const int64_t value)
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return false;
	}

	//old value
	NFCData oldData;
	int64_t oldValue = mNodes[index]->mValue.GetInt64();
	oldData.SetInt64(oldValue);

	mNodes[index]->mValue.SetInt64(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeUInt64(const std::string& name, const uint64_t value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    //old value
	NFCData oldData;
    uint64_t oldValue = mNodes[index]->mValue.GetUInt64();
    oldData.SetUInt64(oldValue);

    mNodes[index]->mValue.SetUInt64(value);

    if(oldValue != value)
    {
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
    }

    return true;
}

bool NFCDataNodeManager::SetNodeFloat(const std::string& name, const float value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    //old value
	NFCData oldData;
    float oldValue = mNodes[index]->mValue.GetFloat();
    oldData.SetFloat(oldValue);

    mNodes[index]->mValue.SetFloat(value);

    if(!almostEqual(oldValue, value))
    {
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
    }

    return true;
}

bool NFCDataNodeManager::SetNodeDouble(const std::string& name, const double value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    //old value
	NFCData oldData;
    double oldValue = mNodes[index]->mValue.GetDouble();
    oldData.SetDouble(oldValue);

    mNodes[index]->mValue.SetDouble(value);

    if(!almostEqual(oldValue, value))
    {
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
    }

    return true;
}

bool NFCDataNodeManager::SetNodeString(const std::string& name, const std::string& value)
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return false;
    }

    //old value
	NFCData oldData;
    std::string oldValue = mNodes[index]->mValue.GetString();
    oldData.SetString(oldValue);

    mNodes[index]->mValue.SetString(value);

    if(oldValue != value)
    {
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);;
    }

    return true;
}

bool NFCDataNodeManager::SetNodeBool(uint32_t index, const bool value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	bool oldValue = mNodes[index]->mValue.GetBool();
	oldData.SetBool(oldValue);

	mNodes[index]->mValue.SetBool(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeInt(uint32_t index, const int32_t value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	int32_t oldValue = mNodes[index]->mValue.GetInt32();
	oldData.SetInt32(oldValue);

	mNodes[index]->mValue.SetInt32(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeInt32(uint32_t index, const int32_t value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	int32_t oldValue = mNodes[index]->mValue.GetInt32();
	oldData.SetInt32(oldValue);

	mNodes[index]->mValue.SetInt32(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeUInt32(uint32_t index, const uint32_t value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	uint32_t oldValue = mNodes[index]->mValue.GetUInt32();
	oldData.SetUInt32(oldValue);

	mNodes[index]->mValue.SetUInt32(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeInt64(uint32_t index, const int64_t value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	int64_t oldValue = mNodes[index]->mValue.GetInt64();
	oldData.SetInt64(oldValue);

	mNodes[index]->mValue.SetInt64(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeUInt64(uint32_t index, const uint64_t value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	uint64_t oldValue = mNodes[index]->mValue.GetUInt64();
	oldData.SetUInt64(oldValue);

	mNodes[index]->mValue.SetUInt64(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeFloat(uint32_t index, const float value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	float oldValue = mNodes[index]->mValue.GetFloat();
	oldData.SetFloat(oldValue);

	mNodes[index]->mValue.SetFloat(value);

	if (!almostEqual(oldValue, value))
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeDouble(uint32_t index, const double value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	double oldValue = mNodes[index]->mValue.GetDouble();
	oldData.SetDouble(oldValue);

	mNodes[index]->mValue.SetDouble(value);

	if (!almostEqual(oldValue, value))
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::SetNodeString(uint32_t index, const std::string& value)
{
	NF_ASSERT(index < mNodes.size());

	//old value
	NFCData oldData;
	std::string oldValue = mNodes[index]->mValue.GetString();
	oldData.SetString(oldValue);

	mNodes[index]->mValue.SetString(value);

	if (oldValue != value)
	{
		//DataNode callbacks
		OnNodeCallback(index, oldData, mNodes[index]->mValue);
	}

	return true;
}

bool NFCDataNodeManager::GetNodeBool(const std::string& name) const
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return NFCDataStatics::empty_boolean;
    }

    return mNodes[index]->mValue.GetBool();
}

int32_t NFCDataNodeManager::GetNodeInt(const std::string& name) const
{
    size_t index;
    if(!FindIndex(name, index))
    {
        return static_cast<int32_t>(NFCDataStatics::empty_int);;
    }

    return mNodes[index]->mValue.GetInt();
}

int32_t NFCDataNodeManager::GetNodeInt32(const std::string& name) const
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return static_cast<int32_t>(NFCDataStatics::empty_int);;
	}

	return mNodes[index]->mValue.GetInt32();
}

uint32_t NFCDataNodeManager::GetNodeUInt32(const std::string& name) const
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return static_cast<uint32_t>(NFCDataStatics::empty_int);;
	}

	return mNodes[index]->mValue.GetUInt32();
}

int64_t NFCDataNodeManager::GetNodeInt64(const std::string& name) const
{
	size_t index;
	if (!FindIndex(name, index))
	{
		return NFCDataStatics::empty_int;;
	}

	return mNodes[index]->mValue.GetInt64();
}

uint64_t NFCDataNodeManager::GetNodeUInt64(const std::string& name) const
{
    size_t index;
    if(!FindIndex(name, index))
    {
		return NFCDataStatics::empty_uint64;
    }

    return mNodes[index]->mValue.GetUInt64();
}

float NFCDataNodeManager::GetNodeFloat(const std::string& name) const
{
    size_t index;
    if(!FindIndex(name, index))
    {
		return NFCDataStatics::empty_float;
    }

    return mNodes[index]->mValue.GetFloat();
}

double NFCDataNodeManager::GetNodeDouble(const std::string& name) const
{
    size_t index;
    if(!FindIndex(name, index))
    {
		return NFCDataStatics::empty_double;
    }

    return mNodes[index]->mValue.GetDouble();
}

const std::string& NFCDataNodeManager::GetNodeString(const std::string& name) const
{
    size_t index;
    if(!FindIndex(name, index))
    {
		return NFCDataStatics::empty_string;
    }

    return mNodes[index]->mValue.GetString();
}

bool NFCDataNodeManager::GetNodeBool(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetBool();
}

int32_t NFCDataNodeManager::GetNodeInt(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetInt();
}

int32_t NFCDataNodeManager::GetNodeInt32(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetInt32();
}

uint32_t NFCDataNodeManager::GetNodeUInt32(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetUInt32();
}

int64_t NFCDataNodeManager::GetNodeInt64(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetInt64();
}

uint64_t NFCDataNodeManager::GetNodeUInt64(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetUInt64();
}

float NFCDataNodeManager::GetNodeFloat(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetFloat();
}

double NFCDataNodeManager::GetNodeDouble(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetDouble();
}

const std::string& NFCDataNodeManager::GetNodeString(uint32_t index) const
{
	NF_ASSERT(index < mNodes.size());

	return mNodes[index]->mValue.GetString();
}
