// -------------------------------------------------------------------------
//    @FileName         :    NFIObject.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFIModule.h"

class NFIObject : public NFIModule
{
public:
	explicit NFIObject()
	{
	}

	virtual ~NFIObject() = default;

	virtual uint64_t GetObjectId() = 0;

	virtual bool CheckNodeExist(const std::string& name) = 0;

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

	template <typename BaseType>
	bool AddNodeCallBack(const std::string& name, BaseType* pBase, int (BaseType::*handler)(const uint64_t, const std::string&, const NFCData&, const NFCData&))
	{
		DATA_NODE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		return AddNodeCallBack(name, std::make_shared<DATA_NODE_EVENT_FUNCTOR>(functor));
	}

	template <typename BaseType>
	bool AddNodeCallBack(uint32_t index, BaseType* pBase, int (BaseType::*handler)(const uint64_t, const std::string&, const NFCData&, const NFCData&))
	{
		DATA_NODE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		return AddNodeCallBack(index, std::make_shared<DATA_NODE_EVENT_FUNCTOR>(functor));
	}

	virtual bool AddNodeCallBack(const std::string& col, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddNodeCallBack(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) = 0;
};

