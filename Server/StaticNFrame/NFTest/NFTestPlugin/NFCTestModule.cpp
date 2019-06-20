// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"

NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

bool NFCTestModule::Init()
{
	return true;
}

bool NFCTestModule::AfterInit()
{
	return true;
}

bool NFCTestModule::Execute()
{
	static bool flag = true;
	if (flag)
	{
		flag = false;
		std::vector<std::string> vec = { "userid", "regdate" };
		std::vector<std::map<std::string, std::string>> data;
		std::map<std::string, std::string> oneData;
		FindModule<NFIAsyMysqlModule>()->QueryMoreWithCond("dy_player", "userid>=101200 and userid < 101203", vec);
		data.clear();
		FindModule<NFIAsyMysqlModule>()->ExecuteOne("select * from dy_player limit 1");
		FindModule<NFIAsyMysqlModule>()->ExecuteMore("select * from dy_player limit 1");
	}
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