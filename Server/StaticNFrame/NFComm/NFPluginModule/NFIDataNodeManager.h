// -------------------------------------------------------------------------
//    @FileName         :    NFIDataNodeManager.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/10
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once
#include <string>
#include "NFDefine.h"

class NFDataNode;
class NFCData;

class NFIDataNodeManager
{
public:
	virtual ~NFIDataNodeManager() = default;
	virtual void Clear() = 0;

	virtual bool RegisterCallback(const std::string& name, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool RegisterCallback(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual size_t GetNodeCount() = 0;
	virtual NFDataNode* GetNodeByIndex(size_t index) = 0;
	virtual NFDataNode* GetNode(const std::string& name) = 0;
	virtual bool AddNode(const std::string& name, const NFCData& value, const int8_t feature) = 0;
	virtual bool SetNode(const std::string& name, const NFCData& value) = 0;

	virtual bool SetNodeBool(const std::string& name, const bool value) = 0;
	virtual bool SetNodeInt(const std::string& name, const int32_t value) = 0;
	virtual bool SetNodeInt32(const std::string& name, const int32_t value) = 0;
	virtual bool SetNodeUInt32(const std::string& name, const uint32_t value) = 0;
	virtual bool SetNodeInt64(const std::string& name, const int64_t value) = 0;
	virtual bool SetNodeUInt64(const std::string& name, const uint64_t value) = 0;
	virtual bool SetNodeFloat(const std::string& name, const float value) = 0;
	virtual bool SetNodeDouble(const std::string& name, const double value) = 0;
	virtual bool SetNodeString(const std::string& name, const std::string& value) = 0;

	virtual bool SetNodeBool(uint32_t index, const bool value) = 0;
	virtual bool SetNodeInt(uint32_t index, const int32_t value) = 0;
	virtual bool SetNodeInt32(uint32_t index, const int32_t value) = 0;
	virtual bool SetNodeUInt32(uint32_t index, const uint32_t value) = 0;
	virtual bool SetNodeInt64(uint32_t index, const int64_t value) = 0;
	virtual bool SetNodeUInt64(uint32_t index, const uint64_t value) = 0;
	virtual bool SetNodeFloat(uint32_t index, const float value) = 0;
	virtual bool SetNodeDouble(uint32_t index, const double value) = 0;
	virtual bool SetNodeString(uint32_t index, const std::string& value) = 0;

	virtual bool GetNodeBool(const std::string& name) const = 0;
	virtual int32_t GetNodeInt(const std::string& name) const = 0;
	virtual int32_t GetNodeInt32(const std::string& name) const = 0;
	virtual uint32_t GetNodeUInt32(const std::string& name) const = 0;
	virtual int64_t GetNodeInt64(const std::string& name) const = 0;
	virtual uint64_t GetNodeUInt64(const std::string& name) const = 0;
	virtual float GetNodeFloat(const std::string& name) const = 0;
	virtual double GetNodeDouble(const std::string& name) const = 0;
	virtual const std::string& GetNodeString(const std::string& name) const = 0;

	virtual bool GetNodeBool(uint32_t index) const = 0;
	virtual int32_t GetNodeInt(uint32_t index) const = 0;
	virtual int32_t GetNodeInt32(uint32_t indexe) const = 0;
	virtual uint32_t GetNodeUInt32(uint32_t index) const = 0;
	virtual int64_t GetNodeInt64(uint32_t index) const = 0;
	virtual uint64_t GetNodeUInt64(uint32_t index) const = 0;
	virtual float GetNodeFloat(uint32_t index) const = 0;
	virtual double GetNodeDouble(uint32_t index) const = 0;
	virtual const std::string& GetNodeString(uint32_t index) const = 0;
};

