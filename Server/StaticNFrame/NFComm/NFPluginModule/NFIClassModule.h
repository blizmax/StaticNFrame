// -------------------------------------------------------------------------
//    @FileName         :    AFIClassModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/17
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFIDataNodeManager.h"
#include "NFIDataTableManager.h"

#include "NFComm/NFCore/NFPlatform.h"
#include "NFIModule.h"

class NFIClass
{
public:
	virtual ~NFIClass()
	{
	}

	virtual NFIDataNodeManager* GetNodeManager() = 0;
	virtual NFIDataTableManager* GetTableManager() = 0;

	virtual void SetParent(NFIClass* pClass) = 0;
	virtual NFIClass* GetParent() = 0;

	virtual void SetTypeName(const char* strType) = 0;
	virtual const std::string& GetTypeName() = 0;
	virtual const std::string& GetClassName() = 0;

	virtual bool AddConfigName(std::string& strConfigName) = 0;
	virtual std::vector<std::string>& GetConfigNameList() = 0;

	virtual void SetInstancePath(const std::string& strPath) = 0;
	virtual const std::string& GetInstancePath() = 0;

	virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool DoEvent(const uint64_t objectID, const NF_OBJECT_EVENT eClassEvent, const NFCData& valueList) = 0;
};

class NFIClassModule : public NFIModule
{
public:
	virtual ~NFIClassModule()
	{
	}

	virtual bool Load() = 0;
	virtual bool Save() = 0;
	virtual bool Clear() = 0;

	template <typename BaseType>
	bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const uint64_t, const std::string&, const NF_OBJECT_EVENT, const NFCData&))
	{
		CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
		return AddClassCallBack(strClassName, functorPtr);
	}

	virtual bool DoEvent(const uint64_t objectID, const std::string& strClassName, const NF_OBJECT_EVENT eClassEvent, const NFCData& valueList) = 0;

	virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual NFIDataNodeManager* GetNodeManager(const std::string& strClassName) = 0;
	virtual NFIDataTableManager* GetTableManager(const std::string& strClassName) = 0;
};

