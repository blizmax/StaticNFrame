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
	std::cout << "--XButton or -x Close the 'X' button, only on windows" << std::endl;
	std::cout << "--Plugin=Plugin.lua Load the plugin when programs be launched" << std::endl;
	std::cout << "--Server=AllServer Load the AllServer plugin when programs be launched" << std::endl;
	std::cout << "--ID=number(0) Load the number Server when programs be launched" << std::endl;
	std::cout << "--PATH=../Config Load the Config Path when programs be launched" << std::endl;
	std::cout << "--LuaScript=../ScriptModule Load the Config Path when programs be launched" << std::endl;
	std::cout << "--LogPath=./spdlog Load the Config Path when programs be launched" << std::endl;
	std::cout << "Input '--Exit' Programs will exit when it runs" << std::endl;
	std::cout << "Input '--Reload' Programs will reload config when it runs" << std::endl;
	std::cout << "Input '--Profiler' Programs will open/close profiler when it runs" << std::endl;
	std::cout << "\n" << std::endl;

#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}
