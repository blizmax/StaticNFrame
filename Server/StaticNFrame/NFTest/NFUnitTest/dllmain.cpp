#include "NFComm/NFCore/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "Dbghelp.lib" )

#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "gtest.lib" )
#endif

