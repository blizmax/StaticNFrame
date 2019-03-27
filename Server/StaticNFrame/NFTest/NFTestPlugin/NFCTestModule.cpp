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
#include "NFComm/NFSqlitePlugin/NFCSqliteDriver.h"

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
		NFLogDebug(0,0,"a={},b={},c={}", a, b, c);
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

struct tagRecordInfo
{
	uint32_t	lID;
	uint32_t    lRecordID;
	uint32_t    lServerID;
	int64_t		lActorDBID;
	int64_t		lEnterTime;
	int64_t     lLeaveTime;
	int64_t		lEnterScore;
	int64_t     lLeaveScore;
	int64_t     lChangeScore;
	int64_t     lTotalWinScore;
	int64_t		lTotalLostScore;
	int64_t		lTotalWinHBQCount;
	int64_t		lTotalWinBoxCount;
	int64_t		lTotalWinDrawCount;
	int64_t		lTotalShopingScore;

	virtual const std::string& GetTableName()
	{
		static std::string strTableName = "UserInOutRecord";
		return strTableName;
	}

	virtual const std::string& GetTableColumnInfo()
	{
		static std::string strTableColumnInfo = "ID INTEGER PRIMARY KEY,"
			"'RecordID'		int,"
			"'ServerID'		int,"
			"'UserDBID'		bigint,"
			"'EnterTime'	bigint,"
			"'LeaveTime'	bigint,"
			"'EnterScore'	bigint,"
			"'LeaveScore'	bigint,"
			"'ChangeScore'	bigint,"
			"'TotalWinScore'	bigint,"
			"'TotalLostScore'	bigint,"
			"'TotalWinHBQCount'	bigint,"
			"'TotalWinBoxCount'	bigint,"
			"'TotalWinDrawCount'	bigint,"
			"'TotalShopingScore'	bigint";
		return strTableColumnInfo;
	}

	virtual const std::string& CreateSqlitePrepareSql()
	{
		static std::string strSql = "insert into UserInOutRecord values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		return strSql;
	}

	virtual void SqliteBindStep(sqlite3_stmt *pStmt)
	{
		int ret = 0;
		ret = sqlite3_bind_int(pStmt, 2, lRecordID);
		ret = sqlite3_bind_int64(pStmt, 3, lServerID);
		ret = sqlite3_bind_int64(pStmt, 4, lActorDBID);
		ret = sqlite3_bind_int64(pStmt, 5, lEnterTime);
		ret = sqlite3_bind_int64(pStmt, 6, lLeaveTime);
		ret = sqlite3_bind_int64(pStmt, 7, lEnterScore);
		ret = sqlite3_bind_int64(pStmt, 8, lLeaveScore);
		ret = sqlite3_bind_int64(pStmt, 9, lChangeScore);
		ret = sqlite3_bind_int64(pStmt, 10, lTotalWinScore);
		ret = sqlite3_bind_int64(pStmt, 11, lTotalLostScore);
		ret = sqlite3_bind_int64(pStmt, 12, lTotalWinHBQCount);
		ret = sqlite3_bind_int64(pStmt, 13, lTotalWinBoxCount);
		ret = sqlite3_bind_int64(pStmt, 14, lTotalWinDrawCount);
		ret = sqlite3_bind_int64(pStmt, 15, lTotalShopingScore);
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

	tagRecordInfo* pInsertInfo = new tagRecordInfo();
	pInsertInfo->lActorDBID = NFGetTime();
	pInsertInfo->lEnterTime = NFGetSecondTime();
	pInsertInfo->lLeaveTime = NFGetSecondTime() + 1000000;

	sqlite3_stmt *stmt1;
	std::string strDrop = "drop table if exists " + pInsertInfo->GetTableName();
	rc = sqlite3_exec(db, strDrop.c_str(), 0, 0, 0);

	std::string strCreateTable = "create table " + pInsertInfo->GetTableName() + "(" + pInsertInfo->GetTableColumnInfo() + ")";
	rc = sqlite3_exec(db, strCreateTable.c_str(), 0, 0, 0);

	std::string sql = pInsertInfo->CreateSqlitePrepareSql();

	uint64_t startTime = NFGetTime();
	rc = sqlite3_exec(db, "begin;", 0, 0, 0);
	assert(rc == 0);
	rc = sqlite3_prepare_v2(db, sql.data(), sql.size(), &stmt1, 0);
	assert(rc == 0);
	for (int i = 0; i < 100000; ++i)
	{
		rc = sqlite3_reset(stmt1);
		assert(rc == 0);
		pInsertInfo->SqliteBindStep(stmt1);
		rc = sqlite3_step(stmt1);
		assert(rc == 100);
	}
	rc = sqlite3_finalize(stmt1);
	assert(rc == 0);
	char *errmsg;
	rc = sqlite3_exec(db, "commit;", 0, 0, &errmsg);
	assert(rc == 0);

	uint64_t useTime = NFGetTime() - startTime;
	std::cout << "use time :" << useTime << " ms" << std::endl;

	rc = sqlite3_close_v2(db);
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
		NFLogDebug(0,0,str.c_str());
	}

	uint64_t endTime = NFGetTime();
	std::cout << "log 100000 cost time: " << (endTime - startTime) << "ms" << std::endl;
}

bool NFCTestModule::Init()
{
	testSqlite3();
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