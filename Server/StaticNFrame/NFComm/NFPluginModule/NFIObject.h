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
#include "NFCData.h"
#include "NFDefine.h"

class NFIDataNodeManager;
class NFIDataTableManager;

class NFIObject : public NFIModule
{
public:
	explicit NFIObject()
	{
	}

	virtual ~NFIObject() = default;

	virtual uint64_t GetObjectId() = 0;

	virtual int AddTableRow(const std::string& table_name) = 0;
	virtual size_t GetTableRowCount(const std::string& table_name) = 0;

	virtual bool CheckNodeExist(const std::string& name) = 0;
	virtual bool CheckNodeExist(uint32_t index) = 0;

	virtual const NFCData::Array& GetArray(const std::string& name) const = 0;
	virtual const NFCData::List& GetList(const std::string& name) const = 0;
	virtual const NFCData::MapStringData& GetMapStringData(const std::string& name) const = 0;
	virtual const NFCData::MapIntData& GetMapIntData(const std::string& name) const = 0;

	virtual NFCData::Array* MutableArray(const std::string& name) = 0;
	virtual NFCData::List* MutableList(const std::string& name) = 0;
	virtual NFCData::MapStringData* MutableMapStringData(const std::string& name) = 0;
	virtual NFCData::MapIntData* MutableMapIntData(const std::string& name) = 0;

	virtual bool AddArrayItem(const std::string& name, const NFCData& data) = 0;
	virtual bool AddListItem(const std::string& name, const NFCData& data) = 0;
	virtual bool AddMapStringItem(const std::string& name, const std::string& key, const NFCData& value) = 0;
	virtual bool AddMapIntItem(const std::string& name, uint64_t key, const NFCData& value) = 0;

	virtual const NFCData::Array& GetArray(uint32_t index) const = 0;
	virtual const NFCData::List& GetList(int32_t index) const = 0;
	virtual const NFCData::MapStringData& GetMapStringData(uint32_t index) const = 0;
	virtual const NFCData::MapIntData& GetMapIntData(uint32_t index) const = 0;

	virtual NFCData::Array* MutableArray(uint32_t index) = 0;
	virtual NFCData::List* MutableList(uint32_t index) = 0;
	virtual NFCData::MapStringData* MutableMapStringData(uint32_t index) = 0;
	virtual NFCData::MapIntData* MutableMapIntData(uint32_t index) = 0;

	virtual bool AddArrayItem(uint32_t index, const NFCData& data) = 0;
	virtual bool AddListItem(uint32_t index, const NFCData& data) = 0;
	virtual bool AddMapStringItem(uint32_t index, const std::string& key, const NFCData& value) = 0;
	virtual bool AddMapIntItem(uint32_t index, uint64_t key, const NFCData& value) = 0;


	virtual bool SetNodeBool(const std::string& name, const bool value) = 0;
	virtual bool SetNodeInt(const std::string& name, const int32_t value) = 0;
	virtual bool SetNodeInt32(const std::string& name, const int32_t value) = 0;
	virtual bool SetNodeUInt32(const std::string& name, const uint32_t value) = 0;
	virtual bool SetNodeInt64(const std::string& name, const int64_t value) = 0;
	virtual bool SetNodeUInt64(const std::string& name, const uint64_t value) = 0;
	virtual bool SetNodeFloat(const std::string& name, const float value) = 0;
	virtual bool SetNodeDouble(const std::string& name, const double value) = 0;
	virtual bool SetNodeString(const std::string& name, const std::string& value) = 0;
	virtual bool SetNodeString(const std::string& name, const char* value) = 0;

	virtual bool SetNodeBool(uint32_t index, const bool value) = 0;
	virtual bool SetNodeInt(uint32_t index, const int32_t value) = 0;
	virtual bool SetNodeInt32(uint32_t index, const int32_t value) = 0;
	virtual bool SetNodeUInt32(uint32_t index, const uint32_t value) = 0;
	virtual bool SetNodeInt64(uint32_t index, const int64_t value) = 0;
	virtual bool SetNodeUInt64(uint32_t index, const uint64_t value) = 0;
	virtual bool SetNodeFloat(uint32_t index, const float value) = 0;
	virtual bool SetNodeDouble(uint32_t index, const double value) = 0;
	virtual bool SetNodeString(uint32_t index, const std::string& value) = 0;
	virtual bool SetNodeString(uint32_t index, const char* value) = 0;

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

	virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value) = 0;
	virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableInt32(const std::string& name, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableUInt32(const std::string& name, const int row, const int col, const uint32_t value) = 0;
	virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value) = 0;
	virtual bool SetTableUInt64(const std::string& name, const int row, const int col, const uint64_t value) = 0;
	virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value) = 0;
	virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value) = 0;
	virtual bool SetTableString(const std::string& name, const int row, const int col, const std::string& value) = 0;

	virtual bool GetTableBool(const std::string& name, const int row, const int col) const = 0;
	virtual int32_t GetTableInt(const std::string& name, const int row, const int col) const = 0;
	virtual int32_t GetTableInt32(const std::string& name, const int row, const int col) const = 0;
	virtual uint32_t GetTableUInt32(const std::string& name, const int row, const int col) const = 0;
	virtual int64_t GetTableInt64(const std::string& name, const int row, const int col) const = 0;
	virtual uint64_t GetTableUInt64(const std::string& name, const int row, const int col) const = 0;
	virtual float GetTableFloat(const std::string& name, const int row, const int col) const = 0;
	virtual double GetTableDouble(const std::string& name, const int row, const int col) const = 0;
	virtual const std::string& GetTableString(const std::string& name, const int row, const int col) const = 0;

	virtual const NFCData::Array& GetTableArray(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::List& GetTableList(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::MapStringData& GetTableMapStringData(const std::string&, size_t row, size_t col) const = 0;
	virtual const NFCData::MapIntData& GetTableMapIntData(const std::string&, size_t row, size_t col) const = 0;

	virtual NFCData::Array* MutableTableArray(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::List* MutableTableList(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::MapStringData* MutableTableMapStringData(const std::string&, size_t row, size_t col) = 0;
	virtual NFCData::MapIntData* MutableTableMapIntData(const std::string&, size_t row, size_t col) = 0;

	virtual bool AddTableArrayItem(const std::string&, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableListItem(const std::string&, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableMapStringItem(const std::string&, size_t row, size_t col, const std::string& key, const NFCData& value) = 0;
	virtual bool AddTableMapIntItem(const std::string&, size_t row, size_t col, uint64_t key, const NFCData& value) = 0;

	virtual bool SetTableBool(uint32_t index, const int row, const int col, const bool value) = 0;
	virtual bool SetTableInt(uint32_t index, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableInt32(uint32_t index, const int row, const int col, const int32_t value) = 0;
	virtual bool SetTableUInt32(uint32_t index, const int row, const int col, const uint32_t value) = 0;
	virtual bool SetTableInt64(uint32_t index, const int row, const int col, const int64_t value) = 0;
	virtual bool SetTableUInt64(uint32_t index, const int row, const int col, const uint64_t value) = 0;
	virtual bool SetTableFloat(uint32_t index, const int row, const int col, const float value) = 0;
	virtual bool SetTableDouble(uint32_t index, const int row, const int col, const double value) = 0;
	virtual bool SetTableString(uint32_t index, const int row, const int col, const std::string& value) = 0;

	virtual bool GetTableBool(uint32_t index, const int row, const int col) const = 0;
	virtual int32_t GetTableInt(uint32_t index, const int row, const int col) const = 0;
	virtual int32_t GetTableInt32(uint32_t index, const int row, const int col) const = 0;
	virtual uint32_t GetTableUInt32(uint32_t index, const int row, const int col) const = 0;
	virtual int64_t GetTableInt64(uint32_t index, const int row, const int col) const = 0;
	virtual uint64_t GetTableUInt64(uint32_t index, const int row, const int col) const = 0;
	virtual float GetTableFloat(uint32_t index, const int row, const int col) const = 0;
	virtual double GetTableDouble(uint32_t index, const int row, const int col) const = 0;
	virtual const std::string& GetTableString(uint32_t index, const int row, const int col) const = 0;

	virtual const NFCData::Array& GetTableArray(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::List& GetTableList(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::MapStringData& GetTableMapStringData(uint32_t index, size_t row, size_t col) const = 0;
	virtual const NFCData::MapIntData& GetTableMapIntData(uint32_t index, size_t row, size_t col) const = 0;

	virtual NFCData::Array* MutableTableArray(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::List* MutableTableList(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::MapStringData* MutableTableMapStringData(uint32_t index, size_t row, size_t col) = 0;
	virtual NFCData::MapIntData* MutableTableMapIntData(uint32_t index, size_t row, size_t col) = 0;

	virtual bool AddTableArrayItem(uint32_t index, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableListItem(uint32_t index, size_t row, size_t col, const NFCData& data) = 0;
	virtual bool AddTableMapStringItem(uint32_t index, size_t row, size_t col, const std::string& key, const NFCData& value) = 0;
	virtual bool AddTableMapIntItem(uint32_t index, size_t row, size_t col, uint64_t key, const NFCData& value) = 0;

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

	template <typename BaseType>
	bool AddTableCallBack(const std::string& name, BaseType* pBase, int (BaseType::*handler)(const uint64_t, const DATA_TABLE_EVENT_DATA&, const NFCData&, const NFCData&))
	{
		DATA_TABLE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		return AddTableCallback(name, std::make_shared<DATA_TABLE_EVENT_FUNCTOR>(functor));
	}

	template <typename BaseType>
	bool AddTableCallBack(uint32_t index, BaseType* pBase, int (BaseType::*handler)(const uint64_t, const DATA_TABLE_EVENT_DATA&, const NFCData&, const NFCData&))
	{
		DATA_TABLE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		return AddTableCallback(index, std::make_shared<DATA_TABLE_EVENT_FUNCTOR>(functor));
	}

	virtual bool AddNodeCallBack(const std::string& col, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddNodeCallBack(uint32_t index, const DATA_NODE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddTableCallback(const std::string& table_name, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddTableCallback(uint32_t index, const DATA_TABLE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual NFIDataNodeManager* GetNodeManager() = 0;
	virtual NFIDataTableManager* GetTableManager() = 0;
};

