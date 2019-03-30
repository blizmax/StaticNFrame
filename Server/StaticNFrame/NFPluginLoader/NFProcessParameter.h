// -------------------------------------------------------------------------
//    @FileName         :    NFPrintfLogo.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-30
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------

#pragma once

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <atomic>

#include "NFCPluginManager.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFCmdLine.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFCore/NFFileUtility.h"

#include "NFCrashHandlerMgr.h"
#include "NFPrintfLogo.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/prctl.h>
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

//转变成守护进程后，会新建一个进程
void InitDaemon()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	daemon(1, 0);
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
