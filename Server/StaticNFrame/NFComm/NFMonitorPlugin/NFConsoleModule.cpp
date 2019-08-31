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
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCConsoleModule::NFCConsoleModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCConsoleModule::~NFCConsoleModule()
{
	
}

bool NFCConsoleModule::Shut()
{
	if (mThread.joinable())
	{
		mThread.detach();
	}
	return true;
}

bool NFCConsoleModule::Awake()
{
	if (m_pPluginManager->IsDaemon() == false)
	{
		try
		{
			mCmdParser.Add("Exit", 0, "Exit App");
			mCmdParser.Add("Profiler", 0, "Open Profiler");
			mCmdParser.Add("ProductFile", 0, "Product File, node header file, sql file, prrotobuf file");

			mCmdParser.Add<std::string>("Dynamic", 0, "Dynamic Load Plugin", false, "xxPlugin");
			mCmdParser.Add<std::string>("Reload", 0, "Reload plugin/alllua/luafiles", false, "");
		}
		catch (NFCmdLine::NFCmdLine_Error& e)
		{
			NFLogWarning(NF_LOG_MONITOR_PLUGIN, 0, e.what());
		}

		/*
		* 创建检查输入线程
		*/
		CreateBackThread();
		/*
		* 定时器每1秒检查一次， 看是否有输入
		*/
		SetTimer(0, 1000, INFINITY_CALL);
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
			mCmdParser.ClearParse();
			if (!mCmdParser.ParseConsoleCommand(cmd))
			{
				std::cout << mCmdParser.ErrorFull() << std::endl;
				std::cout << mCmdParser.Usage() << std::endl;
				continue;
			}

			if (mCmdParser.Exist("Exit"))
			{
				NFConsoleMsg msg;
				msg.mMsgType = NFConsoleMsg_Exit;
				mQueueMsg.Push(msg);
				mThread.detach();
				return;
			}

			if (mCmdParser.Exist("Profiler"))
			{
				NFConsoleMsg msg;
				msg.mMsgType = NFConsoleMsg_Profiler;
				mQueueMsg.Push(msg);
			}

			if (mCmdParser.Exist("Dynamic"))
			{
				std::string strPluginName = mCmdParser.Get<std::string>("Dynamic");
				if (!strPluginName.empty())
				{
					NFConsoleMsg msg;
					msg.mMsgType = NFConsoleMsg_Dynamic;
					msg.mParam1 = strPluginName;
					mQueueMsg.Push(msg);
				}
			}

			if (mCmdParser.Exist("Reload"))
			{
				std::string strConfigName = mCmdParser.Get<std::string>("Reload");
				if (!strConfigName.empty())
				{
					NFConsoleMsg msg;
					msg.mMsgType = NFConsoleMsg_Reload;
					msg.mParam1 = strConfigName;
					mQueueMsg.Push(msg);
				}
			}

			if (mCmdParser.Exist("ProductFile"))
			{
				NFConsoleMsg msg;
				msg.mMsgType = NFConsoleMsg_ProductFile;
				mQueueMsg.Push(msg);
			}
		}
		catch (NFCmdLine::NFCmdLine_Error& e)
		{
			NFLogWarning(NF_LOG_MONITOR_PLUGIN, 0, e.what());
		}
	}
}

void NFCConsoleModule::OnTimer(uint32_t nTimerID)
{
	std::vector<NFConsoleMsg> vec;
	mQueueMsg.Pop(vec);

	for (size_t i = 0; i < vec.size(); i++)
	{
		const NFConsoleMsg& msg = vec[i];

		if (msg.mMsgType == NFConsoleMsg_Exit)
		{
			m_pPluginManager->SetExitApp(true);
		}
		else if (msg.mMsgType == NFConsoleMsg_Profiler)
		{
			m_pPluginManager->SetOpenProfiler(!m_pPluginManager->IsOpenProfiler());
		}
		else if (msg.mMsgType == NFConsoleMsg_Reload)
		{
			if (NFStringUtility::ToLower(msg.mParam1) == "plugin")
			{
				m_pPluginManager->OnReloadPlugin();
			}
			else if (NFStringUtility::ToLower(msg.mParam1) == "alllua")
			{
				NFILuaScriptModule* pLuaModule = FindModule<NFILuaScriptModule>();
				if (pLuaModule)
				{
					pLuaModule->ReloadAllLuaFiles();
				}
			}
			else if (NFStringUtility::ToLower(msg.mParam1) == "luafiles")
			{
				NFILuaScriptModule* pLuaModule = FindModule<NFILuaScriptModule>();
				if (pLuaModule)
				{
					pLuaModule->ReloadLuaFiles();
				}
			}
		}
		else if (msg.mMsgType == NFConsoleMsg_Dynamic)
		{
			m_pPluginManager->DynamicLoadPluginLibrary(msg.mParam1);
		}
		else if (msg.mMsgType == NFConsoleMsg_ProductFile)
		{
			FindModule<NFIConfigModule>()->ProductFile();
		}
	}
}