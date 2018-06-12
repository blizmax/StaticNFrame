// -------------------------------------------------------------------------
//    @FileName         :    NFCObject.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/10
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFDefine.h"
#include "NFIObject.h"
#include "NFEventObj.h"

class NFIPluginManager;
class NFIDataNodeManager;

class NFCObject : public NFIObject
{
public:
	NFCObject() = delete;

	explicit NFCObject(const uint64_t mObjectId, NFIPluginManager* pluginManager);
	virtual ~NFCObject();

	virtual bool Init() override;
	virtual bool Shut() override;
	virtual bool Execute() override;
	virtual uint64_t GetObjectId() override;

	virtual bool CheckNodeExist(const std::string& name) override;
	virtual bool CheckNodeExist(uint32_t index) override;

	virtual bool AddNode(const std::string& name, const NFCData& value, const int8_t feature);
	virtual bool SetNode(const std::string& name, const NFCData& value);

	virtual bool SetNodeBool(const std::string& name, const bool value) override;
	virtual bool SetNodeInt(const std::string& name, const int32_t value) override;
	virtual bool SetNodeInt32(const std::string& name, const int32_t value) override;
	virtual bool SetNodeUInt32(const std::string& name, const uint32_t value) override;
	virtual bool SetNodeInt64(const std::string& name, const int64_t value) override;
	virtual bool SetNodeUInt64(const std::string& name, const uint64_t value) override;
	virtual bool SetNodeFloat(const std::string& name, const float value) override;
	virtual bool SetNodeDouble(const std::string& name, const double value) override;
	virtual bool SetNodeString(const std::string& name, const std::string& value) override;

	virtual bool SetNodeBool(uint32_t index, const bool value) override;
	virtual bool SetNodeInt(uint32_t index, const int32_t value) override;
	virtual bool SetNodeInt32(uint32_t index, const int32_t value) override;
	virtual bool SetNodeUInt32(uint32_t index, const uint32_t value) override;
	virtual bool SetNodeInt64(uint32_t index, const int64_t value) override;
	virtual bool SetNodeUInt64(uint32_t index, const uint64_t value) override;
	virtual bool SetNodeFloat(uint32_t index, const float value) override;
	virtual bool SetNodeDouble(uint32_t index, const double value) override;
	virtual bool SetNodeString(uint32_t index, const std::string& value) override;

	virtual bool GetNodeBool(const std::string& name) const override;
	virtual int32_t GetNodeInt(const std::string& name) const override;
	virtual int32_t GetNodeInt32(const std::string& name) const override;
	virtual uint32_t GetNodeUInt32(const std::string& name) const override;
	virtual int64_t GetNodeInt64(const std::string& name) const override;
	virtual uint64_t GetNodeUInt64(const std::string& name) const override;
	virtual float GetNodeFloat(const std::string& name) const override;
	virtual double GetNodeDouble(const std::string& name) const override;
	virtual const std::string& GetNodeString(const std::string& name) const override;

	virtual bool GetNodeBool(uint32_t index) const override;
	virtual int32_t GetNodeInt(uint32_t index) const override;
	virtual int32_t GetNodeInt32(uint32_t indexe) const override;
	virtual uint32_t GetNodeUInt32(uint32_t index) const override;
	virtual int64_t GetNodeInt64(uint32_t index) const override;
	virtual uint64_t GetNodeUInt64(uint32_t index) const override;
	virtual float GetNodeFloat(uint32_t index) const override;
	virtual double GetNodeDouble(uint32_t index) const override;
	virtual const std::string& GetNodeString(uint32_t index) const override;

	virtual NFIDataNodeManager* GetNodeManager();
	virtual bool AddNodeCallBack(const std::string& col, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) override;
	virtual bool AddNodeCallBack(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) override;
private:
	uint64_t mObjectId;
	NFIDataNodeManager* m_pNodeManager;
};

