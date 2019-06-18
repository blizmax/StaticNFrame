// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIMysqlModule
//
//
// -------------------------------------------------------------------------

#ifndef NFI_MYSQL_MODULE_H
#define NFI_MYSQL_MODULE_H

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIMysqlModule
	: public NFIModule
{
public:
	virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;
	virtual bool UpdateOne(const google::protobuf::Message& message) = 0;
	virtual bool QueryOne(google::protobuf::Message& message) = 0;
	virtual bool QueryMore(google::protobuf::Message& message) = 0;
	virtual bool Execute(const std::string& qstr) = 0;

	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) = 0;
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap) = 0;

	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec) = 0;
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec) = 0;
	
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::map<std::string, std::string>& valueVec) = 0;
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec) = 0;
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec) = 0;
	
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) = 0;
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit) = 0;

	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec) = 0;
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	virtual bool LuaUpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap)
	{
		return UpdateOne(strTableName, strKeyColName, strKey, keyvalueMap);
	}

	virtual std::map<std::string, std::string> LuaQueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec)
	{
		std::map<std::string, std::string> valueVec;
		if (!QueryOne(strTableName, strKeyColName, strKey, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaQueryOne error!");
		}
		return valueVec;
	}

	virtual std::vector<std::map<std::string, std::string>> LuaQueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMore(strTableName, strKeyColName, strKey, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaQueryMore error!");
		}
		return valueVec;
	}

	virtual std::vector<std::map<std::string, std::string>> LuaQueryMoreWithCond(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMoreWithCond(strTableName, strKeyColName, nOffset, nRows, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "QueryMoreWithCond error!");
		}
		return valueVec;
	}

	virtual bool LuaExists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey)
	{
		bool bExit = false;
		if (!Exists(strTableName, strKeyColName, strKey, bExit))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaExists error!");
		}
		return bExit;
	}

	virtual std::vector<std::map<std::string, std::string>> LuaQueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMoreByLike(strTableName, strKeyColName, strKey, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaKeys error!");
		}
		return valueVec;
	}
};
#endif

