// -------------------------------------------------------------------------
//    @FileName         :    NFPluginLoader.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------

//#include <crtdbg.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <atomic>

#include "NFComm/NFPluginModule/NFCPluginManager.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFProfiler.h"
#include "NFComm/NFCore/NFCmdLine.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFCore/NFFileUtility.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/prctl.h>
#endif

bool bExitApp = false;
std::thread gThread;

#if NF_PLATFORM == NF_PLATFORM_WIN

void CreateDumpFile(const std::string& strDumpFilePathName, EXCEPTION_POINTERS* pException)
{
	//Dump
	HANDLE hDumpFile = CreateFile(strDumpFilePathName.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);

	CloseHandle(hDumpFile);
}

long ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	time_t t = time(nullptr);
	char szDmupName[MAX_PATH];
	tm* ptm = localtime(&t);

	sprintf_s(szDmupName, "%04d_%02d_%02d_%02d_%02d_%02d.dmp", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	CreateDumpFile(szDmupName, pException);

	FatalAppExit(-1, szDmupName);

	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void CloseXButton()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	HWND hWnd = GetConsoleWindow();
	if (hWnd)
	{
		HMENU hMenu = GetSystemMenu(hWnd, FALSE);
		EnableMenuItem(hMenu, SC_CLOSE, MF_DISABLED | MF_BYCOMMAND);
	}
#endif
}

void ThreadFunc()
{
	while (!bExitApp)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		std::string s;
		std::cin >> s;
		if (0 == stricmp(s.c_str(), "exit"))
		{
			bExitApp = true;
			gThread.detach();
		}
	}
}

void CreateBackThread()
{
	gThread = std::thread(std::bind(&ThreadFunc));
	std::async(std::launch::deferred, std::bind(ThreadFunc));
	std::cout << "CreateBackThread, thread ID = " << gThread.get_id() << std::endl;
}

static void sig_usr(int signo)
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	switch (signo)
	{
	case SIGUSR1:
	case SIGUSR2:
	case SIGKILL:
	case SIGINT:
	case SIGQUIT:
	case SIGHUP:
	case SIGTERM:
	{
		NFCPluginManager::GetSingletonPtr()->BeforeShut();
		NFCPluginManager::GetSingletonPtr()->Shut();
		NFCPluginManager::GetSingletonPtr()->Finalize();

		NFCPluginManager::GetSingletonPtr()->ReleaseInstance();
	}
	break;
	default:
		break;
	}
#endif
	exit(0);
}

void InitSignal()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	signal(SIGUSR1, sig_usr);
	signal(SIGUSR2, sig_usr);
	signal(SIGKILL, sig_usr);
	signal(SIGINT, sig_usr);
	signal(SIGQUIT, sig_usr);
	signal(SIGHUP, sig_usr);
	signal(SIGTERM, sig_usr);
	// ignore signals



	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
#endif
}

void InitDaemon()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	daemon(1, 0);
#endif
}

void PrintfLogo()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

	std::cout << "\n" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "**                StaticNFrame                **" << std::endl;
	std::cout << "**          Copyright (c) 2018, Yi.Gao        **" << std::endl;
	std::cout << "**             All rights reserved.           **" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << "\n" << std::endl;
	std::cout << "--Daemon or -d Run it as daemon mode, only on linux" << std::endl;
	std::cout << "--XButton or -x1 Close the 'X' button, only on windows" << std::endl;
	std::cout << "--Plugin=Plugin.lua Load the plugin when programs be launched" << std::endl;
	std::cout << "--Server=AllServer Load the AllServer plugin when programs be launched" << std::endl;
	std::cout << "--ID=number(0) Load the number Server when programs be launched" << std::endl;
	std::cout << "--PATH=../Config Load the Config Path when programs be launched" << std::endl;
	std::cout << "--LuaScript=../ScriptModule Load the Config Path when programs be launched" << std::endl;
	std::cout << "--LogPath=./spdlog Load the Config Path when programs be launched" << std::endl;
	std::cout << "Input 'Exit' Programs will exit when it runs" << std::endl;
	std::cout << "\n" << std::endl;

#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void ProcessParameter(int argc, char* argv[])
{
	try
	{
		NFCmdLine::NFParser cmdParser;

		cmdParser.Add<std::string>("Server", 'S', "Server Name", false, "AllServer");
		cmdParser.Add<int>("ID", 'I', "Server ID", false, 6);
		cmdParser.Add<std::string>("Path", 'P', "Config Path", false, "../Config");
		cmdParser.Add<std::string>("Plugin", 'p', "Plugin Config", false, "Plugin.lua");
		cmdParser.Add<std::string>("LuaScript", 'l', "Lua Script Path", false, "../ScriptModule");
		cmdParser.Add<std::string>("LogPath", 'o', "Log Path", false, "logs");
		
		cmdParser.Add("XButton", 'x', "Close the 'X' button, only on windows");
		cmdParser.Add("Daemon", 'd', "Run it as daemon mode, only on linux");

		cmdParser.Usage();

		cmdParser.ParseCheck(argc, argv);

#if NF_PLATFORM == NF_PLATFORM_WIN
		if (cmdParser.Exist("XButton"))
		{
			CloseXButton();
		}
#else
		//run it as a daemon process
		if (cmdParser.Exist("Daemon"))
		{
			NFCPluginManager::GetSingletonPtr()->SetDaemon();
			InitDaemon();
		}

		InitSignal();
#endif

		std::string strPluginName = cmdParser.Get<std::string>("Plugin");
		NFCPluginManager::GetSingletonPtr()->SetConfigName(strPluginName);
		std::string strAppName = cmdParser.Get<std::string>("Server");
		NFCPluginManager::GetSingletonPtr()->SetAppName(strAppName);
		int nAppID = cmdParser.Get<int>("ID");
		NFCPluginManager::GetSingletonPtr()->SetAppID(nAppID);
		std::string strDataPath = cmdParser.Get<std::string>("Path");
		NFCPluginManager::GetSingletonPtr()->SetConfigPath(strDataPath);
		std::string luaScript = cmdParser.Get<std::string>("LuaScript");
		NFCPluginManager::GetSingletonPtr()->SetLuaScriptPath(luaScript);
		std::string logPath = cmdParser.Get<std::string>("LogPath");
		NFCPluginManager::GetSingletonPtr()->SetLogPath(logPath);

		std::string strTitleName = "NF" + strAppName + lexical_cast<std::string>(nAppID);// +" PID" + NFGetPID();
#if NF_PLATFORM == NF_PLATFORM_WIN
		SetConsoleTitle(strTitleName.c_str());
#elif NF_PLATFORM == NF_PLATFORM_LINUX
		prctl(PR_SET_NAME, strTitleName.c_str());
		//setproctitle(strTitleName.c_str());
#endif
		//std::string pid_str = NFGetPID();
		//NFFileUtility::WriteFile("panic." + pid_str, pid_str.data(), pid_str.length());
	}
	catch (NFCmdLine::NFCmdLine_Error& e)
	{
		std::cout << e.what() << std::endl;
		NFSLEEP(10000);
		exit(0);
	}
}

int main(int argc, char* argv[])
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	//SetConsoleOutputCP(CP_UTF8);
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#elif NF_PLATFORM == NF_PLATFORM_LINUX
#endif

	ProcessParameter(argc, argv);

	PrintfLogo();
	CreateBackThread();

	NFCPluginManager::GetSingletonPtr()->Awake();
	NFCPluginManager::GetSingletonPtr()->Init();
	NFCPluginManager::GetSingletonPtr()->AfterInit();
	NFCPluginManager::GetSingletonPtr()->CheckConfig();
	NFCPluginManager::GetSingletonPtr()->ReadyExecute();

	uint64_t nIndex = 0;
	while (!bExitApp)
	{
		while (true)
		{
			nIndex++;

			if (bExitApp)
			{
				break;
			}

#if NF_PLATFORM == NF_PLATFORM_WIN
			__try
			{
#endif
				NFCPluginManager::GetSingletonPtr()->Execute();

#if NF_PLATFORM == NF_PLATFORM_WIN
			}
			__except (ApplicationCrashHandler(GetExceptionInformation()))
			{
			}
#endif
		}
	}

	NFCPluginManager::GetSingletonPtr()->BeforeShut();
	NFCPluginManager::GetSingletonPtr()->Shut();
	NFCPluginManager::GetSingletonPtr()->Finalize();

	NFCPluginManager::GetSingletonPtr()->ReleaseInstance();

	return 0;
}