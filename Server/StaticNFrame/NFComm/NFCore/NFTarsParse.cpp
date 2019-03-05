// -------------------------------------------------------------------------
//    @FileName         :    NFTarsParse.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFTarsParse.h"
#include <errno.h>
#include <fstream>
#include <string.h>
#include "NFCommon.h"
#include "NFFileUtility.h"

TarsParsePtr g_parse = new TarsParse();

void yyerror(char const *msg)
{
	g_parse->error(msg);
}

int yyparse()
{
	return 0;
}

int yylex()
{
	return 0;
}

TarsParse::TarsParse()
{
	_bWithTars = false;
	_bUseCurrentPath = false;
	_itab = 0;
	_bUseCurrentPathFirst = false;
	initScanner();
}

void TarsParse::setTars(bool bWithTars)
{
	_bWithTars = bWithTars;
}

void TarsParse::setHeader(const std::string &sHeader)
{
	_sHeader = sHeader;
}

void TarsParse::setCurrentPriority(bool bFlag)
{
	_bUseCurrentPathFirst = bFlag;
}

std::string TarsParse::getHeader()
{
	if (_sHeader.empty())
		return _sHeader;
	return _sHeader + "/";
}

void TarsParse::clear()
{
	while (!_contexts.empty()) _contexts.pop();
	while (!_contains.empty()) _contains.pop();
	_vcontexts.clear();
	_structs.clear();
	_enums.clear();
	_namespaces.clear();
}

void TarsParse::parse(const std::string& sFileName)
{
	if (_bUseCurrentPath)
	{
		std::string sTemp = sFileName;
		bool isExist = NFFileUtility::IsFileExist(sFileName);

		if (!isExist)
		{
			sTemp = NFFileUtility::GetFileName(sFileName);
		}

		clear();

		_contains.push(new Container(""));
		//if (!(yyin = fopen(sTemp.c_str(), "r")))
		{
			error("open file '" + sFileName + "(" + sTemp + ")" + "' error :" + string(strerror(errno)));
		}

		pushFile(sFileName);

		yyparse();

		return;
	}

	if (_bUseCurrentPathFirst)
	{
		std::string sTemp = NFFileUtility::GetFileName(sFileName);//tars::NFFile::extractFileName(sFileName);
		bool isExist = NFFileUtility::IsFileExist(sTemp);

		if (!isExist)
		{
			sTemp = sFileName;
		}

		clear();

		_contains.push(new Container(""));
		//if (!(yyin = fopen(sTemp.c_str(), "r")))
		{
			error("open file '" + sFileName + "(" + sTemp + ")" + "' error :" + string(strerror(errno)));
		}

		pushFile(sFileName);

		yyparse();

		return;
	}

	{
		clear();

		_contains.push(new Container(""));
		//if (!(yyin = fopen(sFileName.c_str(), "r")))
		{
			error("open file '" + sFileName + "' error :" + string(strerror(errno)));
		}

		pushFile(sFileName);

		yyparse();
	}
}

void TarsParse::pushFile(const string &file)
{
	ContextPtr c = new Context(file);
	_contexts.push(c);
	_vcontexts.push_back(c);
}

ContextPtr TarsParse::popFile()
{
	ContextPtr c = _contexts.top();
	_contexts.pop();
	return c;
}

bool TarsParse::getFilePath(const string &s, string &file)
{
	if (_bUseCurrentPath)
	{
		if (s.length() < 2 || s[0] != '\"' || s[s.length() - 1] != '\"')
		{
			error("#include need \"FILENAME\"");
		}

		file = s.substr(1, s.length() - 2);

		std::string sTemp = file;
		bool isExist = NFFileUtility::IsFileExist(file);

		if (!isExist)
		{
			file = NFFileUtility::GetFileName(file);// tars::NFFile::extractFileName(file);
		}

		for (size_t i = 0; i < _vcontexts.size(); i++)
		{
			if (_vcontexts[i]->getFileName() == file)
			{
				return false;
			}
		}

		return true;
	}

	if (_bUseCurrentPathFirst)
	{
		if (s.length() < 2 || s[0] != '\"' || s[s.length() - 1] != '\"')
		{
			error("#include need \"FILENAME\"");
		}

		file = s.substr(1, s.length() - 2);

		std::string sTemp = NFFileUtility::GetFileName(file);//tars::NFFile::extractFileName(file);
		bool isExist = NFFileUtility::IsFileExist(sTemp);//tars::NFFile::isFileExist(sTemp);

		if (isExist)
		{
			file = sTemp;
		}

		for (size_t i = 0; i < _vcontexts.size(); i++)
		{
			if (_vcontexts[i]->getFileName() == file)
			{
				return false;
			}
		}

		return true;
	}

	if (s.length() < 2 || s[0] != '\"' || s[s.length() - 1] != '\"')
	{
		error("#include need \"FILENAME\"");
	}

	file = s.substr(1, s.length() - 2);

	if (!NFFileUtility::IsFileExist(file))
	{
		for (size_t i = 0; i < _vIncludePath.size(); i++)
		{
			if (NFFileUtility::IsFileExist(_vIncludePath[i] + "/" + file))
			{
				file = _vIncludePath[i] + "/" + file;
				break;
			}
		}
	}

	for (size_t i = 0; i < _vcontexts.size(); i++)
	{
		if (_vcontexts[i]->getFileName() == file)
		{
			return false;
		}
	}

	return true;
}

string TarsParse::getCurrFileName()
{
	return _contexts.top()->getFileName();
}

void TarsParse::nextLine()
{
	_contexts.top()->nextLine();
}

ContextPtr TarsParse::currentContextPtr()
{
	return _contexts.top();
}

void TarsParse::error(const string &msg)
{
	cerr << _contexts.top()->getFileName() << ": " << _contexts.top()->getCurrLine() << ": error: " << msg << endl;
	exit(-1);
}

int TarsParse::checkKeyword(const string& s)
{
	std::map<std::string, int>::const_iterator it = _keywordMap.find(s);
	if (it != _keywordMap.end())
	{
		return it->second;
	}

	if (!_bWithTars)
	{
		string sPrefix = "tars";
		//������tars��ͷ
		if ((s.length() >= sPrefix.length()) && (s.substr(0, sPrefix.length()) == sPrefix))
		{
			error("identifier can't start with 'tars'");
		}
	}

	return TARS_IDENTIFIER;
}

void TarsParse::initScanner()
{
	_keywordMap["void"] = TARS_VOID;
	_keywordMap["struct"] = TARS_STRUCT;
	_keywordMap["bool"] = TARS_BOOL;
	_keywordMap["byte"] = TARS_BYTE;
	_keywordMap["short"] = TARS_SHORT;
	_keywordMap["int"] = TARS_INT;
	_keywordMap["double"] = TARS_DOUBLE;
	_keywordMap["float"] = TARS_FLOAT;
	_keywordMap["long"] = TARS_LONG;
	_keywordMap["string"] = TARS_STRING;
	_keywordMap["vector"] = TARS_VECTOR;
	_keywordMap["map"] = TARS_MAP;
	_keywordMap["key"] = TARS_KEY;
	_keywordMap["routekey"] = TARS_ROUTE_KEY;
	_keywordMap["module"] = TARS_NAMESPACE;
	_keywordMap["interface"] = TARS_INTERFACE;
	_keywordMap["out"] = TARS_OUT;
	_keywordMap["require"] = TARS_REQUIRE;
	_keywordMap["optional"] = TARS_OPTIONAL;
	_keywordMap["false"] = TARS_FALSE;
	_keywordMap["true"] = TARS_TRUE;
	_keywordMap["enum"] = TARS_ENUM;
	_keywordMap["const"] = TARS_CONST;
	_keywordMap["unsigned"] = TARS_UNSIGNED;
}

string TarsParse::getTab()
{
	ostringstream s;
	for (int i = 0; i < _itab; i++)
	{
		s << "    ";
	}

	return s.str();
}

BuiltinPtr TarsParse::createBuiltin(Builtin::Kind kind, bool isUnsigned)
{
	return new Builtin(kind, isUnsigned);
}

VectorPtr TarsParse::createVector(const TypePtr &ptr)
{
	return new Vector(ptr);
}

MapPtr TarsParse::createMap(const TypePtr &pleft, const TypePtr &pright)
{
	return new Map(pleft, pright);
}

void TarsParse::addNamespacePtr(const NamespacePtr &nPtr)
{
	_namespaces.push_back(nPtr);
}

NamespacePtr TarsParse::findNamespace(const string &id)
{
	for (size_t i = 0; i < _namespaces.size(); i++)
	{
		if (_namespaces[i]->getId() == id)
		{
			return _namespaces[i];
		}
	}

	return NULL;
}

NamespacePtr TarsParse::currentNamespace()
{
	return _namespaces.back();
}

void TarsParse::addStructPtr(const StructPtr &sPtr)
{
	_structs.push_back(sPtr);
}

void TarsParse::addEnumPtr(const EnumPtr &ePtr)
{
	_enums.push_back(ePtr);
}

StructPtr TarsParse::findStruct(const string &sid)
{
	string ssid = sid;

	//�ڵ�ǰnamespace�в���
	NamespacePtr np = currentNamespace();
	if (ssid.find("::") == string::npos)
	{
		ssid = np->getId() + "::" + ssid;
	}

	for (size_t i = 0; i < _structs.size(); i++)
	{
		if (_structs[i]->getSid() == ssid)
		{
			return _structs[i];
		}
	}

	return NULL;
}

EnumPtr TarsParse::findEnum(const string &sid)
{
	string ssid = sid;

	//�ڵ�ǰnamespace�в���
	NamespacePtr np = currentNamespace();
	if (ssid.find("::") == string::npos)
	{
		ssid = np->getId() + "::" + sid;
	}

	for (size_t i = 0; i < _enums.size(); i++)
	{
		if (_enums[i]->getSid() == ssid)
		{
			return _enums[i];
		}
	}

	return NULL;
}

bool TarsParse::checkEnum(const string &idName)
{
	for (size_t i = 0; i < _enums.size(); i++)
	{
		vector<TypeIdPtr> & list = _enums[i]->getAllMemberPtr();

		for (size_t j = 0; j < list.size(); j++)
		{
			if (list[j]->getId() == idName)
			{
				return true;
			}
		}
	}

	return false;
}
void TarsParse::checkConflict(const string &sid)
{
	//�Ƿ��ö������
	if (findEnum(sid))
	{
		error("conflicts with enum '" + sid + "'");
	}

	//�Ƿ�ͽṹ����
	if (findStruct(sid))
	{
		error("conflicts with struct '" + sid + "'");
	}
}

TypePtr TarsParse::findUserType(const string &sid)
{
	StructPtr sPtr = findStruct(sid);
	if (sPtr) return sPtr;

	EnumPtr ePtr = findEnum(sid);
	if (ePtr) return ePtr;

	return NULL;
}

ContainerPtr TarsParse::currentContainer()
{
	return _contains.top();
}

void TarsParse::pushContainer(const ContainerPtr &c)
{
	_contains.push(c);
	NamespacePtr np = NamespacePtr::dynamicCast(c);
	if (np)
	{
		addNamespacePtr(np);
	}
}

ContainerPtr TarsParse::popContainer()
{
	ContainerPtr c = _contains.top();
	_contains.pop();

	return c;
}

void TarsParse::checkTag(int i)
{
	if (i >= 256)
	{
		error("struct memeber tag can't beyond 256");
	}
	if (i < 0)
	{
		error("struct memeber tag can't less then 0");
	}
}

void TarsParse::checkSize(int i)
{
	if (i >= 1024 * 1024)
	{
		error("struct memeber size can't beyond 1M");
	}
	if (i < 1)
	{
		error("struct memeber size can't less than 1");
	}
}

void TarsParse::checkArrayVaid(TypePtr &tPtr, int size)
{
	checkSize(size);
	//ֻ��string/vector����Ϊarray����
	//vector������Ƕ��array���� ���粻����vector<string:8>:2; 

	VectorPtr vPtr = VectorPtr::dynamicCast(tPtr);
	if (vPtr)
	{
		if (vPtr->getTypePtr()->isArray())
		{
			error("fixed array type can not be nested");
		}
		return;
	}
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(tPtr);
	if (bPtr && bPtr->kind() == Builtin::KindString)
	{
		return;
	}
	error("only string or vector can use fix array");
}

void TarsParse::checkPointerVaid(TypePtr &tPtr)
{
	//����Ϊvector<Byte>����

	VectorPtr vPtr = VectorPtr::dynamicCast(tPtr);
	if (vPtr)
	{
		BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
		if (bPtr && bPtr->kind() == Builtin::KindByte)
			return;
	}
	error("only 'byte *' can  be pointer type");
}

void TarsParse::checkConstValue(TypeIdPtr &tPtr, int c)
{
	//ֻ���ڽ����Ͳ�����ȱʡֵ
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(tPtr->getTypePtr());
	EnumPtr ePtr = EnumPtr::dynamicCast(tPtr->getTypePtr());
	if (!bPtr && !ePtr)
	{
		error("only base/enum type can have default value");
	}

	if (ePtr)
	{
		if (c != ConstGrammar::VALUE && c != ConstGrammar::ENUM)
		{
			error("default value of enum only be int or enum_type");
		}

		return;
	}

	int b = bPtr->kind();

	if (c == ConstGrammar::VALUE)
	{
		if (b == Builtin::KindBool)
		{
			error("default value of bool can only be true or false");
		}
		if (b == Builtin::KindString)
		{
			error("default value of string can only be \"string\"");
		}
	}
	else if (c == ConstGrammar::BOOL)
	{
		if (b != Builtin::KindBool)
		{
			error("only bool type can be true or false");
		}
	}
	else if (c == ConstGrammar::STRING)
	{
		if (b != Builtin::KindString)
		{
			error("only string type can be \"string\"");
		}
	}
}

string TarsParse::printHeaderRemark()
{
	ostringstream s;
	s << "// **********************************************************************" << endl;
	s << "// This file was generated by a TARS parser!" << endl;
	s << "// TARS version " << TARS_VERSION << "." << endl;
	s << "// **********************************************************************" << endl;
	s << endl;

	return s.str();
}