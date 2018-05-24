// -------------------------------------------------------------------------
//    @FileName         :    NFAlgorithm_Test.h
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

#include "NFComm/NFCore/NFPlatform.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include "NFComm/NFCore/NFBase64.h"
#include "NFComm/NFCore/NFCRC32.h"
#include "NFComm/NFCore/NFCRC16.h"
#include "NFComm/NFCore/NFSHA1.h"
#include "NFComm/NFCore/NFMD5.h"
#include "NFComm/NFCore/NFMMOMD5.h"

TEST(NF_Core, NF_BASE64)
{
	std::string gaoyi = "gaoyi";
	std::string code;
	std::string code2;

	EXPECT_NE(true, NFBase64::Encode("", &code2));
	EXPECT_NE(true, NFBase64::Encode(gaoyi, nullptr));

	EXPECT_EQ(true, NFBase64::Encode(gaoyi, &code));

	std::string gaoyi2;
	std::string gaoyi3;

	EXPECT_NE(true, NFBase64::Decode("", &gaoyi3));
	EXPECT_NE(true, NFBase64::Decode("", nullptr));
	EXPECT_NE(true, NFBase64::Decode(code, nullptr));

	EXPECT_EQ(true, NFBase64::Decode(code, &gaoyi2));
	EXPECT_EQ(gaoyi, gaoyi2);
}

