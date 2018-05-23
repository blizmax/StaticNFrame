// -------------------------------------------------------------------------
//    @FileName         :    NFFileString_Test.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFUnitTest
//
// -------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <cstdarg>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#include <map>
#include <typeinfo>
#include <iomanip>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>

#include "googletest/googletest/include/gtest/gtest.h"

#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFCore/NFCmdLine.h"

TEST(NF_Core, NF_FILESTRING)
{
	std::string typeName = NFStringUtility::GetTypeName<NFStringUtility>();
	std::cout << "NFStringUtility::GetTypeName<std::string>() = " << typeName << std::endl;
}

TEST(NF_Core, NF_CMDLINE)
{
	char *argv[10];

	argv[0] = "a.out";
	argv[1] = "--Server=AllServer";
	argv[2] = "--Id=6";
	argv[3] = "--PATH=../";
	argv[4] = "--XButton";

	NFCmdLine::NFParser cmd;
	cmd.Add<std::string>("Server", 'S', "Server Name", true, "AllServer");
	cmd.Add<int>("ID", 'I', "Server ID", true, 0);
	cmd.Add<std::string>("Path", 'P', "Config Path", false, "../");
	cmd.Add("XButton", 'x', "Win X Button");
	cmd.Add("Daemon", 'd', "Win X Button");

	cmd.ParseCheck(5, argv);
	EXPECT_EQ("AllServer", cmd.Get<std::string>("server"));
	EXPECT_EQ(6, cmd.Get<int>("ID"));
	EXPECT_EQ("../", cmd.Get<std::string>("PATH"));
}
