// -------------------------------------------------------------------------
//    @FileName         :    NFConsoleModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-30
//    @Module           :    NFConsoleModule
//
// -------------------------------------------------------------------------

#include "NFConsoleModule.h"
#include "NFComm/NFPluginModule/NFIConsoleModule.h"

#include <string>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

NFCConsoleModule::NFCConsoleModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCConsoleModule::~NFCConsoleModule()
{
	mThread.join();
}

bool NFCConsoleModule::Awake()
{
	try
	{
		mCmdParser.Add("Exit", 0, "Exit App");
		mCmdParser.Add("Reload", 0, "Reload Plugin Config");
		mCmdParser.Add("Profiler", 0, "Open Profiler");

		//mCmdParser.ParseCheck();

		//std::string strPluginName = mCmdParser.Get<std::string>("Plugin");
	}
	catch (NFCmdLine::NFCmdLine_Error& e)
	{
		NFLogWarning(NF_LOG_MONITOR_PLUGIN, 0, e.what());
	}

	if (m_pPluginManager->IsDaemon() == false)
	{
		CreateBackThread();
	}

	return true;
}

void NFCConsoleModule::CreateBackThread()
{
	mThread = std::thread(&NFCConsoleModule::BackThreadLoop, this);
	NFLogInfo(NF_LOG_MONITOR_PLUGIN, 0, "CreateBackThread...............");
}

bool  NFCConsoleModule::Execute()
{
	return true;
}

bool  NFCConsoleModule::OnReloadPlugin()
{
	return true;
}

void NFCConsoleModule::BackThreadLoop()
{
	while (!m_pPluginManager->GetExitApp())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		std::string s;
		std::cin >> s;

		if (s.empty())
		{
			continue;
		}

		std::string cmd = m_pPluginManager->GetAppName() + " " + s;
		try
		{
			if (!mCmdParser.ParseConsoleCommand(cmd))
			{
				continue;
			}

			if (mCmdParser.Exist("Exit"))
			{
				m_pPluginManager->SetExitApp(true);
				return;
			}

			if (mCmdParser.Exist("Reload"))
			{
				m_pPluginManager->OnReloadPlugin();
			}

			if (mCmdParser.Exist("Profiler"))
			{
				m_pPluginManager->SetOpenProfiler(!m_pPluginManager->IsOpenProfiler());
			}
		}
		catch (NFCmdLine::NFCmdLine_Error& e)
		{
			NFLogWarning(NF_LOG_MONITOR_PLUGIN, 0, e.what());
		}
	}
}