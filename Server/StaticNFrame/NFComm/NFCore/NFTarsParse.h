// -------------------------------------------------------------------------
//    @FileName         :    NFTarsParse.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <stack>
#include <sstream>
#include "NFPlatform.h"
#include "NFAutoPtr.h"
#include "NFTarsElement.h"
#include "NFCommon.h"

enum yytokentype {
	TARS_VOID = 258,
	TARS_STRUCT = 259,
	TARS_BOOL = 260,
	TARS_BYTE = 261,
	TARS_SHORT = 262,
	TARS_INT = 263,
	TARS_DOUBLE = 264,
	TARS_FLOAT = 265,
	TARS_LONG = 266,
	TARS_STRING = 267,
	TARS_VECTOR = 268,
	TARS_MAP = 269,
	TARS_NAMESPACE = 270,
	TARS_INTERFACE = 271,
	TARS_IDENTIFIER = 272,
	TARS_OUT = 273,
	TARS_OP = 274,
	TARS_KEY = 275,
	TARS_ROUTE_KEY = 276,
	TARS_REQUIRE = 277,
	TARS_OPTIONAL = 278,
	TARS_CONST_INTEGER = 279,
	TARS_CONST_FLOAT = 280,
	TARS_FALSE = 281,
	TARS_TRUE = 282,
	TARS_STRING_LITERAL = 283,
	TARS_SCOPE_DELIMITER = 284,
	TARS_CONST = 285,
	TARS_ENUM = 286,
	TARS_UNSIGNED = 287,
	BAD_CHAR = 288
};

#define TARS_VERSION 1

/**
* Tars文件解析类
*
*/
class TarsParse : public NF_HandleBase
{
public:
	/**
	* 构造函数
	*/
	TarsParse();

	/**
	* 是否支持tars开头的标识
	* @param bWithTars
	*/
	void setTars(bool bWithTars);

	/**
	* 头文件路径
	* @param sHeader
	*/
	void setHeader(const std::string &sHeader);

	/**
	* 是否支持优先使用当前tars文件
	* @param bWithTars
	*/
	void setCurrentPriority(bool bFlag);

	/**
	* 获取头文件路径
	*/
	std::string getHeader();

	/**
	* 解析某一个文件
	* @param sFileName
	*/
	void parse(const std::string &sFileName);

	/**
	* 错误提示
	* @param msg
	*/
	void error(const std::string &msg);

	/**
	* 检查关键字
	* @param s
	*
	* @return int
	*/
	int  checkKeyword(const std::string &s);

	/**
	* 下一行
	*/
	void nextLine();

	/**
	* 目前解析的文件名称
	*
	* @return string
	*/
	std::string getCurrFileName();

	/**
	* tab
	*
	* @return string
	*/
	std::string getTab();

	/**
	* 增加tab数
	*/
	void incTab() { _itab++; }

	/**
	* 减少tab数
	*/
	void delTab() { _itab--; }

	/**
	* 解析文件
	* @param file
	*/
	void pushFile(const std::string &file);

	/**
	* 弹出解析文件
	*/
	ContextPtr popFile();

	/**
	* 获取所有的上下文
	*
	* @return std::vector<ContextPtr>
	*/
	std::vector<ContextPtr> getContexts() { return _vcontexts; }

	/**
	* 获取目前的容器
	*
	* @return ContainerPtr
	*/
	ContainerPtr currentContainer();

	/**
	* push容器
	* @param c
	*/
	void pushContainer(const ContainerPtr &c);

	/**
	* 目前的上下文
	*
	* @return ContextPtr
	*/
	ContextPtr currentContextPtr();

	/**
	* 弹出容器
	*
	* @return ContainerPtr
	*/
	ContainerPtr popContainer();

	/**
	* 生成Builtin元素
	* @param kind
	*
	* @return BuiltinPtr
	*/
	BuiltinPtr createBuiltin(Builtin::Kind kind, bool isUnsigned = false);

	/**
	* 生成Vector元素
	* @param ptr
	*
	* @return VectorPtr
	*/
	VectorPtr createVector(const TypePtr &ptr);

	/**
	* 生成Map元素
	* @param pleft
	* @param pright
	*
	* @return MapPtr
	*/
	MapPtr createMap(const TypePtr &pleft, const TypePtr &pright);

	/**
	* 添加结构元素
	* @param sPtr
	*/
	void addStructPtr(const StructPtr &sPtr);

	/**
	* 查找结构
	* @param id
	*
	* @return StructPtr
	*/
	StructPtr findStruct(const string &sid);

	/**
	* 添加枚举元素
	* @param ePtr
	*/
	void addEnumPtr(const EnumPtr &ePtr);

	/**
	* 查找结构
	* @param id
	*
	* @return EnumPtr
	*/
	EnumPtr findEnum(const std::string &sid);

	/**
	* 检查冲突
	* @param id
	*/
	void checkConflict(const std::string &sid);

	/**
	* 查找自定义类型
	* @param sid
	*
	* @return TypePtr
	*/
	TypePtr findUserType(const std::string &sid);

	/**
	* 查找名字空间
	* @param id
	*
	* @return NamespacePtr
	*/
	NamespacePtr findNamespace(const std::string &id);

	/**
	* 目前的名字空间
	*
	* @return NamespacePtr
	*/
	NamespacePtr currentNamespace();

	/**
	* 检查tag的合法性
	* @param i
	*/
	void checkTag(int i);


	/**
	* 检查szie的合法性
	* @param i
	*/
	void checkSize(int i);

	/**
	* 检查array的合法性
	* @param i
	*/
	void checkArrayVaid(TypePtr &tPtr, int size);

	/**
	* 检查pointer的合法性
	* @param i
	*/
	void checkPointerVaid(TypePtr &tPtr);

	/**
	* 检查常量类型和值是否一致
	* @param c
	* @param b
	*/
	void checkConstValue(TypeIdPtr &tPtr, int b);

	/**
	* 获取文件名
	* @param s
	*
	* @return string
	*/
	bool getFilePath(const std::string &s, std::string &file);

	void setKeyStruct(const StructPtr& key)
	{
		_key = key;
	}

	StructPtr getKeyStruct()
	{
		return _key;
	}

	/**
	* 打印文件开头注释
	*/
	std::string printHeaderRemark();


	/**
	* 判断当前的标识符是否是枚举变量
	*/
	bool checkEnum(const std::string &idName);

	/**

	* 设置include的tars文件全部从当前文件搜寻
	*/
	void setUseCurrentPath(const bool & bEnable) { _bUseCurrentPath = bEnable; }

	/**
	* 查找tars文件时,设置include路径
	*/
	void addIncludePath(const std::string &include)
	{
		std::vector<std::string> v = NFCommon::sepstr<std::string>(include, "; ,", false);

		_vIncludePath.insert(_vIncludePath.end(), v.begin(), v.end());
	}

protected:
	/**
	* 添加名字空间
	* @param nPtr
	*/
	void addNamespacePtr(const NamespacePtr &nPtr);

	/**
	* 初始化
	*/
	void initScanner();

	/**
	* 清除
	*/
	void clear();

protected:
	bool                            _bWithTars;
	std::map<std::string, int>      _keywordMap;
	int                             _itab;
	StructPtr                       _key;
	std::stack<ContextPtr>          _contexts;
	std::stack<ContainerPtr>        _contains;
	std::vector<ContextPtr>         _vcontexts;
	std::vector<StructPtr>          _structs;
	std::vector<EnumPtr>            _enums;
	std::vector<NamespacePtr>       _namespaces;
	std::string                          _sHeader;
	bool                            _bUseCurrentPath;
	bool                            _bUseCurrentPathFirst;
	std::vector<string>             _vIncludePath;
};

extern int yyparse();
extern int yylex();
extern FILE *yyin, *yyout;

typedef NFAutoPtr<TarsParse> TarsParsePtr;

extern TarsParsePtr g_parse;
