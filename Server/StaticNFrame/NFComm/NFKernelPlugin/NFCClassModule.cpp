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
	return nullptr;
}

NFIDataTableManager* NFCClassModule::GetTableManager(const std::string& strClassName)
{
	return nullptr;
}

bool NFCClassModule::AddClass(const std::string& strClassName, const std::string& strParentName)
{
	return true;
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
