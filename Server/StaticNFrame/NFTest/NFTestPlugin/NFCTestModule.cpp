// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFMemChunk.h"
#include "NFComm/NFCore/NFMallocChunk.h"
#include "NFComm/NFCore/NFSimpleBuffer.h"
//#include "NFComm/NFCore/NFMemHashMap.h"
#include "NFComm/NFCore/NFMemHashMap2.h"
#include "NFComm/NFCore/NFShm.h"
#include "sqlite3/sqlite3.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFMessageDefine/NFMsgDefine.h"

struct Test
{
	Test()
	{
		a = 0;
		b = 0;
		c = 0;
	}

	Test(uint32_t _a , uint32_t _b, uint32_t _c)
	{
		a = _a;
		b = _b;
		c = _c;
	}

	void print()
	{
		NFLogDebug("a={},b={},c={}", a, b, c);
	}
	uint32_t a;
	uint32_t b;
	uint32_t c;
};


NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

void testHashMap()
{
	NFShm shm;
	shm.init(1024 * 1024 * 32, 8888, true);

	NFMemHashMap<int, Test> memHashMap;
	memHashMap.create(shm.getPointer(), 1024 * 1024 * 32);

	{
		memHashMap.insert(1, Test(1, 2, 3));
		memHashMap.insert(2, Test(4, 5, 6));
		memHashMap.insert(3, Test(7, 8, 9));
	}


	NFMemHashMap<int, Test> memHashMap2;
	memHashMap2.connect(shm.getPointer(), 1024 * 1024 * 32);

	{
		for (auto iter = memHashMap2.begin(); iter != memHashMap2.end(); iter++)
		{
			iter->getValue().print();
		}
	}
}

struct sqliteInsertInfo
{
	virtual const std::string& GetTableName() = 0;
	virtual const std::string& GetTableColumnInfo() = 0;
	virtual const std::string& CreateSqlitePrepareSql() = 0;
	virtual void SqliteBindStep(sqlite3_stmt *pStmt) = 0;
};

struct tagRecordInfo : public sqliteInsertInfo
{
	int64_t		lActorDBID;
	int64_t		lEnterTime;
	int64_t     lLeaveTime;

	virtual const std::string& GetTableName()
	{
		static std::string strTableName = "UserInOutRecord";
		return strTableName;
	}

	virtual const std::string& GetTableColumnInfo()
	{
		static std::string strTableColumnInfo = "ID INTEGER PRIMARY KEY,"
			"'ServerID'		int,"
			"'UserDBID'		bigint,"
			"'EnterTime'	int,"
			"'LeaveTime'	int,"
			"'Time'			datetime";
		return strTableColumnInfo;
	}

	virtual const std::string& CreateSqlitePrepareSql()
	{
		static std::string strSql = "insert into UserInOutRecord values(?,?,?,?,?,?)";
		return strSql;
	}

	virtual void SqliteBindStep(sqlite3_stmt *pStmt)
	{
		int ret = 0;
		ret = sqlite3_bind_int(pStmt, 2, 1);
		ret = sqlite3_bind_int64(pStmt, 3, lActorDBID);
		ret = sqlite3_bind_int64(pStmt, 4, lEnterTime);
		ret = sqlite3_bind_int64(pStmt, 5, lLeaveTime);
	}
};

void testSqlite3()
{
	sqlite3* db;
	int rc = sqlite3_open("test.db", &db);
	sqlite3_exec(db, "PRAGMA synchronous = OFF; ", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA auto_vacuum = 0; ", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA cache_size = 8000; ", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA case_sensitive_like = 1; ", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA count_changes = 1; ", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA page_size = 8192; ", 0, 0, 0);
	sqlite3_exec(db, "PRAGMA temp_store = MEMORY; ", 0, 0, 0);

	tagRecordInfo* pTagInfo = new tagRecordInfo();
	pTagInfo->lActorDBID = NFGetTime();
	pTagInfo->lEnterTime = NFGetSecondTime();
	pTagInfo->lLeaveTime = NFGetSecondTime() + 1000000;
	sqliteInsertInfo* pInsertInfo = pTagInfo;

	std::string strDrop = "drop table if exists " + pInsertInfo->GetTableName();
	rc = sqlite3_exec(db, strDrop.c_str(), 0, 0, 0);

	std::string strCreateTable = "create table " + pInsertInfo->GetTableName() + "(" + pInsertInfo->GetTableColumnInfo() + ")";
	rc = sqlite3_exec(db, strCreateTable.c_str(), 0, 0, 0);

	std::string sql = pInsertInfo->CreateSqlitePrepareSql();

	uint64_t startTime = NFGetTime();
	rc = sqlite3_exec(db, "begin;", 0, 0, 0);
	sqlite3_stmt *stmt1;
	rc = sqlite3_prepare_v2(db, sql.data(), sql.size(), &stmt1, 0);
	for (int i = 0; i < 100; ++i)
	{
		rc = sqlite3_reset(stmt1);
		pInsertInfo->SqliteBindStep(stmt1);
		rc = sqlite3_step(stmt1);
	}
	rc = sqlite3_finalize(stmt1);
	char *errmsg;
	rc = sqlite3_exec(db, "commit;", 0, 0, &errmsg);

	uint64_t endTime = NFGetTime();

	rc = sqlite3_close_v2(db);
	std::cout << "sqlite3 300000 cost time: " << (endTime - startTime) << "ms" << std::endl;
}

void testLog()
{
	uint64_t startTime = NFGetTime();

	for (int i = 0; i < 1000; i++)
	{
		std::string str;
		for (uint32_t i = 0; i < 1000; i++)
		{
			str += "fuck your mother.................................................";
		}
		NFLogDebug(str.c_str());
	}

	uint64_t endTime = NFGetTime();
	std::cout << "log 100000 cost time: " << (endTime - startTime) << "ms" << std::endl;
}

bool NFCTestModule::Init()
{
	std::string table_name = NFProtobufCommon::GetDBNameFromMessage(proto::message::table_user::default_instance());
	return true;
}

bool NFCTestModule::AfterInit()
{
	return true;
}

bool NFCTestModule::Execute()
{
	return true;
}

bool NFCTestModule::BeforeShut()
{
	return true;
}

bool NFCTestModule::Shut()
{
	return true;
}