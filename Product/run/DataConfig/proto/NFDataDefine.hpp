#pragma once

#include<string>
#include<vector>
#include<NFComm/NFCore/NFPlatform.h>

namespace NFrame {

class IObject
{
public:
	//Class name
	static const std::string& ThisName(){ static std::string xIObject = "IObject"; return xIObject; }
	//DataTables

	enum HateTable
	{
		HateTable_ConfigID,		 //ConfigID -> int
		HateTable_ClassName,		 //ClassName -> string
		HateTable_SceneID,		 //SceneID -> int
		HateTable_GroupID,		 //GroupID -> int

	};

	enum EnumIObjectNode
	{
		Enum_Node_ConfigID,		//int
		Enum_Node_ClassName,		//string
		Enum_Node_SceneID,		//int
		Enum_Node_GroupID,		//int
		Enum_Max_Node,
	};

	static const std::string& GetIObjectNodeName(EnumIObjectNode index)
	{
		static const std::vector<std::string> IObjectNodeName =
		{
			"ConfigID",		//int
			"ClassName",		//string
			"SceneID",		//int
			"GroupID",		//int
		};
		NF_ASSERT(index < IObjectNodeName.size());
		return IObjectNodeName[index];
	}

	enum EnumIObjectTable
	{
		Enum_Table_HateTable,		
		Enum_Max_Table,
	};

	static const std::string& GetIObjectTableName(EnumIObjectTable index)
	{
		static const std::vector<std::string> IObjectTableName = 
		{
			"HateTable",		
		};
		NF_ASSERT(index < IObjectTableName.size());
		return IObjectTableName[index];
	}

};

}
