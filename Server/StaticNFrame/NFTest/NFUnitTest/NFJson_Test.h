// -------------------------------------------------------------------------
//    @FileName         :    NFJson_Test.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
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

#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFFileUtility.h"

TEST(NF_Core, NF_JSON)
{
	NFJson json = NFJson::object {{"key1","value1"},{"key2","value2"}};
	std::cout << json.dump() << std::endl;
}
