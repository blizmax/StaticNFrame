// -------------------------------------------------------------------------
//    @FileName         :    AFCClassModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/17
//    @Email			:    445267987@qq.com
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------
#pragma once
#include <string>
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include <NFComm/NFPluginModule/NFCDataTableManager.h>
#include <NFComm/NFPluginModule/NFCDataNodeManager.h>
#include <RapidXML/rapidxml.hpp>


class NFCClass : public NFIClass
{
public:
	explicit NFCClass(const std::string& strClassName);

	virtual ~NFCClass();

	virtual NFIDataNodeManager* GetNodeManager() override;

	virtual NFIDataTableManager* GetTableManager() override;

	virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb) override;

	virtual bool DoEvent(const uint64_t objectID, const NF_OBJECT_EVENT eClassEvent, const NFCData& valueList) override;

	void SetParent(NFIClass* pClass) override;

	virtual NFIClass* GetParent() override;

	virtual void SetTypeName(const char* strType) override;

	virtual const std::string& GetTypeName() override;

	virtual const std::string& GetClassName() override;

	virtual bool AddConfigName(std::string& strConfigName) override;

	virtual std::vector<std::string>& GetConfigNameList() override;

	virtual void SetInstancePath(const std::string& strPath) override;

	virtual const std::string& GetInstancePath() override;

private:
    NFIDataNodeManager* m_pNodeManager;
    NFIDataTableManager* m_pTableManager;

    NFIClass* m_pParentClass;
    std::string mstrType;
    std::string mstrClassName;
    std::string mstrClassInstancePath;

    std::vector<std::string> mxConfigList;

    std::vector<CLASS_EVENT_FUNCTOR_PTR> mxClassEventInfo;

	std::vector<std::string> mNames;
};

class NFCClassModule
    : public NFIClassModule
{
public:
    explicit NFCClassModule(NFIPluginManager* p);
    virtual ~NFCClassModule();

    virtual bool Init() override;
    virtual bool Shut() override;

    virtual bool Load() override;
    virtual bool Save() override;
    virtual bool Clear() override;

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) override;
    virtual bool DoEvent(const uint64_t objectID, const std::string& strClassName, const NF_OBJECT_EVENT eClassEvent, const NFCData& valueList) override;

    virtual NFIDataNodeManager* GetNodeManager(const std::string& strClassName) override;
    virtual NFIDataTableManager* GetTableManager(const std::string& strClassName) override;

protected:
	virtual int ComputerType(const std::string& pstrTypeName, NFCData& var) const;
	virtual bool AddClass(const std::string& pstrClassFilePath, NFIClass* pClass);
	virtual bool AddClassInclude(const std::string& pstrClassFilePath, NFIClass* pClass);
	virtual bool AddNodes(rapidxml::xml_node<>* pNodeRootNode, NFIClass* pClass);
	virtual bool AddTables(rapidxml::xml_node<>* pTableRootNode, NFIClass* pClass);
	virtual bool AddTableFeature(rapidxml::xml_node<>* pTableFeature, NFIClass* pClass);

	virtual bool Load(rapidxml::xml_node<>* attrNode);
private:
    std::string msConfigFileName;
	std::unordered_map<std::string, NFIClass*> mClassMap;
};