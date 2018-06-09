// -------------------------------------------------------------------------
//    @FileName         :    NFUnitTest.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFUnitTest
//
// -------------------------------------------------------------------------
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
#include "NFAlgorithm_Test.h"
#include "NFThread_Test.h"
#include "NFFileString_Test.h"
#include "NFJson_test.h"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	NFSLEEP(10000);
	return 0;
}