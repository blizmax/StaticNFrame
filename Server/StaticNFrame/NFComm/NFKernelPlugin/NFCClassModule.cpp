// -------------------------------------------------------------------------
//    @FileName         :    AFCClassModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/17
//    @Email			:    445267987@qq.com
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------
#include "NFCClassModule.h"
#include <iostream>
#include <NFComm/NFPluginModule/NFIPluginManager.h>
#include <NFComm/NFCore/NFStringUtility.h>
#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>
#include <NFComm/NFPluginModule/NFBitValue.hpp>
#include <NFComm/NFPluginModule/NFDataNode.h>
#include <NFComm/NFPluginModule/NFDataTable.h>

NFCClass::NFCClass(const std::string& strClassName)
{
	m_pParentClass = nullptr;
	mstrClassName = strClassName;

	m_pNodeManager = NF_NEW NFCDataNodeManager(0);
	m_pTableManager = NF_NEW NFCDataTableManager(0);
}

NFCClass::~NFCClass()
{
	NF_SAFE_DELETE(m_pNodeManager);
	NF_SAFE_DELETE(m_pTableManager);
}

NFIDataNodeManager* NFCClass::GetNodeManager()
{
	return m_pNodeManager;
}

NFIDataTableManager* NFCClass::GetTableManager()
{
	return m_pTableManager;
}

bool NFCClass::AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	mxClassEventInfo.push_back(cb);
	return true;
}

bool NFCClass::DoEvent(const uint64_t objectID, const NF_OBJECT_EVENT eClassEvent, const NFCData& valueList)
{
	CLASS_EVENT_FUNCTOR_PTR cb;
	for(auto it = mxClassEventInfo.begin(); it != mxClassEventInfo.end(); ++it)
	{
		CLASS_EVENT_FUNCTOR_PTR cb = *it;
		(*cb)(objectID, mstrClassName, eClassEvent, valueList);
	}
	return true;
}

void NFCClass::SetParent(NFIClass* pClass)
{
	m_pParentClass = pClass;
}

NFIClass* NFCClass::GetParent()
{
	return m_pParentClass;
}

void NFCClass::SetTypeName(const char* strType)
{
	mstrType = strType;
}

const std::string& NFCClass::GetTypeName()
{
	return mstrType;
}

const std::string& NFCClass::GetClassName()
{
	return mstrClassName;
}

bool NFCClass::AddConfigName(std::string& strConfigName)
{
	mxConfigList.push_back(strConfigName);
	return true;
}

std::vector<std::string>& NFCClass::GetConfigNameList()
{
	return mxConfigList;
}

void NFCClass::SetInstancePath(const std::string& strPath)
{
	mstrClassInstancePath = strPath;
}

const std::string& NFCClass::GetInstancePath()
{
	return mstrClassInstancePath;
}

NFCClassModule::NFCClassModule(NFIPluginManager* p)
{
	pPluginManager = p;

	msConfigFileName = "DataConfig/Struct/LogicClass.xml";

	std::cout << "Using [" << pPluginManager->GetConfigPath() + msConfigFileName << "]" << std::endl;

	bool ret = Load();
	NF_ASSERT(ret);
}

NFCClassModule::~NFCClassModule()
{
}

bool NFCClassModule::Init()
{
	return true;
}

bool NFCClassModule::Shut()
{
    return true;
}

bool NFCClassModule::Load()
{
    std::string strFile = pPluginManager->GetConfigPath() + msConfigFileName;
    rapidxml::file<> fdoc(strFile.c_str());

    rapidxml::xml_document<> xDoc;
    xDoc.parse<0>(fdoc.data());
    //////////////////////////////////////////////////////////////////////////
    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = xDoc.first_node();
	rapidxml::xml_node<>* pClassRoot = root->first_node("Class");
	if (pClassRoot == nullptr) return false;

    for(rapidxml::xml_node<>* attrNode = pClassRoot->first_node(); attrNode; attrNode = attrNode->next_sibling())
    {
        if(!Load(attrNode))
        {
            return false;
        }
    }
    return true;
}

bool NFCClassModule::Load(rapidxml::xml_node<>* attrNode)
{
    const char* pstrLogicClassName = attrNode->first_attribute("Id")->value();
    const char* pstrType = attrNode->first_attribute("Type")->value();
    const char* pstrPath = attrNode->first_attribute("Path")->value();

    NFIClass* pClass = NF_NEW NFCClass(pstrLogicClassName);
    pClass->SetParent(nullptr);
    pClass->SetTypeName(pstrType);
	mClassMap.emplace(pstrLogicClassName, pClass);

    if(!AddClass(pstrPath, pClass))
    {
        return false;
    }

    return true;
}

bool NFCClassModule::Save()
{
	return true;
}

bool NFCClassModule::Clear()
{
	return true;
}

bool NFCClassModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	return true;
}

bool NFCClassModule::DoEvent(const uint64_t objectID, const std::string& strClassName, const NF_OBJECT_EVENT eClassEvent, const NFCData& valueList)
{
	return true;
}

NFIDataNodeManager* NFCClassModule::GetNodeManager(const std::string& strClassName)
{
	auto it = mClassMap.find(strClassName);
	if (it != mClassMap.end())
	{
		if (it->second)
		{
			return it->second->GetNodeManager();
		}
	}
	return nullptr;
}

NFIDataTableManager* NFCClassModule::GetTableManager(const std::string& strClassName)
{
	auto it = mClassMap.find(strClassName);
	if (it != mClassMap.end())
	{
		if (it->second)
		{
			return it->second->GetTableManager();
		}
	}
	return nullptr;
}

int NFCClassModule::ComputerType(const std::string& pstrTypeName, NFCData& var) const
{
	std::string lowerName = NFStringUtility::ToLower(pstrTypeName);
    if(lowerName == "bool")
    {
		var.SetDefaultValue(DT_BOOLEAN);
        return var.GetType();
    }
    else if(lowerName == "int" || lowerName == "int32" || lowerName == "uint32" || lowerName == "int64" || lowerName == "uint64")
    {
		var.SetDefaultValue(DT_INT);
        return var.GetType();
    }
    else if(lowerName == "float" || lowerName == "double")
    {
		var.SetDefaultValue(DT_DOUBLE);
        return var.GetType();
    }
    else if(lowerName == "string")
    {
		var.SetDefaultValue(DT_STRING);
        return var.GetType();
    }
	else if (lowerName == "array")
	{
		var.SetDefaultValue(DT_ARRAY);
		return var.GetType();
	}
	else if (lowerName == "list")
	{
		var.SetDefaultValue(DT_LIST);
		return var.GetType();
	}
	else if (lowerName == "mapstring")
	{
		var.SetDefaultValue(DT_MAPSTRING);
		return var.GetType();
	}
	else if (lowerName == "mapint")
	{
		var.SetDefaultValue(DT_MAPINT);
		return var.GetType();
	}

    return DT_UNKNOWN;
}

bool NFCClassModule::AddClass(const std::string& pstrClassFilePath, NFIClass* pClass)
{
    //////////////////////////////////////////////////////////////////////////
    if(AddClassInclude(pstrClassFilePath, pClass))
    {
		return true;
    }
	return false;
}

bool NFCClassModule::AddClassInclude(const std::string& pstrClassFilePath, NFIClass* pClass)
{
    //////////////////////////////////////////////////////////////////////////
    std::string strFile = pPluginManager->GetConfigPath() + pstrClassFilePath;
    rapidxml::file<> fdoc(strFile.c_str());

    rapidxml::xml_document<> xDoc;
    xDoc.parse<0>(fdoc.data());
    //////////////////////////////////////////////////////////////////////////

    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = xDoc.first_node();

    rapidxml::xml_node<>* pRootNodeDataNode = root->first_node("DataNodes");
    if(pRootNodeDataNode != nullptr)
    {
        if(!AddNodes(pRootNodeDataNode, pClass))
        {
            NF_ASSERT_MSG(0, "AddNodes failed");
            return false;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //Add table
    rapidxml::xml_node<>* pRootNodeDataTable = root->first_node("DataTables");
    if(pRootNodeDataTable != nullptr)
    {
        if(!AddTables(pRootNodeDataTable, pClass))
        {
            NF_ASSERT_MSG(0, "AddTables failed");
            return false;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //set table feature
    rapidxml::xml_node<>* pRootTableFeature = root->first_node("ListTable");
    if(pRootTableFeature != nullptr)
    {
        if(!AddTableFeature(pRootTableFeature, pClass))
        {
            NF_ASSERT_MSG(0, "AddTables failed");
            return false;
        }
    }

	return true;
}

bool NFCClassModule::AddNodes(rapidxml::xml_node<>* pNodeRootNode, NFIClass* pClass)
{
    for(rapidxml::xml_node<>* pNode = pNodeRootNode->first_node(); pNode != nullptr; pNode = pNode->next_sibling())
    {
        if(pNode == nullptr)
        {
            continue;
        }

        std::string strNodeName = pNode->first_attribute("Property")->value();
        if(pClass->GetNodeManager()->GetNode(strNodeName) != nullptr)
        {
            NF_ASSERT_MSG(0, "property name:" + strNodeName + " repeated!");
            continue;
        }

        std::string strType = pNode->first_attribute("Type")->value();
        bool bPublic = lexical_cast<bool>(pNode->first_attribute("Public")->value());
        bool bPrivate = lexical_cast<bool>(pNode->first_attribute("Private")->value());
        bool bSave = lexical_cast<bool>(pNode->first_attribute("Save")->value());
        bool bRealTime = lexical_cast<bool>(pNode->first_attribute("Cache")->value()); //TODO:real-time

        NFCData varNode;
        if(DT_UNKNOWN == ComputerType(strType, varNode))
        {
            NF_ASSERT_MSG(0, strNodeName);
        }

        int8_t feature(0);
        if(bPublic)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_PUBLIC);
        }

        if(bPrivate)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_PRIVATE);
        }

        if(bRealTime)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_REAL_TIME);
        }

        if(bSave)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_SAVE);
        }

        pClass->GetNodeManager()->AddNode(strNodeName, varNode, feature);
    }

    return true;
}

bool NFCClassModule::AddTables(rapidxml::xml_node<>* pTableRootNode, NFIClass* pClass)
{
    for(rapidxml::xml_node<>* pTableNode = pTableRootNode->first_node(); pTableNode != nullptr;  pTableNode = pTableNode->next_sibling())
    {
        std::string pTableName = pTableNode->first_attribute("TableName")->value();
        if(pClass->GetTableManager()->GetTable(pTableName) != nullptr)
        {
            NF_ASSERT_MSG(0, pTableName);
            continue;
        }

		NFCData dataArray;
		dataArray.SetDefaultValue(DT_ARRAY);
        for(rapidxml::xml_node<>* pTableColNode = pTableNode->first_node(); pTableColNode != nullptr;  pTableColNode = pTableColNode->next_sibling())
        {
			NFCData data;
            std::string strColType = pTableColNode->first_attribute("Type")->value();
            if(DT_UNKNOWN == ComputerType(strColType, data))
            {
                NF_ASSERT_MSG(0, pTableName);
            }
			dataArray.AddArrayItem(data);
        }

        bool result = pClass->GetTableManager()->AddTable(0, pTableName, dataArray, 0);
        NF_ASSERT_MSG(result, "add table failed, please check");
    }

    return true;
}

bool NFCClassModule::AddTableFeature(rapidxml::xml_node<>* pTableFeature, NFIClass* pClass)
{
    for(rapidxml::xml_node<>* pNode = pTableFeature->first_node(); pNode != nullptr; pNode = pNode->next_sibling())
    {
        if(pNode == nullptr)
        {
            continue;
        }

        std::string strTableName = pNode->first_attribute("TableName")->value();
		NFDataTable* pTable = pClass->GetTableManager()->GetTable(strTableName);
        if(pTable == nullptr)
        {
            NF_ASSERT_MSG(0, "table :" + strTableName + " not exist!");
            continue;
        }

        bool bPublic = lexical_cast<bool>(pNode->first_attribute("Public")->value());
        bool bPrivate = lexical_cast<bool>(pNode->first_attribute("Private")->value());
        bool bSave = lexical_cast<bool>(pNode->first_attribute("Save")->value());
        bool bRealTime = lexical_cast<bool>(pNode->first_attribute("Cache")->value()); //TODO:real-time

        int8_t feature(0);
        if(bPublic)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_PUBLIC);
        }

        if(bPrivate)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_PRIVATE);
        }

        if(bRealTime)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_REAL_TIME);
        }

        if(bSave)
        {
            NFBitValue<int8_t>::SetBitValue(feature, NFDataNode::PF_SAVE);
        }

		pTable->SetFeature(feature);
    }
	return true;
}
