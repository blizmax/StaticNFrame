// -------------------------------------------------------------------------
//    @FileName         :    AFCEntity.h
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

class NFCObject : public NFIObject, public NFEventObj
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
    virtual bool CheckNodeExist(uint32_t index);

    virtual bool SetNodeBool(const std::string& name, const bool value) override;
	virtual bool SetNodeInt(const std::string& name, const int32_t value) override;
	virtual bool SetNodeInt32(const std::string& name, const int32_t value) override;
	virtual bool SetNodeUInt32(const std::string& name, const uint32_t value) override;
	virtual bool SetNodeInt64(const std::string& name, const int64_t value) override;
	virtual bool SetNodeUInt64(const std::string& name, const uint64_t value) override;
    virtual bool SetNodeFloat(const std::string& name, const float value) override;
    virtual bool SetNodeDouble(const std::string& name, const double value) override;
    virtual bool SetNodeString(const std::string& name, const std::string& value) override;

    virtual bool GetNodeBool(const std::string& name) override;
	virtual int32_t GetNodeInt(const std::string& name) override;
	virtual int32_t GetNodeInt32(const std::string& name) override;
	virtual uint32_t GetNodeUInt32(const std::string& name) override;
	virtual int64_t GetNodeInt64(const std::string& name) override;
	virtual uint64_t GetNodeUInt64(const std::string& name) override;
    virtual float GetNodeFloat(const std::string& name) override;
    virtual double GetNodeDouble(const std::string& name) override;
    virtual const std::string& GetNodeString(const std::string& name) override;
    virtual NFIDataNodeManager* GetNodeManager();
	virtual bool AddNodeCallBack(const std::string& col, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) override;
	virtual bool AddNodeCallBack(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) override;
private:
    uint64_t mObjectId;
	NFIDataNodeManager* m_pNodeManager;
};
